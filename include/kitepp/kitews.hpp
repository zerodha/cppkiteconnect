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
#include <vector>

#include "config.hpp"
#include "kiteppexceptions.hpp"
#include "responses.hpp"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rjhelper.hpp"
#include "uWS.h"

namespace kitepp {

// To make sure doubles are parsed correctly
static_assert(std::numeric_limits<double>::is_iec559, "Requires IEEE 754 floating point!");

using std::string;

namespace rj = rapidjson;
namespace rjh = RJHelper;

class kiteWS {

  public:
    // member variables
    // user constants
    // FIXME make these static
    const string MODE_LTP = "ltp";
    const string MODE_QUOTE = "quote";
    const string MODE_FULL = "full";

    // callbacks
    std::function<void(kiteWS* ws)> onConnect;
    std::function<void(kiteWS* ws, const std::vector<kitepp::tick>& ticks)> onTicks;
    std::function<void(kiteWS* ws, const kitepp::postback& postback)> onOrderUpdate;
    std::function<void(kiteWS* ws, const string& message)> onMessage;
    std::function<void(kiteWS* ws, int code, const string& message)> onError;
    std::function<void(kiteWS* ws)> onConnectError;
    std::function<void(kiteWS* ws, unsigned int attemptCount)> onTryReconnect;
    std::function<void(kiteWS* ws)> onReconnectFail;
    std::function<void(kiteWS* ws, int code, const string& message)> onClose;

    // constructors & destructors

    kiteWS(const string& apikey, unsigned int connecttimeout = 5, bool enablereconnect = false,
        unsigned int maxreconnectdelay = 60, unsigned int maxreconnecttries = 30)
        : _apiKey(apikey), _connectTimeout(connecttimeout * 1000), _enableReconnect(enablereconnect),
          _maxReconnectDelay(maxreconnectdelay), _maxReconnectTries(maxreconnecttries),
          _hubGroup(_hub.createGroup<uWS::CLIENT>()) {};

    // x~kiteWS() {};

    // methods

    /**
     * @brief Set the API key
     *
     * @param arg
     */
    void setAPIKey(const string& arg) { _apiKey = arg; };

    /**
     * @brief get set API key
     *
     * @return string
     */
    string getAPIKey() const { return _apiKey; };

    /**
     * @brief Set the Access Token
     *
     * @param arg the string you want to set as access token
     *
     * @paragraph ex1 example
     * @snippet example2.cpp settting access token
     */
    void setAccessToken(const string& arg) { _accessToken = arg; };

    /**
     * @brief Get the Access Token set currently
     *
     * @return string
     */
    string getAccessToken() const { return _accessToken; };

    void connect() {
        _assignCallbacks();
        _connect();
    };

    bool isConnected() const { return _WS; };

    std::chrono::time_point<std::chrono::system_clock> getLastBeatTime() { return _lastBeatTime; };

    void run() { _hub.run(); };

    void stop() {
        if (isConnected()) { _WS->close(); };
    };

    void subscribe(const std::vector<int>& instrumentToks) {
        rj::Document req;
        req.SetObject();
        auto& reqAlloc = req.GetAllocator();
        rj::Value val;
        rj::Value toksArr(rj::kArrayType);

        val.SetString("subscribe", reqAlloc);
        req.AddMember("a", val, reqAlloc);

        for (const int tok : instrumentToks) { toksArr.PushBack(tok, reqAlloc); }
        req.AddMember("v", toksArr, reqAlloc);

        string reqStr = rjh::_dump(req);
        if (isConnected()) {
            _WS->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT);
            for (const int tok : instrumentToks) { _subbedInstruments[tok] = ""; };

        } else {
            throw kitepp::libException("Not connected to websocket server");
        };
    };

    void unsubscribe(const std::vector<int>& instrumentToks) {

        rj::Document req;
        req.SetObject();
        auto& reqAlloc = req.GetAllocator();
        rj::Value val;
        rj::Value toksArr(rj::kArrayType);

        val.SetString("unsubscribe", reqAlloc);
        req.AddMember("a", val, reqAlloc);

        for (const int tok : instrumentToks) { toksArr.PushBack(tok, reqAlloc); }
        req.AddMember("v", toksArr, reqAlloc);

        string reqStr = rjh::_dump(req);
        if (isConnected()) {

            _WS->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT);
            for (const int tok : instrumentToks) {
                auto it = _subbedInstruments.find(tok);
                if (it != _subbedInstruments.end()) { _subbedInstruments.erase(it); };
            };

        } else {

            throw kitepp::libException("Not connected to websocket server");
        };
    };

    void setMode(const string& mode, const std::vector<int>& instrumentToks) {

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
        for (const int& tok : instrumentToks) { toksArr.PushBack(tok, reqAlloc); }
        valArr.PushBack(toksArr, reqAlloc);
        req.AddMember("v", valArr, reqAlloc);

        string reqStr = rjh::_dump(req);
        if (isConnected()) {

            _WS->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT);
            for (const int tok : instrumentToks) { _subbedInstruments[tok] = mode; };

        } else {

            throw kitepp::libException("Not connected to websocket server");
        };
    };

  private:
    // member variables
    const string _connectURLFmt = "wss://ws.kite.trade/?api_key={0}&access_token={1}";
    string _apiKey;
    string _accessToken;
    const std::unordered_map<string, int> _segmentConstants = {

        { "nse", 1 },
        { "nfo", 2 },
        { "cds", 3 },
        { "bse", 4 },
        { "bfo", 5 },
        { "bsecds", 6 },
        { "mcx", 7 },
        { "mcxsx", 8 },
        { "indices", 9 },
    };
    std::unordered_map<int, string> _subbedInstruments; // instrument ID, mode

    uWS::Hub _hub;
    uWS::Group<uWS::CLIENT>* _hubGroup;
    uWS::WebSocket<uWS::CLIENT>* _WS = nullptr;
    const unsigned int _connectTimeout = 5000; // in ms

    const string _pingMessage = "";
    const unsigned int _pingInterval = 3000; // in ms

    const bool _enableReconnect = false;
    const unsigned int _initReconnectDelay = 2; // in seconds
    unsigned int _reconnectDelay = _initReconnectDelay;
    const unsigned int _maxReconnectDelay = 0; // in seconds
    unsigned int _reconnectTries = 0;
    const unsigned int _maxReconnectTries = 0; // in seconds
    std::atomic<bool> _isReconnecting { false };

    std::chrono::time_point<std::chrono::system_clock> _lastPongTime;
    std::chrono::time_point<std::chrono::system_clock> _lastBeatTime;

    // methods

    void _connect() {
        _hub.connect(FMT(_connectURLFmt, _apiKey, _accessToken), nullptr, {}, _connectTimeout, _hubGroup);
    };

    void _processTextMessage(char* message, size_t length) {
        rj::Document res;
        rjh::_parse(res, string(message, length));
        if (!res.IsObject()) { throw libException("Expected a JSON object"); };

        string type;
        rjh::_getIfExists(res, type, "type");
        if (type.empty()) { throw kitepp::libException(FMT("Cannot recognize websocket message type {0}", type)); }

        if (type == "order" && onOrderUpdate) { onOrderUpdate(this, kitepp::postback(res["data"].GetObject())); }
        if (type == "message" && onMessage) { onMessage(this, string(message, length)); };
        if (type == "error" && onError) { onError(this, 0, res["data"].GetString()); };
    };

    // Convert bytesarray(array[start], arrray[end]) to number of type T
    template <typename T> T _getNum(const std::vector<char>& bytes, size_t start, size_t end) {

        T value;
        std::vector<char> requiredBytes(bytes.begin() + start, bytes.begin() + end + 1);

        // clang-format off
        #ifndef WORDS_BIGENDIAN
        std::reverse(requiredBytes.begin(), requiredBytes.end());
        #endif
        // clang-format on

        std::memcpy(&value, requiredBytes.data(), sizeof(T));

        return value;
    };

    std::vector<std::vector<char>> _splitPackets(const std::vector<char>& bytes) {

        int16_t numberOfPackets = _getNum<int16_t>(bytes, 0, 1);

        std::vector<std::vector<char>> packets;

        unsigned int packetLengthStartIdx = 2;
        for (int i = 1; i <= numberOfPackets; i++) {

            unsigned int packetLengthEndIdx = packetLengthStartIdx + 1;
            int16_t packetLength = _getNum<int16_t>(bytes, packetLengthStartIdx, packetLengthEndIdx);
            packetLengthStartIdx = packetLengthEndIdx + packetLength + 1;
            packets.emplace_back(bytes.begin() + packetLengthEndIdx + 1, bytes.begin() + packetLengthStartIdx);
        };

        return packets;
    };

    std::vector<kitepp::tick> _parseBinaryMessage(char* bytes, size_t size) {

        std::vector<std::vector<char>> packets = _splitPackets(std::vector<char>(bytes, bytes + size));
        if (packets.empty()) { return {}; };

        std::vector<kitepp::tick> ticks;
        for (const auto& packet : packets) {

            size_t packetSize = packet.size();
            int32_t instrumentToken = _getNum<int32_t>(packet, 0, 3);
            int segment = instrumentToken & 0xff;
            double divisor = (segment == _segmentConstants.at("cds")) ? 10000000.0 : 100.0;
            bool tradable = (segment == _segmentConstants.at("indices")) ? false : true;

            kitepp::tick Tick;

            Tick.isTradable = tradable;
            Tick.instrumentToken = instrumentToken;

            // LTP packet
            if (packetSize == 8) {

                Tick.mode = MODE_LTP;
                Tick.lastPrice = _getNum<int32_t>(packet, 4, 7) / divisor;

            } else if (packetSize == 28 || packetSize == 32) {
                // indices quote and full mode

                Tick.mode = (packetSize == 28) ? MODE_QUOTE : MODE_FULL;
                Tick.lastPrice = _getNum<int32_t>(packet, 4, 7) / divisor;
                Tick.OHLC.high = _getNum<int32_t>(packet, 8, 11) / divisor;
                Tick.OHLC.low = _getNum<int32_t>(packet, 12, 15) / divisor;
                Tick.OHLC.open = _getNum<int32_t>(packet, 16, 19) / divisor;
                Tick.OHLC.close = _getNum<int32_t>(packet, 20, 23) / divisor;
                // xTick.netChange = (Tick.lastPrice - Tick.OHLC.close) * 100 / Tick.OHLC.close;
                Tick.netChange = _getNum<int32_t>(packet, 24, 27) / divisor;

                // parse full mode with timestamp
                if (packetSize == 32) { Tick.timestamp = _getNum<int32_t>(packet, 28, 33); }

            } else if (packetSize == 44 || packetSize == 184) {
                // Quote and full mode

                Tick.mode = (packetSize == 44) ? MODE_QUOTE : MODE_FULL;
                Tick.lastPrice = _getNum<int32_t>(packet, 4, 7) / divisor;
                Tick.lastTradedQuantity = _getNum<int32_t>(packet, 8, 11) / divisor;
                Tick.averageTradePrice = _getNum<int32_t>(packet, 12, 15) / divisor;
                Tick.volumeTraded = _getNum<int32_t>(packet, 16, 19) / divisor;
                Tick.totalBuyQuantity = _getNum<int32_t>(packet, 20, 23) / divisor;
                Tick.totalSellQuantity = _getNum<int32_t>(packet, 24, 27) / divisor;
                Tick.OHLC.high = _getNum<int32_t>(packet, 28, 31) / divisor;
                Tick.OHLC.low = _getNum<int32_t>(packet, 32, 35) / divisor;
                Tick.OHLC.open = _getNum<int32_t>(packet, 36, 39) / divisor;
                Tick.OHLC.close = _getNum<int32_t>(packet, 40, 43) / divisor;

                Tick.netChange = (Tick.lastPrice - Tick.OHLC.close) * 100 / Tick.OHLC.close;

                // parse full mode
                if (packetSize == 184) {

                    Tick.lastTradeTime = _getNum<int32_t>(packet, 44, 47) / divisor;
                    Tick.OI = _getNum<int32_t>(packet, 48, 51) / divisor;
                    Tick.OIDayHigh = _getNum<int32_t>(packet, 52, 55) / divisor;
                    Tick.OIDayLow = _getNum<int32_t>(packet, 56, 59) / divisor;
                    Tick.timestamp = _getNum<int32_t>(packet, 60, 63);

                    unsigned int depthStartIdx = 64;
                    for (int i = 0; i <= 9; i++) {

                        kitepp::depthWS depth;
                        depth.quantity = _getNum<int32_t>(packet, depthStartIdx, depthStartIdx + 3);
                        depth.price = _getNum<int32_t>(packet, depthStartIdx + 4, depthStartIdx + 7) / divisor;
                        depth.orders = _getNum<int32_t>(packet, depthStartIdx + 8, depthStartIdx + 9);

                        (i >= 5) ? Tick.marketDepth.sell.emplace_back(depth) : Tick.marketDepth.buy.emplace_back(depth);
                        depthStartIdx = depthStartIdx + 12;
                    };
                };
            };

            ticks.emplace_back(Tick);
        };

        return ticks;
    };

    void _resubInstruments() {

        std::vector<int> LTPInstruments;
        std::vector<int> quoteInstruments;
        std::vector<int> fullInstruments;
        for (const auto& i : _subbedInstruments) {

            if (i.second == MODE_LTP) { LTPInstruments.push_back(i.first); };
            if (i.second == MODE_QUOTE) { quoteInstruments.push_back(i.first); };
            if (i.second == MODE_FULL) { fullInstruments.push_back(i.first); };
            // Set mode as quote if no mode was set
            if (i.second.empty()) { quoteInstruments.push_back(i.first); };
        };

        if (!LTPInstruments.empty()) { setMode(MODE_LTP, LTPInstruments); };
        if (!quoteInstruments.empty()) { setMode(MODE_QUOTE, quoteInstruments); };
        if (!fullInstruments.empty()) { setMode(MODE_FULL, fullInstruments); };
    };

    void _reconnect() {

        std::cout << "_reconnect called\n";

        if (isConnected()) { return; };

        _isReconnecting = true;
        _reconnectTries++;

        if (_reconnectTries <= _maxReconnectTries) {

            std::this_thread::sleep_for(std::chrono::seconds(_reconnectDelay));
            _reconnectDelay = (_reconnectDelay * 2 > _maxReconnectDelay) ? _maxReconnectDelay : _reconnectDelay * 2;

            if (onTryReconnect) { onTryReconnect(this, _reconnectTries); };
            _connect();

            if (isConnected()) { return; };

        } else {

            if (onReconnectFail) { onReconnectFail(this); };
            _isReconnecting = false;
        };
    };

    /*
    handled by uWS::Group::StartAutoPing() now. Code kept here as fallback.
    void _pingLoop() {

        while (!_stop) {

            std::cout << "Sending ping..\n";
            if (isConnected()) { _WS->ping(_pingMessage.data()); };
            std::this_thread::sleep_for(std::chrono::seconds(_pingInterval));

            if (_enableReconnect) {

                auto tmDiff =
                    std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - _lastPongTime)
                        .count();

                if (tmDiff > _maxPongDelay) {
                    std::cout << FMT("Max pong exceeded.. tmDiff={0}\n", tmDiff);
                    if (isConnected()) { _WS->close(1006, "ping timed out"); };
                };
            };
        };
    };*/

    void _assignCallbacks() {

        _hubGroup->onConnection([&](uWS::WebSocket<uWS::CLIENT>* ws, uWS::HttpRequest req) {
            std::cout << "connected...\n";
            _WS = ws;
            // Not setting this time would prompt reconnecting immediately, even when conected since pongTime would be
            // far back or default
            _lastPongTime = std::chrono::system_clock::now();

            _reconnectTries = 0;
            _reconnectDelay = _initReconnectDelay;
            _isReconnecting = false;
            if (!_subbedInstruments.empty()) { _resubInstruments(); };
            if (onConnect) { onConnect(this); };
        });

        _hubGroup->onMessage([&](uWS::WebSocket<uWS::CLIENT>* ws, char* message, size_t length, uWS::OpCode opCode) {
            if (opCode == uWS::OpCode::BINARY && onTicks) {

                if (length == 1) {
                    // is a heartbeat
                    _lastBeatTime = std::chrono::system_clock::now();
                } else {
                    onTicks(this, _parseBinaryMessage(message, length));
                };

            } else if (opCode == uWS::OpCode::TEXT) {
                _processTextMessage(message, length);
            };
        });

        _hubGroup->onPong([&](uWS::WebSocket<uWS::CLIENT>* ws, char* message, size_t length) {
            std::cout << "Pong recieved..\n";
            _lastPongTime = std::chrono::system_clock::now();
        });

        _hubGroup->onError([&](void*) {
            if (onConnectError) { onConnectError(this); }

            // Close the non-responsive connection
            if (isConnected()) {

                std::cout << "***Closing connection in onError***\n";
                _WS->close(1006);
            };
            if (_enableReconnect) { _reconnect(); };
        });

        _hubGroup->onDisconnection([&](uWS::WebSocket<uWS::CLIENT>* ws, int code, char* reason, size_t length) {
            std::cout << "Disconnection code: " << code << "\n";
            _WS = nullptr;

            if (code != 1000) {
                if (onError) { onError(this, code, string(reason, length)); };
            };
            if (onClose) { onClose(this, code, string(reason, length)); };
            if (code != 1000) {
                if (_enableReconnect && !_isReconnecting) { _reconnect(); };
            };
        });

        _hubGroup->startAutoPing(_pingInterval, _pingMessage);
    };
};

} // namespace kitepp
