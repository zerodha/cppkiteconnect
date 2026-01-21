/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2022 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a
 * copy of this software and associated  documentation files (the "Software"),
 * to deal in the Software  without restriction, including without  limitation
 * the rights to  use, copy,  modify, merge,  publish, distribute,  sublicense,
 * and/or  sell copies  of  the Software,  and  to  permit persons  to  whom the
 * Software  is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS
 * OR IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN
 * NO EVENT  SHALL THE AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY
 * CLAIM,  DAMAGES OR  OTHER LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <algorithm> //reverse
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring> //memcpy
#include <functional>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../exceptions.hpp"
#include "../responses/responses.hpp"
#include "../userconstants.hpp" //modes
#include "../utils.hpp"
#include "ws.hpp"

#include <glaze/glaze.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

namespace kiteconnect {
// To make sure doubles are parsed correctly
static_assert(std::numeric_limits<double>::is_iec559,
    "Requires IEEE 754 floating point!");
using std::string;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

namespace internal {
// Helper structs for JSON parsing with glaze
struct WsTypeMessage {
    string type;
};

struct WsErrorMessage {
    string data;
};
} // namespace internal

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
inline ticker::ticker(string Key, unsigned int ConnectTimeout,
    bool EnableReconnect, unsigned int maxreconnectdelay,
    unsigned int MaxReconnectTries)
    : key(std::move(Key)),
      connectTimeout(ConnectTimeout),
      enableReconnect(EnableReconnect), maxReconnectDelay(maxreconnectdelay),
      maxReconnectTries(MaxReconnectTries) {
    // Load default SSL certificates
    sslCtx.set_default_verify_paths();
    sslCtx.set_verify_mode(ssl::verify_peer);
}

inline ticker::~ticker() {
    stopRequested = true;
    if (pingTimer) {
        pingTimer->cancel();
    }
    if (ws && connected) {
        beast::error_code ec;
        ws->close(websocket::close_code::normal, ec);
    }
    ioc.stop();
}

inline void ticker::setApiKey(const string& Key) { key = Key; }

inline string ticker::getApiKey() const { return key; }

inline void ticker::setAccessToken(const string& Token) { token = Token; }

inline string ticker::getAccessToken() const { return token; }

inline void ticker::connect() {
    connectInternal();
}

inline bool ticker::isConnected() const { return connected.load(); }

inline std::chrono::time_point<std::chrono::system_clock> ticker::
    getLastBeatTime() const {
    return lastBeatTime;
}

inline void ticker::run() {
    ioc.run();
}

inline void ticker::stop() {
    stopRequested = true;
    if (pingTimer) {
        pingTimer->cancel();
    }
    if (isConnected()) {
        ws->async_close(websocket::close_code::normal,
            [this](beast::error_code ec) {
                connected = false;
                if (onClose) {
                    onClose(this, static_cast<int>(websocket::close_code::normal),
                            "Connection closed by client");
                }
            });
    }
}

inline void ticker::subscribe(const std::vector<int>& instrumentTokens) {
    utils::json::json<utils::json::JsonObject> req;
    req.field("a", "subscribe");
    req.field("v", instrumentTokens);
    string reqStr = req.serialize();

    if (isConnected()) {
        sendMessage(reqStr);
        for (const int tok : instrumentTokens) {
            subbedInstruments[tok] = DEFAULT_MODE;
        }
    } else {
        throw kc::libException("not connected to websocket server");
    }
}

inline void ticker::unsubscribe(const std::vector<int>& instrumentTokens) {
    utils::json::json<utils::json::JsonObject> req;
    req.field("a", "unsubscribe");
    req.field("v", instrumentTokens);
    string reqStr = req.serialize();

    if (isConnected()) {
        sendMessage(reqStr);
        for (const int tok : instrumentTokens) {
            auto it = subbedInstruments.find(tok);
            if (it != subbedInstruments.end()) { subbedInstruments.erase(it); }
        }
    } else {
        throw kc::libException("not connected to websocket server");
    }
}

inline void ticker::setMode(
    const string& mode, const std::vector<int>& instrumentTokens) {
    // create request json: {"a": "mode", "v": ["full", [408065, 2953217]]}
    auto req = glz::obj{
        "a", "mode",
        "v", std::make_tuple(mode, instrumentTokens)
    };
    string reqStr = glz::write_json(req).value_or("");

    if (isConnected()) {
        sendMessage(reqStr);
        for (const int tok : instrumentTokens) {
            if (mode == MODE_LTP) {
                subbedInstruments[tok] = MODES::LTP;
            } else if (mode == MODE_QUOTE) {
                subbedInstruments[tok] = MODES::QUOTE;
            } else {
                subbedInstruments[tok] = MODES::FULL;
            }
        }
    } else {
        throw kc::libException("not connected to websocket server");
    }
}

inline void ticker::sendMessage(const string& message) {
    if (!isConnected() || !ws) {
        return;
    }
    ws->text(true);
    ws->async_write(net::buffer(message),
        [this](beast::error_code ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                if (onError) {
                    onError(this, ec.value(), ec.message());
                }
            }
        });
}

inline void ticker::connectInternal() {
    // Reset io_context if it was stopped
    if (ioc.stopped()) {
        ioc.restart();
    }

    // Create new WebSocket stream
    ws = std::make_unique<WsStream>(ioc, sslCtx);

    // Set SNI hostname (required for SSL)
    if (!SSL_set_tlsext_host_name(ws->next_layer().native_handle(), wsHost.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        if (onConnectError) { onConnectError(this); }
        if (enableReconnect) { reconnect(); }
        return;
    }

    // Set timeout for the TCP connect operation
    beast::get_lowest_layer(*ws).expires_after(std::chrono::seconds(connectTimeout));

    // Start async DNS resolution
    resolver.async_resolve(wsHost, wsPort,
        [this](beast::error_code ec, tcp::resolver::results_type results) {
            if (ec) {
                if (onConnectError) { onConnectError(this); }
                if (enableReconnect) { reconnect(); }
                return;
            }
            doConnect(results);
        });
}

inline void ticker::doConnect(const tcp::resolver::results_type& results) {
    // Set timeout
    beast::get_lowest_layer(*ws).expires_after(std::chrono::seconds(connectTimeout));

    // Make the TCP connection
    beast::get_lowest_layer(*ws).async_connect(results,
        [this](beast::error_code ec, tcp::resolver::results_type::endpoint_type /*ep*/) {
            if (ec) {
                if (onConnectError) { onConnectError(this); }
                if (enableReconnect) { reconnect(); }
                return;
            }
            doSslHandshake();
        });
}

inline void ticker::doSslHandshake() {
    // Set timeout
    beast::get_lowest_layer(*ws).expires_after(std::chrono::seconds(connectTimeout));

    // Perform SSL handshake
    ws->next_layer().async_handshake(ssl::stream_base::client,
        [this](beast::error_code ec) {
            if (ec) {
                if (onConnectError) { onConnectError(this); }
                if (enableReconnect) { reconnect(); }
                return;
            }
            doWsHandshake();
        });
}

inline void ticker::doWsHandshake() {
    // Turn off timeout for websocket (we'll handle our own ping/pong)
    beast::get_lowest_layer(*ws).expires_never();

    // Set suggested timeout settings for the websocket
    ws->set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent
    ws->set_option(websocket::stream_base::decorator(
        [](websocket::request_type& req) {
            req.set(boost::beast::http::field::user_agent,
                std::string("cppkiteconnect/boost-beast"));
        }));

    // Set up pong handler
    ws->control_callback([this](websocket::frame_type kind, beast::string_view /*payload*/) {
        if (kind == websocket::frame_type::pong) {
            lastPongTime = std::chrono::system_clock::now();
        }
    });

    // Build the target path with query parameters
    string target = "/?api_key=" + key + "&access_token=" + token;

    // Perform WebSocket handshake
    ws->async_handshake(wsHost, target,
        [this](beast::error_code ec) {
            if (ec) {
                if (onConnectError) { onConnectError(this); }
                if (enableReconnect) { reconnect(); }
                return;
            }
            onConnected();
        });
}

inline void ticker::onConnected() {
    connected = true;
    lastPongTime = std::chrono::system_clock::now();

    reconnectTries = 0;
    reconnectDelay = initReconnectDelay;
    isReconnecting = false;

    if (!subbedInstruments.empty()) {
        resubInstruments();
    }

    if (onConnect) {
        onConnect(this);
    }

    // Start ping timer
    startPingTimer();

    // Start reading messages
    doRead();
}

inline void ticker::doRead() {
    if (stopRequested || !ws) {
        return;
    }

    ws->async_read(readBuffer,
        [this](beast::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                connected = false;

                if (ec == websocket::error::closed) {
                    // Normal closure
                    if (onClose) {
                        onClose(this, static_cast<int>(websocket::close_code::normal),
                                "Connection closed");
                    }
                } else {
                    // Error
                    if (onError) {
                        onError(this, ec.value(), ec.message());
                    }
                    if (onClose) {
                        onClose(this, ec.value(), ec.message());
                    }
                }

                if (!stopRequested && enableReconnect && !isReconnecting) {
                    reconnect();
                }
                return;
            }

            // Process the message
            if (ws->got_binary()) {
                // Binary message (ticks or heartbeat)
                auto data = readBuffer.data();
                auto* charData = static_cast<char*>(data.data());
                std::size_t size = data.size();

                if (size == 1) {
                    // Heartbeat
                    lastBeatTime = std::chrono::system_clock::now();
                } else if (onTicks) {
                    // Tick data
                    onTicks(this, parseBinaryMessage(charData, size));
                }
            } else {
                // Text message (JSON)
                string message = beast::buffers_to_string(readBuffer.data());
                processTextMessage(message);
            }

            // Clear the buffer
            readBuffer.consume(readBuffer.size());

            // Continue reading
            doRead();
        });
}

inline void ticker::startPingTimer() {
    if (stopRequested) {
        return;
    }

    pingTimer = std::make_unique<net::steady_timer>(ioc);
    sendPing();
}

inline void ticker::sendPing() {
    if (stopRequested || !connected || !ws) {
        return;
    }

    pingTimer->expires_after(std::chrono::milliseconds(pingInterval));
    pingTimer->async_wait([this](const boost::system::error_code& ec) {
        if (ec || stopRequested || !connected) {
            return;
        }

        // Send ping
        ws->async_ping({}, [this](beast::error_code ec) {
            if (ec) {
                // Ping failed
                return;
            }
        });

        // Schedule next ping
        sendPing();
    });
}

inline void ticker::reconnect() {
    if (isConnected()) { return; }
    isReconnecting = true;
    reconnectTries++;

    if (reconnectTries <= maxReconnectTries) {
        // Use async timer for delay instead of blocking sleep
        auto reconnectTimer = std::make_shared<net::steady_timer>(ioc);
        reconnectTimer->expires_after(std::chrono::seconds(reconnectDelay));
        reconnectTimer->async_wait([this, reconnectTimer](const boost::system::error_code& ec) {
            if (ec || stopRequested) {
                isReconnecting = false;
                return;
            }

            reconnectDelay = (reconnectDelay * 2 > maxReconnectDelay) ?
                                 maxReconnectDelay :
                                 reconnectDelay * 2;

            if (onTryReconnect) { onTryReconnect(this, reconnectTries); }
            connectInternal();
        });
    } else {
        if (onReconnectFail) { onReconnectFail(this); }
        isReconnecting = false;
    }
}

inline void ticker::processTextMessage(const string& message) {
    // Parse the type field first to determine message type
    internal::WsTypeMessage typeMsg;
    auto typeErr = glz::read_json(typeMsg, message);
    if (typeErr) {
        throw libException("Expected a JSON object");
    }

    if (typeMsg.type.empty()) {
        throw kc::libException(
            FMT("Cannot recognize websocket message type {0}", typeMsg.type));
    }

    if (typeMsg.type == "order" && onOrderUpdate) {
        kc::postback pb;
        auto err = glz::read_json(pb, message);
        if (!err) {
            onOrderUpdate(this, pb);
        }
    }
    if (typeMsg.type == "message" && onMessage) { onMessage(this, message); }
    if (typeMsg.type == "error" && onError) {
        internal::WsErrorMessage errMsg;
        std::ignore = glz::read_json(errMsg, message);
        onError(this, 0, errMsg.data);
    }
}

template <typename T>
T ticker::unpack(const std::vector<char>& bytes, size_t start, size_t end) {
    // FIXME directly iterate over bytes instead of making a copy or reversing
    T value;
    std::vector<char> requiredBytes(bytes.begin() + static_cast<int64_t>(start),
        bytes.begin() + static_cast<int64_t>(end) + 1);

    // clang-format off
        #ifndef WORDS_BIGENDIAN
        std::reverse(requiredBytes.begin(), requiredBytes.end());
        #endif
    // clang-format on

    std::memcpy(&value, requiredBytes.data(), sizeof(T));
    return value;
}

inline std::vector<std::vector<char>> ticker::splitPackets(
    const std::vector<char>& bytes) {
    const auto numberOfPackets = unpack<int16_t>(bytes, 0, 1);
    std::vector<std::vector<char>> packets;

    unsigned int packetLengthStartIdx = 2;
    for (int i = 1; i <= numberOfPackets; i++) {
        unsigned int packetLengthEndIdx = packetLengthStartIdx + 1;
        auto packetLength =
            unpack<int16_t>(bytes, packetLengthStartIdx, packetLengthEndIdx);
        packetLengthStartIdx = packetLengthEndIdx + packetLength + 1;
        packets.emplace_back(bytes.begin() + packetLengthEndIdx + 1,
            bytes.begin() + packetLengthStartIdx);
    }
    return packets;
}

inline std::vector<kc::tick> ticker::parseBinaryMessage(
    char* bytes, size_t size) {
    static constexpr uint8_t SEGMENT_MASK = 0xff;
    static constexpr double CDS_DIVISOR = 10000000.0;
    static constexpr double BSECDS_DIVISOR = 10000.0;
    static constexpr double GENERIC_DIVISOR = 100.0;
    static constexpr size_t LTP_PACKET_SIZE = 8;
    static constexpr size_t INDICES_QUOTE_PACKET_SIZE = 28;
    static constexpr size_t INDICES_FULL_PACKET_SIZE = 32;
    static constexpr size_t QUOTE_PACKET_SIZE = 44;
    static constexpr size_t FULL_PACKET_SIZE = 184;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::vector<std::vector<char>> packets =
        splitPackets(std::vector<char>(bytes, bytes + size));
    if (packets.empty()) { return {}; }

    std::vector<kc::tick> ticks;
    for (const auto& packet : packets) {
        const size_t packetSize = packet.size();
        const auto instrumentToken = unpack<int32_t>(packet, 0, 3);
        // NOLINTNEXTLINE(hicpp-signed-bitwise)
        const uint8_t segment = instrumentToken & SEGMENT_MASK;
        const bool tradable =
            segment != static_cast<uint8_t>(SEGMENTS::INDICES);
        double divisor = 0.0;
        if (segment == static_cast<uint8_t>(SEGMENTS::CDS)) {
            divisor = CDS_DIVISOR;

        } else if (segment == static_cast<uint8_t>(SEGMENTS::BSECDS)) {
            divisor = BSECDS_DIVISOR;

        } else {
            divisor = GENERIC_DIVISOR;
        }

        kc::tick Tick;
        Tick.isTradable = tradable;
        Tick.instrumentToken = instrumentToken;

        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
        // LTP packet
        if (packetSize == LTP_PACKET_SIZE) {
            Tick.mode = MODE_LTP;
            Tick.lastPrice = unpack<int32_t>(packet, 4, 7) / divisor;
        } else if (packetSize == INDICES_QUOTE_PACKET_SIZE ||
                   packetSize == INDICES_FULL_PACKET_SIZE) {
            // indices quote and full mode
            Tick.mode = (packetSize == INDICES_QUOTE_PACKET_SIZE) ? MODE_QUOTE :
                                                                    MODE_FULL;
            Tick.lastPrice = unpack<int32_t>(packet, 4, 7) / divisor;
            Tick.ohlc.high = unpack<int32_t>(packet, 8, 11) / divisor;
            Tick.ohlc.low = unpack<int32_t>(packet, 12, 15) / divisor;
            Tick.ohlc.open = unpack<int32_t>(packet, 16, 19) / divisor;
            Tick.ohlc.close = unpack<int32_t>(packet, 20, 23) / divisor;
            Tick.netChange = unpack<int32_t>(packet, 24, 27) / divisor;
            if (packetSize == INDICES_FULL_PACKET_SIZE) {
                Tick.timestamp = unpack<int32_t>(packet, 28, 31);
            }
        } else if (packetSize == QUOTE_PACKET_SIZE ||
                   packetSize == FULL_PACKET_SIZE) {
            // Quote and full mode
            Tick.mode =
                (packetSize == QUOTE_PACKET_SIZE) ? MODE_QUOTE : MODE_FULL;
            Tick.lastPrice = unpack<int32_t>(packet, 4, 7) / divisor;
            Tick.lastTradedQuantity = unpack<int32_t>(packet, 8, 11);
            Tick.averageTradePrice = unpack<int32_t>(packet, 12, 15) / divisor;
            Tick.volumeTraded = unpack<int32_t>(packet, 16, 19);
            Tick.totalBuyQuantity = unpack<int32_t>(packet, 20, 23);
            Tick.totalSellQuantity = unpack<int32_t>(packet, 24, 27);
            Tick.ohlc.open = unpack<int32_t>(packet, 28, 31) / divisor;
            Tick.ohlc.high = unpack<int32_t>(packet, 32, 35) / divisor;
            Tick.ohlc.low = unpack<int32_t>(packet, 36, 39) / divisor;
            Tick.ohlc.close = unpack<int32_t>(packet, 40, 43) / divisor;
            Tick.netChange =
                (Tick.lastPrice - Tick.ohlc.close) * 100 / Tick.ohlc.close;

            // parse full mode
            if (packetSize == FULL_PACKET_SIZE) {
                Tick.lastTradeTime = unpack<int32_t>(packet, 44, 47);
                Tick.oi = unpack<int32_t>(packet, 48, 51);
                Tick.oiDayHigh = unpack<int32_t>(packet, 52, 55);
                Tick.oiDayLow = unpack<int32_t>(packet, 56, 59);
                Tick.timestamp = unpack<int32_t>(packet, 60, 63);

                unsigned int depthStartIdx = 64;
                for (int i = 0; i <= 9; i++) {
                    kc::depthWS depth;
                    depth.quantity = unpack<int32_t>(
                        packet, depthStartIdx, depthStartIdx + 3);
                    depth.price = unpack<int32_t>(packet, depthStartIdx + 4,
                                      depthStartIdx + 7) /
                                  divisor;
                    depth.orders = unpack<int16_t>(
                        packet, depthStartIdx + 8, depthStartIdx + 9);

                    (i >= 5) ? Tick.marketDepth.sell.emplace_back(depth) :
                               Tick.marketDepth.buy.emplace_back(depth);
                    depthStartIdx = depthStartIdx + 12;
                }
            }
        }
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
        ticks.emplace_back(Tick);
    }
    return ticks;
}

inline void ticker::resubInstruments() {
    std::vector<int> ltpInstruments;
    std::vector<int> quoteInstruments;
    std::vector<int> fullInstruments;
    for (const auto& i : subbedInstruments) {
        if (i.second == MODES::LTP) { ltpInstruments.push_back(i.first); }
        if (i.second == MODES::QUOTE) { quoteInstruments.push_back(i.first); }
        if (i.second == MODES::FULL) { fullInstruments.push_back(i.first); }
    }

    if (!ltpInstruments.empty()) { setMode(MODE_LTP, ltpInstruments); }
    if (!quoteInstruments.empty()) { setMode(MODE_QUOTE, quoteInstruments); }
    if (!fullInstruments.empty()) { setMode(MODE_FULL, fullInstruments); }
}

} // namespace kiteconnect
