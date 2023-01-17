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

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/writer.h"
#include <uWS/uWS.h>

namespace kiteconnect {
// To make sure doubles are parsed correctly
static_assert(std::numeric_limits<double>::is_iec559,
    "Requires IEEE 754 floating point!");
using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
inline ticker::ticker(string Key, unsigned int ConnectTimeout,
    bool EnableReconnect, unsigned int maxreconnectdelay,
    unsigned int MaxReconnectTries)
    : key(std::move(Key)),
      connectTimeout(ConnectTimeout * utils::MILLISECONDS_IN_A_SECOND),
      enableReconnect(EnableReconnect), maxReconnectDelay(maxreconnectdelay),
      maxReconnectTries(MaxReconnectTries),
      group(hub.createGroup<uWS::CLIENT>()) {};

inline void ticker::setApiKey(const string& Key) { key = Key; };

inline string ticker::getApiKey() const { return key; };

inline void ticker::setAccessToken(const string& Token) { token = Token; };

inline string ticker::getAccessToken() const { return token; };

inline void ticker::connect() {
    assignCallbacks();
    connectInternal();
};

inline bool ticker::isConnected() const { return ws != nullptr; };

inline std::chrono::time_point<std::chrono::system_clock> ticker::
    getLastBeatTime() const {
    return lastBeatTime;
};

inline void ticker::run() { hub.run(); };

inline void ticker::stop() {
    if (isConnected()) { ws->close(); };
};

inline void ticker::subscribe(const std::vector<int>& instrumentTokens) {
    utils::json::json<utils::json::JsonObject> req;
    req.field("a", "subscribe");
    req.field("v", instrumentTokens);
    string reqStr = req.serialize();

    if (isConnected()) {
        ws->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT);
        for (const int tok : instrumentTokens) {
            subbedInstruments[tok] = DEFAULT_MODE;
        };
    } else {
        throw kc::libException("not connected to websocket server");
    };
};

inline void ticker::unsubscribe(const std::vector<int>& instrumentTokens) {
    utils::json::json<utils::json::JsonObject> req;
    req.field("a", "unsubscribe");
    req.field("v", instrumentTokens);
    string reqStr = req.serialize();

    if (isConnected()) {
        ws->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT);
        for (const int tok : instrumentTokens) {
            auto it = subbedInstruments.find(tok);
            if (it != subbedInstruments.end()) { subbedInstruments.erase(it); };
        };
    } else {
        throw kc::libException("not connected to websocket server");
    };
};

inline void ticker::setMode(
    const string& mode, const std::vector<int>& instrumentTokens) {
    // create request json
    rj::Document req;
    req.SetObject();
    auto& reqAlloc = req.GetAllocator();
    rj::Value val;
    rj::Value valArr(rj::kArrayType);
    rj::Value toksArr(rj::kArrayType);

    val.SetString("mode", reqAlloc);
    req.AddMember("a", val, reqAlloc);

    val.SetString(mode.c_str(), mode.size(), reqAlloc);
    valArr.PushBack(val, reqAlloc);
    for (const int tok : instrumentTokens) { toksArr.PushBack(tok, reqAlloc); }
    valArr.PushBack(toksArr, reqAlloc);
    req.AddMember("v", valArr, reqAlloc);

    // send the request
    string reqStr = utils::json::serialize(req);
    if (isConnected()) {
        ws->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT);
        for (const int tok : instrumentTokens) {
            if (mode == MODE_LTP) {
                subbedInstruments[tok] = MODES::LTP;
            } else if (mode == MODE_QUOTE) {
                subbedInstruments[tok] = MODES::QUOTE;
            } else {
                subbedInstruments[tok] = MODES::FULL;
            }
        };
    } else {
        throw kc::libException("not connected to websocket server");
    };
};

inline void ticker::connectInternal() {
    hub.connect(FMT(connectUrlFmt, key, token), nullptr, {},
        static_cast<int>(connectTimeout), group);
};

inline void ticker::reconnect() {
    if (isConnected()) { return; };
    isReconnecting = true;
    reconnectTries++;

    if (reconnectTries <= maxReconnectTries) {
        std::this_thread::sleep_for(std::chrono::seconds(reconnectDelay));
        reconnectDelay = (reconnectDelay * 2 > maxReconnectDelay) ?
                             maxReconnectDelay :
                             reconnectDelay * 2;

        if (onTryReconnect) { onTryReconnect(this, reconnectTries); };
        connectInternal();

        if (isConnected()) { return; };
    } else {
        if (onReconnectFail) { onReconnectFail(this); };
        isReconnecting = false;
    };
};

inline void ticker::processTextMessage(const string& message) {
    rj::Document res;
    utils::json::parse(res, message);
    if (!res.IsObject()) { throw libException("Expected a JSON object"); };

    auto type = utils::json::get<string>(res, "type");
    if (type.empty()) {
        throw kc::libException(
            FMT("Cannot recognize websocket message type {0}", type));
    }

    if (type == "order" && onOrderUpdate) {
        onOrderUpdate(this, kc::postback(utils::json::extractObject(res)));
    }
    if (type == "message" && onMessage) { onMessage(this, message); };
    if (type == "error" && onError) {
        onError(this, 0, utils::json::extractString(res));
    };
};

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
};

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
    };
    return packets;
};

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
    if (packets.empty()) { return {}; };

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
                };
            };
        };
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
        ticks.emplace_back(Tick);
    };
    return ticks;
};

inline void ticker::resubInstruments() {
    std::vector<int> ltpInstruments;
    std::vector<int> quoteInstruments;
    std::vector<int> fullInstruments;
    for (const auto& i : subbedInstruments) {
        if (i.second == MODES::LTP) { ltpInstruments.push_back(i.first); };
        if (i.second == MODES::QUOTE) { quoteInstruments.push_back(i.first); };
        if (i.second == MODES::FULL) { fullInstruments.push_back(i.first); };
    };

    if (!ltpInstruments.empty()) { setMode(MODE_LTP, ltpInstruments); };
    if (!quoteInstruments.empty()) { setMode(MODE_QUOTE, quoteInstruments); };
    if (!fullInstruments.empty()) { setMode(MODE_FULL, fullInstruments); };
};

inline void ticker::assignCallbacks() {
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    group->onConnection(
        [&](uWS::WebSocket<uWS::CLIENT>* Ws, uWS::HttpRequest /*req*/) {
            ws = Ws;
            //! not setting this time would prompt reconnecting immediately even
            //! when conected since pongTime would be far back
            lastPongTime = std::chrono::system_clock::now();

            reconnectTries = 0;
            reconnectDelay = initReconnectDelay;
            isReconnecting = false;
            if (!subbedInstruments.empty()) { resubInstruments(); };
            if (onConnect) { onConnect(this); };
        });

    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    group->onMessage([&](uWS::WebSocket<uWS::CLIENT>* /*ws*/, char* message,
                         size_t length, uWS::OpCode opCode) {
        if (opCode == uWS::OpCode::BINARY && onTicks) {
            if (length == 1) {
                // is a heartbeat
                lastBeatTime = std::chrono::system_clock::now();
            } else {
                onTicks(this, parseBinaryMessage(message, length));
            };
        } else if (opCode == uWS::OpCode::TEXT) {
            processTextMessage(string(message, length));
        };
    });

    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    group->onPong([&](uWS::WebSocket<uWS::CLIENT>* /*ws*/, char* /*message*/,
                      size_t /*length*/) {
        lastPongTime = std::chrono::system_clock::now();
    });

    group->onError([&](void*) {
        if (onConnectError) { onConnectError(this); }
        // close the non-responsive connection
        if (isConnected()) { ws->close(utils::ws::ERROR_CODE::NO_REASON); };
        if (enableReconnect) { reconnect(); };
    });

    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    group->onDisconnection([&](uWS::WebSocket<uWS::CLIENT>* /*ws*/, int code,
                               char* reason, size_t length) {
        ws = nullptr;

        if (code != utils::ws::ERROR_CODE::NORMAL_CLOSURE) {
            if (onError) { onError(this, code, string(reason, length)); };
        };
        if (onClose) { onClose(this, code, string(reason, length)); };
        if (code != utils::ws::ERROR_CODE::NORMAL_CLOSURE) {
            if (enableReconnect && !isReconnecting) { reconnect(); };
        };
    });

    group->startAutoPing(static_cast<int>(pingInterval), pingMessage);
};

} // namespace kiteconnect