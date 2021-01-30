#pragma once

// FIXME correct this header
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"
#include "kiteppexceptions.hpp"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rjhelper.hpp"
#include "uWS.h"

namespace kitepp {

using std::string;

namespace rj = rapidjson;
namespace rjh = RJHelper;

class kiteWS {

    // FIXME have to create callbacks

  public:
    // member variables

    // callbacks
    std::function<void(kiteWS* ws)> onConnect;
    std::function<void(kiteWS* ws, char* message, size_t length)> onMessage;

    // constructors & destructors

    kiteWS(const string& apiKey)
        : _apiKey(apiKey), _hubGroup(_hub.createGroup<uWS::CLIENT>()) {

          };

    ~kiteWS() {

    };

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

        _hub.connect(FMT("wss://ws.kite.trade?api_key={0}&access_token={1}", _apiKey, _accessToken), nullptr, {}, 5000,
            _hubGroup);

        //! assign callbacks here
        _assignCallbacks();
    };

    void run() { _hub.run(); };

    void subscribe(const std::vector<int>& instrumentToks) {

        rj::Document req;
        req.SetObject();
        auto& reqAlloc = req.GetAllocator();
        rj::Value val;
        rj::Value toksArr(rj::kArrayType);

        val.SetString("subscribe", reqAlloc);
        req.AddMember("a", val, reqAlloc);

        for (const int& tok : instrumentToks) { toksArr.PushBack(tok, reqAlloc); }
        req.AddMember("v", toksArr, reqAlloc);

        string reqStr = rjh::_dump(req);
        (_WS) ? _WS->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT) :
                throw kitepp::libException("_WS doesn't point to anything");
    };

    void unsubscribe(const std::vector<int>& instrumentToks) {

        rj::Document req;
        req.SetObject();
        auto& reqAlloc = req.GetAllocator();
        rj::Value val;
        rj::Value toksArr(rj::kArrayType);

        val.SetString("unsubscribe", reqAlloc);
        req.AddMember("a", val, reqAlloc);

        for (const int& tok : instrumentToks) { toksArr.PushBack(tok, reqAlloc); }
        req.AddMember("v", toksArr, reqAlloc);

        string reqStr = rjh::_dump(req);
        (_WS) ? _WS->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT) :
                throw kitepp::libException("_WS doesn't point to anything");
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
        req.AddMember("v", toksArr, reqAlloc);
        valArr.PushBack(toksArr, reqAlloc);

        string reqStr = rjh::_dump(req);
        (_WS) ? _WS->send(reqStr.data(), reqStr.size(), uWS::OpCode::TEXT) :
                throw kitepp::libException("_WS doesn't point to anything");
    };

  private:
    // member variables
    string _apiKey;
    string _accessToken;
    uWS::Hub _hub;
    uWS::Group<uWS::CLIENT>* _hubGroup;
    uWS::WebSocket<uWS::CLIENT>* _WS; // FIXME will have to invalidate it after disconnecting, errors etc.

    // methods

    void _assignCallbacks() {

        // will need to have a pointer to this class
        _hubGroup->onConnection([&](uWS::WebSocket<uWS::CLIENT>* ws, uWS::HttpRequest req) {
            _WS = ws;
            if (onConnect) { onConnect(this); };
        });

        _hubGroup->onMessage([&](uWS::WebSocket<uWS::CLIENT>* ws, char* message, size_t length, uWS::OpCode opCode) {
            if (onMessage) { onMessage(this, message, length); }
        });
    };
};

} // namespace kitepp