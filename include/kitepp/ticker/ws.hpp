/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
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

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include <uWS/uWS.h>

namespace kiteconnect {

// To make sure doubles are parsed correctly
static_assert(std::numeric_limits<double>::is_iec559,
    "Requires IEEE 754 floating point!");

using std::string;
namespace kc = kiteconnect;

///
/// \brief \a ticker wraps around the websocket API provided by KiteConnect and
///         provides a native interface.
///
class ticker {

  public:
    // callbacks
    /// @brief Called on successful connect.
    std::function<void(ticker* ws)> onConnect;

    /// @brief Called when ticks are received.
    std::function<void(ticker* ws, const std::vector<kc::tick>& ticks)> onTicks;

    /// @brief Called when an order update is received.
    std::function<void(ticker* ws, const kc::postback& postback)> onOrderUpdate;

    /// @brief Called when a message is received.
    std::function<void(ticker* ws, const string& message)> onMessage;

    /// @brief Called when connection is closed with an error or websocket
    ///        server sends an error message.
    std::function<void(ticker* ws, int code, const string& message)> onError;

    /// @brief Called when an error occures while trying to connect.
    std::function<void(ticker* ws)> onConnectError;

    ///
    /// @brief Called when reconnection is being attempted.
    ///
    /// \note Auto reconnection:
    ///       Auto reconnection is disabled by default and can be enabled by
    ///       setting `EnableReconnect` to `true` in `ticker`'s constructor.
    ///       Auto reonnection mechanism is based on exponential backoff
    ///       algorithm in which next retry interval will be increased
    ///       exponentially. MaxReconnectDelay and MaxReconnectTries params can
    ///       be used to tweak the alogrithm. MaxReconnectDelay is the
    ///       maximum delay after which subsequent reconnection interval will
    ///       become constant and MaxReconnectTries is maximum number of retries
    ///       before `ticker` quits trying to reconnect.
    ///
    std::function<void(ticker* ws, unsigned int attemptCount)> onTryReconnect;

    ///
    /// @brief Called when reconnect attempts exceed maximum reconnect attempts
    ///        set by user i.e., when `ticker` is unable to reconnect
    ///
    std::function<void(ticker* ws)> onReconnectFail;

    /// @brief Called when connection is closed.
    std::function<void(ticker* ws, int code, const string& message)> onClose;

    /**
     * @brief Construct a new kiteWS object
     *
     * @param apikey API key
     * @param connecttimeout Connection timeout
     * @param enablereconnect Should be set to `true` for enabling reconnection
     * @param maxreconnectdelay Maximum reconnect delay for reconnection
     * @param maxreconnecttries Maximum reconnection attempts after which
     * onReconnectFail will be called and no further attempt to reconnect will
     * be made.
     */

    ///
    /// \brief Construct a new ticker object. All durations are in seconds.
    ///
    /// \param Key               API key
    /// \param ConnectTimeout    connection timeout
    /// \param EnableReconnect   auto reconnect is enabled if
    ///                          \a EnableReconnect is set to `true`
    /// \param MaxReconnectDelay Maximum delay after which subsequent
    ///                          reconnection interval will become constant
    /// \param MaxReconnectTries Maximum number of retries before `ticker` quits
    ///                          trying to reconnect.
    ///
    explicit ticker(string Key,
        unsigned int ConnectTimeout = DEFAULT_CONNECT_TIMEOUT,
        bool EnableReconnect = false,
        unsigned int MaxReconnectDelay = DEFAULT_MAX_RECONNECT_DELAY,
        unsigned int MaxReconnectTries = DEFAULT_MAX_RECONNECT_TRIES);

    ///
    /// @brief Set the API key.
    ///
    /// @param key API key is set to \a key
    ///
    void setApiKey(const string& key);

    ///
    /// @brief Get API key set at the moment.
    ///
    /// @return string API key
    ///
    string getApiKey() const;

    ///
    /// @brief Set the access token.
    ///
    /// @param token access token is set to \a token.
    ///
    /// @paragraph ex1 example
    /// @snippet example2.cpp settting access token
    ///
    void setAccessToken(const string& token);

    ///
    /// @brief Get access token set at the moment.
    ///
    /// @return string access token
    ///
    string getAccessToken() const;

    /// @brief Connect to the websocket server.
    void connect();

    /// @brief Check if client is connected.
    bool isConnected() const;

    ///
    /// @brief Get the last time heartbeat was received. Should be used in
    ///        conjunction with the `isConnected()` method.
    ///
    /// @return std::chrono::time_point<std::chrono::system_clock> time
    ///
    std::chrono::time_point<std::chrono::system_clock> getLastBeatTime() const;

    /// @brief Start the client. Should always be called after `connect()`.
    void run();

    /// @brief Stop the client. Closes the connection if connected. Should be
    ///        the last method that is called.
    void stop();

    ///
    /// @brief Subscribe to a list of instrument tokens.
    ///
    /// @param instrumentTokens list of instrument tokens that should be
    ///                         subscribed
    ///
    void subscribe(const std::vector<int>& instrumentTokens);

    ///
    /// @brief Unsubscribe.
    ///
    /// @param instrumentTokens list of instrument tokens that should be
    ///                         unsubscribed
    ///
    void unsubscribe(const std::vector<int>& instrumentTokens);

    /**
     * @brief Set the subscription mode for a list of instrument tokens.
     *
     * @param mode             mode to set
     * @param instrumentTokens list of instrument tokens whose mode should be
     *                         set
     */
    void setMode(const string& mode, const std::vector<int>& instrumentTokens);

  private:
    friend class tickerTest_binaryParsingTest_Test;
    const string connectUrlFmt =
        "wss://ws.kite.trade/?api_key={0}&access_token={1}";
    string key;
    string token;
    enum class SEGMENTS : int
    {
        NSE = 1,
        NFO,
        CDS,
        BSE,
        BFO,
        BSECDS,
        MCX,
        MCXSX,
        INDICES
    };
    enum class MODES
    {
        LTP,
        QUOTE,
        FULL
    };
    const MODES DEFAULT_MODE = MODES::QUOTE;
    std::unordered_map<int, MODES> subbedInstruments;
    uWS::Hub hub;
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    uWS::Group<uWS::CLIENT>* group;
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    uWS::WebSocket<uWS::CLIENT>* ws = nullptr;
    static constexpr unsigned int DEFAULT_CONNECT_TIMEOUT = 5;      // s
    static constexpr unsigned int DEFAULT_MAX_RECONNECT_DELAY = 60; // s
    static constexpr unsigned int DEFAULT_MAX_RECONNECT_TRIES = 30;
    const unsigned int connectTimeout = DEFAULT_CONNECT_TIMEOUT; // ms
    const string pingMessage;
    const unsigned int pingInterval = 3000; // ms
    const bool enableReconnect = false;
    const unsigned int initReconnectDelay = 2; // s
    unsigned int reconnectDelay = initReconnectDelay;
    const unsigned int maxReconnectDelay = DEFAULT_MAX_RECONNECT_DELAY; // s
    unsigned int reconnectTries = 0;
    const unsigned int maxReconnectTries = DEFAULT_MAX_RECONNECT_TRIES;
    std::atomic<bool> isReconnecting { false };
    std::chrono::time_point<std::chrono::system_clock> lastPongTime;
    std::chrono::time_point<std::chrono::system_clock> lastBeatTime;

    void connectInternal();

    void reconnect();

    void processTextMessage(const string& message);

    template <typename T>
    T unpack(const std::vector<char>& bytes, size_t start, size_t end);

    std::vector<std::vector<char>> splitPackets(const std::vector<char>& bytes);

    std::vector<kc::tick> parseBinaryMessage(char* bytes, size_t size);

    void resubInstruments();

    void assignCallbacks();
};
} // namespace kiteconnect