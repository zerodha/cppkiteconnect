/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
#pragma once

#include <algorithm> //for_each
#include <array>
#include <iostream> //debug
#include <limits>   //nan
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility> //pair<>
#include <vector>

#include "PicoSHA2/picosha2.h"
#include "cpp-httplib/httplib.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "config.hpp"
#include "kiteppexceptions.hpp"
#include "responses/responses.hpp"
#include "rjutils.hpp"
#include "utils.hpp"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace rju = kc::rjutils;
using kc::_methods;
using kc::DEFAULTDOUBLE;
using kc::DEFAULTINT;
using kc::isValid;
using kc::libException;

/**
 * @brief Used for accessing REST interface provided by Kite API.
 *
 */
class kite {

  public:
    // member variables:

    // constructors and destructor:

    /**
     * @brief Construct a new kite object
     *
     * @param apikey
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp initializing kite
     */
    explicit kite(string apikey): _apiKey(std::move(apikey)), _httpClient(_rootURL.c_str()) {};

    virtual ~kite() {};

    // methods:

    // api:

    /**
     * @brief Set the API key
     *
     * @param arg The string that should be set as API key
     */
    void setAPIKey(const string& arg) { _apiKey = arg; };

    /**
     * @brief Fetch current API key
     *
     * @return string
     */
    string getAPIKey() const { return _apiKey; };

    /**
     * @brief Get the remote login url to which a user should be redirected to initiate the login flow.
     *
     * @return string
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp obtaining login url
     */
    string loginURL() const { return FMT(_loginURLFmt, "api_key"_a = _apiKey); };

    /**
     * @brief Generate user session details like `access_token`
     *
     * @param requestToken
     * @param apiSecret
     * @return userSession
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp obtaining access token
     */
    userSession generateSession(const string& requestToken, const string& apiSecret) {

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("api.token"),
            {
                { "api_key", _apiKey },
                { "request_token", requestToken },
                { "checksum", picosha2::hash256_hex_string(_apiKey + requestToken + apiSecret) },
            });

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (generateSession())");
        };

        return userSession(res["data"].GetObject());
    };

    /**
     * @brief Set the Access Token
     *
     * @param arg the string you want to set as access token
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp settting access token
     */
    void setAccessToken(const string& arg) { _accessToken = arg; };

    /**
     * @brief Get the Access Token set currently
     *
     * @return string
     */
    string getAccessToken() const { return _accessToken; };

    /**
     * @brief This call invalidates the access_token and destroys the API session. After this, the user should be sent
     * through a new login flow before further interactions. This does not log the user out of the official Kite web or
     * mobile applications.
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp invalidate session
     */
    void invalidateSession() {

        rj::Document res;
        _sendReq(res, _methods::DEL,
            FMT(_endpoints.at("api.token.invalidate"), "api_key"_a = _apiKey, "access_token"_a = _accessToken));
    };

    // user:

    /**
     * @brief returns user profile
     *
     * @return userProfile
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get user profile
     *
     */
    userProfile profile() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("user.profile"));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (profile())"); };

        return userProfile(res["data"].GetObject());
    };

    /**
     * @brief Get margins for all segments
     *
     * @return allMargins
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get margins
     */
    allMargins getMargins() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("user.margins"));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getMargins())"); };

        return allMargins(res["data"].GetObject());
    };

    /**
     * @brief Get margins for a particular segment
     *
     * @param segment
     * @return margins
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get margins
     */
    margins getMargins(const string& segment) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("user.margins.segment"), "segment"_a = segment));
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (getMargins(segment))");
        };

        return margins(res["data"].GetObject());
    };

    // orders:

    /**
     * @brief place an order.
     *
     * @param variety variety of the order
     * @param exchange name of the exchange
     * @param symbol trading symbol
     * @param txnType transaction type
     * @param quantity quantity to transact
     * @param product margin product to use for the order (margins are blocked based on this)
     * @param orderType order type (MARKET, LIMIT etc.)
     * @param price the min or max price to execute the order at (for LIMIT orders)
     * @param validity order validity
     * @param trigPrice trigger price
     * @param sqOff square off
     * @param SL stoploss
     * @param trailSL trailing stoploss
     * @param discQuantity disclosed quantity
     * @param tag an optional tag to apply to an order to identify it (alphanumeric, max 20 chars)
     *
     * @return string orderID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp placing an order
     */
    string placeOrder(const placeOrderParams& params) {
        std::vector<std::pair<string, string>> bodyParams = {
            { "exchange", params.exchange },
            { "tradingsymbol", params.symbol },
            { "transaction_type", params.transactionType },
            { "quantity", std::to_string(params.quantity) },
            { "product", params.product },
            { "order_type", params.orderType },
        };

        if (isValidArg(params.price)) { bodyParams.emplace_back("price", std::to_string(params.price)); }
        if (isValidArg(params.validity)) { bodyParams.emplace_back("validity", params.validity); }
        if (isValidArg(params.disclosedQuantity)) {
            bodyParams.emplace_back("disclosed_quantity", std::to_string(params.disclosedQuantity));
        }
        if (isValidArg(params.triggerPrice)) {
            bodyParams.emplace_back("trigger_price", std::to_string(params.triggerPrice));
        }
        if (isValidArg(params.squareOff)) { bodyParams.emplace_back("squareoff", std::to_string(params.squareOff)); }
        if (isValidArg(params.stopLoss)) { bodyParams.emplace_back("stoploss", std::to_string(params.stopLoss)); }
        if (isValidArg(params.trailingStopLoss)) {
            bodyParams.emplace_back("trailing_stoploss", std::to_string(params.trailingStopLoss));
        }
        if (isValidArg(params.tag)) { bodyParams.emplace_back("tag", params.tag); }

        rj::Document res;
        _sendReq(res, _methods::POST, FMT(_endpoints.at("order.place"), "variety"_a = params.variety), bodyParams);
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeOrder)"); };

        string rcvdOrdID;
        rju::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief modify an order
     *
     * @param variety variety of the order
     * @param ordID order ID
     * @param parentOrdID parent order ID
     * @param quantity quantity to transact
     * @param price the min or max price to execute the order at (for LIMIT orders)
     * @param ordType order type
     * @param trigPrice trigger price
     * @param validity order validity
     * @param discQuantity disclosed quantity
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp modifying an order
     */
    string modifyOrder(const modifyOrderParams& params) {

        std::vector<std::pair<string, string>> bodyParams = {};

        if (isValidArg(params.parentOrderId)) { bodyParams.emplace_back("parent_order_id", params.parentOrderId); }
        if (isValidArg(params.quantity)) { bodyParams.emplace_back("quantity", std::to_string(params.quantity)); }
        if (isValidArg(params.price)) { bodyParams.emplace_back("price", std::to_string(params.price)); }
        if (isValidArg(params.orderType)) { bodyParams.emplace_back("order_type", params.orderType); }
        if (isValidArg(params.triggerPrice)) {
            bodyParams.emplace_back("trigger_price", std::to_string(params.triggerPrice));
        }
        if (isValidArg(params.validity)) { bodyParams.emplace_back("validity", params.validity); }
        if (isValidArg(params.disclosedQuantity)) {
            bodyParams.emplace_back("disclosed_quantity", std::to_string(params.disclosedQuantity));
        }

        rj::Document res;
        _sendReq(res, _methods::PUT,
            FMT(_endpoints.at("order.modify"), "variety"_a = params.variety, "order_id"_a = params.orderId),
            bodyParams);
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (modifyOrder)"); };

        string rcvdOrdID;
        rju::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief cancel an order
     *
     * @param variety variety of the order
     * @param ordID order ID
     * @param parentOrdID parent order ID
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp cancelling an order
     */
    string cancelOrder(const string& variety, const string& ordID, const string& parentOrdID = "") {

        rj::Document res;
        (variety == "bo") ? _sendReq(res, _methods::DEL,
                                FMT(_endpoints.at("order.cancel.bo"), "variety"_a = variety, "order_id"_a = ordID,
                                    "parent_order_id"_a = parentOrdID)) :
                            _sendReq(res, _methods::DEL,
                                FMT(_endpoints.at("order.cancel"), "variety"_a = variety, "order_id"_a = ordID));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (cancelOrder)"); };

        string rcvdOrdID;
        rju::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief exit an order
     *
     * @param variety variety of the order
     * @param ordID order ID
     * @param parentOrdID parent order ID
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp exiting an order
     */
    string exitOrder(const string& variety, const string& ordID, const string& parentOrdID = "") {

        return cancelOrder(variety, ordID, parentOrdID);
    };

    /**
     * @brief get list of orders
     *
     * @return std::vector<order>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get orders
     */
    std::vector<order> orders() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("orders"));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (orders())"); };

        auto it = res.FindMember("data");
        if (!it->value.IsArray()) { throw libException("Array was expected (orders())"); };

        std::vector<order> orderVec;
        for (auto& i : it->value.GetArray()) { orderVec.emplace_back(i.GetObject()); }

        return orderVec;
    };

    /**
     * @brief get history of an order
     *
     * @param ordID order ID
     *
     * @return std::vector<order>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get order history
     */
    std::vector<order> orderHistory(const string& ordID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("order.info"), "order_id"_a = ordID));

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (orderHistory())");
        };

        auto it = res.FindMember("data");
        if (!it->value.IsArray()) { throw libException("Array was expected (orderHistory())"); };

        std::vector<order> orderVec;
        for (auto& i : it->value.GetArray()) { orderVec.emplace_back(i.GetObject()); }

        return orderVec;
    };

    /**
     * @brief get list of trades
     *
     * @return std::vector<trade>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get trades
     */
    std::vector<trade> trades() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("trades"));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (trades())"); };

        auto it = res.FindMember("data");
        if (!it->value.IsArray()) { throw libException("Array was expected (trades())"); };

        std::vector<trade> tradeVec;
        for (auto& i : it->value.GetArray()) { tradeVec.emplace_back(i.GetObject()); }

        return tradeVec;
    };

    /**
     * @brief get the list of trades executed for a particular order.
     *
     * @param ordID order ID
     *
     *  @return std::vector<trade>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get order trades
     */
    std::vector<trade> orderTrades(const string& ordID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("order.trades"), "order_id"_a = ordID));

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (orderTrades())");
        };

        auto it = res.FindMember("data");
        if (!it->value.IsArray()) { throw libException("Array was expected (orderTrades())"); };

        std::vector<trade> tradeVec;
        for (auto& i : it->value.GetArray()) { tradeVec.emplace_back(i.GetObject()); }

        return tradeVec;
    };

    // gtt:

    /**
     * @brief place GTT order
     *
     * @param trigType trigger type
     * @param symbol trading symbol
     * @param exchange exchange name
     * @param trigValues trigger values
     * @param lastPrice last price
     * @param gttParams vector of GTTParams
     *
     * @return int trigger ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp placing a gtt
     */
    int placeGtt(const placeGttParams& params) {
        // make condition json
        rj::Document condition;
        condition.SetObject();
        auto& condnAlloc = condition.GetAllocator();

        rj::Value val; // used for making string values
        rj::Value trigValArr(rj::kArrayType);

        val.SetString(params.exchange.c_str(), params.exchange.size(), condnAlloc);
        condition.AddMember("exchange", val, condnAlloc);

        val.SetString(params.symbol.c_str(), params.symbol.size(), condnAlloc);
        condition.AddMember("tradingsymbol", val, condnAlloc);

        for (const double& i : params.triggerValues) { trigValArr.PushBack(i, condnAlloc); };
        condition.AddMember("trigger_values", trigValArr, condnAlloc);

        condition.AddMember("last_price", params.lastPrice, condnAlloc);

        // make orders json
        rj::Document reqParams;
        reqParams.SetArray();
        auto& paramsAlloc = reqParams.GetAllocator();

        for (const gttParams& param : params.gttParamsList) {
            rj::Value strVal;
            rj::Value tmpVal(rj::kObjectType);

            strVal.SetString(param.transactionType.c_str(), param.transactionType.size(), paramsAlloc);
            tmpVal.AddMember("transaction_type", strVal, paramsAlloc);

            tmpVal.AddMember("quantity", param.quantity, paramsAlloc);

            strVal.SetString(param.orderType.c_str(), param.orderType.size(), paramsAlloc);
            tmpVal.AddMember("order_type", strVal, paramsAlloc);

            strVal.SetString(param.product.c_str(), param.product.size(), paramsAlloc);
            tmpVal.AddMember("product", strVal, paramsAlloc);

            tmpVal.AddMember("price", param.price, paramsAlloc);

            strVal.SetString(params.exchange.c_str(), params.exchange.size(), paramsAlloc);
            tmpVal.AddMember("exchange", strVal, paramsAlloc);

            strVal.SetString(params.symbol.c_str(), params.symbol.size(), paramsAlloc);
            tmpVal.AddMember("tradingsymbol", strVal, paramsAlloc);

            reqParams.PushBack(tmpVal, paramsAlloc);
        };

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("gtt.place"),
            { { "type", params.triggerType }, { "condition", rju::_dump(condition) },
                { "orders", rju::_dump(reqParams) } });
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeGTT)"); };

        int rcvdTrigID = DEFAULTINT;
        rju::_getIfExists(res["data"].GetObject(), rcvdTrigID, "trigger_id");

        return rcvdTrigID;
    };

    /**
     * @brief get list of GTTs
     *
     * @return std::vector<GTT>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get gtts
     */
    std::vector<GTT> getGtts() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("gtt"));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getGTTs)"); };

        auto it = res.FindMember("data");
        if (!it->value.IsArray()) { throw libException("Array was expected (getGTTs())"); };

        std::vector<GTT> gttVec;
        for (auto& i : it->value.GetArray()) { gttVec.emplace_back(i.GetObject()); }

        return gttVec;
    };

    /**
     * @brief get details of a GTT
     *
     * @param trigID trigger ID
     * @return GTT
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get gtt info
     */
    GTT getGtt(int trigID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("gtt.info"), "trigger_id"_a = trigID));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getGTT)"); };

        return GTT(res["data"].GetObject());
    };

    /**
     * @brief
     *
     * @param trigID trigger ID
     * @param trigType trigger type
     * @param symbol trading symbol
     * @param exchange name of the exchange
     * @param trigValues trigger values
     * @param lastPrice last traded price of the instrument
     * @param gttParams vector of GTTParams
     *
     * @return int trigger ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp modifying a gtt
     */
    int modifyGtt(const kc::modifyGttParams& params) {
        // make condition json
        rj::Document condition;
        condition.SetObject();
        auto& condnAlloc = condition.GetAllocator();
        rj::Value val;
        rj::Value trigValArr(rj::kArrayType);

        val.SetString(params.exchange.c_str(), params.exchange.size(), condnAlloc);
        condition.AddMember("exchange", val, condnAlloc);

        val.SetString(params.symbol.c_str(), params.symbol.size(), condnAlloc);
        condition.AddMember("tradingsymbol", val, condnAlloc);

        for (const double& i : params.triggerValues) { trigValArr.PushBack(i, condnAlloc); };
        condition.AddMember("trigger_values", trigValArr, condnAlloc);

        condition.AddMember("last_price", params.lastPrice, condnAlloc);

        // make orders json
        rj::Document reqParams;
        reqParams.SetArray();
        auto& paramsAlloc = reqParams.GetAllocator();

        for (const gttParams& param : params.gttParamsList) {
            rj::Value strVal;
            rj::Value tmpVal(rj::kObjectType);

            strVal.SetString(param.transactionType.c_str(), param.transactionType.size(), paramsAlloc);
            tmpVal.AddMember("transaction_type", strVal, paramsAlloc);

            tmpVal.AddMember("quantity", param.quantity, paramsAlloc);

            strVal.SetString(param.orderType.c_str(), param.orderType.size(), paramsAlloc);
            tmpVal.AddMember("order_type", strVal, paramsAlloc);

            strVal.SetString(param.product.c_str(), param.product.size(), paramsAlloc);
            tmpVal.AddMember("product", strVal, paramsAlloc);

            tmpVal.AddMember("price", param.price, paramsAlloc);

            strVal.SetString(params.exchange.c_str(), params.exchange.size(), paramsAlloc);
            tmpVal.AddMember("exchange", strVal, paramsAlloc);

            strVal.SetString(params.symbol.c_str(), params.symbol.size(), paramsAlloc);
            tmpVal.AddMember("tradingsymbol", strVal, paramsAlloc);

            reqParams.PushBack(tmpVal, paramsAlloc);
        };

        rj::Document res;
        _sendReq(res, _methods::PUT, FMT(_endpoints.at("gtt.modify"), "trigger_id"_a = params.triggerId),
            { { "type", params.triggerType }, { "condition", rju::_dump(condition) },
                { "orders", rju::_dump(reqParams) } });
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (modifyGTT)"); };

        int rcvdTrigID = DEFAULTINT;
        rju::_getIfExists(res["data"].GetObject(), rcvdTrigID, "trigger_id");

        return rcvdTrigID;
    };

    /**
     * @brief delete a GTT order.
     *
     * @param trigID trigger ID
     *
     * @return int trigger ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp delete a gtt
     */
    int deleteGtt(int trigID) {

        rj::Document res;
        _sendReq(res, _methods::DEL, FMT(_endpoints.at("gtt.delete"), "trigger_id"_a = trigID));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (deleteGTT)"); };

        int rcvdTrigID = DEFAULTINT;
        rju::_getIfExists(res["data"].GetObject(), rcvdTrigID, "trigger_id");

        return rcvdTrigID;
    };

    // portfolio:

    /**
     * @brief get holdings
     *
     * @return std::vector<holding>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get holdings
     */
    std::vector<holding> holdings() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("portfolio.holdings"));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (holdings)"); };

        auto it = res.FindMember("data");
        if (!it->value.IsArray()) { throw libException("Array was expected (holdinga)"); };

        std::vector<holding> holdingsVec;
        for (auto& i : it->value.GetArray()) { holdingsVec.emplace_back(i.GetObject()); }

        return holdingsVec;
    };

    /**
     * @brief get positions
     *
     * @return positions
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get positions
     */
    positions getPositions() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("portfolio.positions"));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getPositions)"); };

        return positions(res["data"].GetObject());
    };

    /**
     * @brief Modify an open position's product type.
     *
     * @param exchange exchange name
     * @param symbol trading symbol
     * @param txnType transaction type
     * @param posType position type
     * @param quantity quantity to transact
     * @param oldProduct old product
     * @param newProduct new product
     *
     * @return bool true if position was successfully modified
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp convert position
     */
    bool convertPosition(const convertPositionParams& params) {

        std::vector<std::pair<string, string>> bodyParams = {
            { "exchange", params.exchange },
            { "tradingsymbol", params.symbol },
            { "transaction_type", params.transactionType },
            { "position_type", params.positionType },
            { "quantity", std::to_string(params.quantity) },
            { "old_product", params.oldProduct },
            { "new_product", params.newProduct },
        };

        rj::Document res;
        _sendReq(res, _methods::PUT, _endpoints.at("portfolio.positions.convert"), bodyParams);

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (convertPosition)");
        };

        auto it = res.FindMember("data");
        if (!it->value.IsBool()) {
            throw libException("data type wasn't the one expected. Expected bool (convertPosition)");
        };

        return it->value.GetBool();
    };

    // quotes and instruments:

    /**
     * @brief Retrieve the list of market instruments available to trade.
     *
     * @param exchange returns all instruments if none specified
     *
     * @return std::vector<instrument>
     *
     * @attention Note that the results could be large, with tens of thousands of entries.
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get instruments
     */
    std::vector<instrument> getInstruments(const string& exchange = "") {

        const string rcvdData =
            (exchange.empty()) ? _sendInstrumentsReq(_endpoints.at("market.instruments.all")) :
                                 _sendInstrumentsReq(FMT(_endpoints.at("market.instruments"), "exchange"_a = exchange));
        if (rcvdData.empty()) { return {}; };

        std::vector<string> tokens = kc::_split(rcvdData, '\n');
        tokens.pop_back(); // because last token is empty since empty tokens aren't ignored

        for (auto& tok : tokens) {
            // sent data has \r\n as delimiter and _split only removes \n. pop_back() to remove that \r
            tok.pop_back();
        };

        return { ++tokens.begin(), tokens.end() }; //++ to skip first iteration (headers)
    };

    /**
     * @brief Retrieve quote for list of instruments
     *
     * @param symbols vector of trading symbols in `exchange:tradingsymbol` (NSE:INFY) format
     *
     *  @return std::unordered_map<string, quote>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get quote
     */
    std::unordered_map<string, quote> getQuote(const std::vector<string>& symbols) {

        rj::Document res;
        _sendReq(
            res, _methods::GET, FMT(_endpoints.at("market.quote"), "symbols_list"_a = _encodeSymbolsList(symbols)));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getQuote)"); };

        std::unordered_map<string, quote> quoteMap;
        for (auto& i : res["data"].GetObject()) { quoteMap.emplace(i.name.GetString(), i.value.GetObject()); };

        return quoteMap;
    };

    /**
     * @brief Retrieve OHLC for list of instruments
     *
     * @param symbols vector of trading symbols in `exchange:tradingsymbol` (NSE:INFY) format
     *
     * @return std::unordered_map<string, OHLCQuote>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get ohlc
     */
    std::unordered_map<string, ohlcQuote> getOhlc(const std::vector<string>& symbols) {

        rj::Document res;
        _sendReq(res, _methods::GET,
            FMT(_endpoints.at("market.quote.ohlc"), "symbols_list"_a = _encodeSymbolsList(symbols)));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getOHLC)"); };

        std::unordered_map<string, ohlcQuote> quoteMap;
        for (auto& i : res["data"].GetObject()) { quoteMap.emplace(i.name.GetString(), i.value.GetObject()); };

        return quoteMap;
    };

    /**
     * @brief Retrieve last price for list of instruments
     *
     * @param symbols vector of trading symbols in `exchange:tradingsymbol` (NSE:INFY) format
     *
     * @return std::unordered_map<string, LTPQuote>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get ltp
     */
    std::unordered_map<string, ltpQuote> getLtp(const std::vector<string>& symbols) {

        rj::Document res;
        _sendReq(
            res, _methods::GET, FMT(_endpoints.at("market.quote.ltp"), "symbols_list"_a = _encodeSymbolsList(symbols)));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getLTP)"); };

        std::unordered_map<string, ltpQuote> quoteMap;
        for (auto& i : res["data"].GetObject()) { quoteMap.emplace(i.name.GetString(), i.value.GetObject()); };

        return quoteMap;
    };

    // historical:

    /**
     * @brief Retrieve historical data (candles) for an instrument
     *
     * @param instrumentTok instrument token (NOT trading symbol)
     * @param from from date in the following format: yyyy-mm-dd HH:MM:SS
     * @param to to date in the following format: yyyy-mm-dd HH:MM:SS
     * @param interval candle interval
     * @param continuous boolean flag to get continuous data for futures and options instruments
     * @param oi boolean flag to get open interest data
     *
     *  @return std::vector<historicalData>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get historical data
     */
    std::vector<historicalData> getHistoricalData(const historicalDataParams& params) {
        rj::Document res;
        _sendReq(res, _methods::GET,
            FMT(_endpoints.at("market.historical"), "instrument_token"_a = params.instrumentToken,
                "interval"_a = params.interval, "from"_a = params.from, "to"_a = params.to,
                "continuous"_a = static_cast<int>(params.continuous), "oi"_a = static_cast<int>(params.oi)));

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (getHistoricalData)");
        };

        auto it = res.FindMember("data")->value.FindMember("candles");
        if (!it->value.IsArray()) { throw libException("Array was expected (trades())"); };

        std::vector<historicalData> dataVec;
        for (auto& i : it->value.GetArray()) { dataVec.emplace_back(i.GetArray()); };

        return dataVec;
    };

    // MF:

    /**
     * @brief place a mutual fund order
     *
     * @param symbol trading symbol
     * @param txnType transaction type
     * @param quantity Quantity to SELL. Not applicable on BUYs. If the holding is less than
     * minimum_redemption_quantity, all the units have to be sold
     * @param amount amount worth of units to purchase. Not applicable on SELLs
     * @param tag an optional tag to apply to an order to identify it (alphanumeric, max 8 chars)
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp place mf order
     */
    string placeMfOrder(const placeMfOrderParams& params) {
        std::vector<std::pair<string, string>> bodyParams = {
            { "tradingsymbol", params.symbol },
            { "transaction_type", params.transactionType },
        };

        if (isValidArg(params.quantity)) { bodyParams.emplace_back("quantity", std::to_string(params.quantity)); }
        if (isValidArg(params.amount)) { bodyParams.emplace_back("amount", std::to_string(params.amount)); }
        if (isValidArg(params.tag)) { bodyParams.emplace_back("tag", params.tag); }

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("mf.order.place"), bodyParams);
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeMFOrder)"); };

        string rcvdOrdID;
        rju::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");
        return rcvdOrdID;
    };

    /**
     * @brief cancel a mutual fund order
     *
     * @param ordID order ID
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp cancel a mf order
     */
    string cancelMfOrder(const string& ordID) {

        rj::Document res;
        _sendReq(res, _methods::DEL, FMT(_endpoints.at("mf.order.cancel"), "order_id"_a = ordID));
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (cancelMFOrder)");
        };

        string rcvdOrdID;
        rju::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief get all mutual fund orders
     *
     * @return std::vector<MFOrder>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf orders
     */
    std::vector<mfOrder> getMfOrders() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("mf.orders"));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getMFOrders)"); };

        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getMFOrders"); };

        std::vector<mfOrder> ordersVec;
        for (auto& i : it->value.GetArray()) { ordersVec.emplace_back(i.GetObject()); }

        return ordersVec;
    };

    /**
     * @brief get details of a mutual fund order
     *
     * @param ordID order ID
     *
     * @return MFOrder
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf order info
     */
    mfOrder getMfOrder(const string& ordID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("mf.order.info"), "order_id"_a = ordID));
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (cancelMFOrder)");
        };

        return mfOrder(res["data"].GetObject());
    };

    /**
     * @brief get mutual fund holdings
     *
     * @return std::vector<MFHolding>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf holdings
     */
    std::vector<mfHolding> getMfHoldings() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("mf.holdings"));
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (getMFHoldings)");
        };

        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getMFHoldings"); };

        std::vector<mfHolding> holdingsVec;
        for (auto& i : it->value.GetArray()) { holdingsVec.emplace_back(i.GetObject()); }

        return holdingsVec;
    };

    // SIP:

    /**
     * @brief place MF SIP
     *
     * @param symbol tradingsymbol (ISIN) of the fund
     * @param amount amount worth of units to purchase. Not applicable on SELLs
     * @param installments number of instalments to trigger. If set to -1, instalments are triggered at fixed intervals
     * until the SIP is cancelled
     * @param freq frequency
     * @param initAmount initial amount
     * @param installDay installment day
     * @param tag an optional tag to apply to an order to identify it (alphanumeric, max 8 chars)
     *
     * @return std::pair<string, string>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp place mf sip order
     */
    std::pair<string, string> placeMfSip(const placeMfSipParams& params) {
        std::vector<std::pair<string, string>> bodyParams = {
            { "tradingsymbol", params.symbol },
            { "amount", std::to_string(params.amount) },
            { "instalments", std::to_string(params.installments) },
            { "frequency", params.frequency },
        };

        if (isValidArg(params.initialAmount)) {
            bodyParams.emplace_back("initial_amount", std::to_string(params.initialAmount));
        };
        if (isValidArg(params.installmentDay)) {
            bodyParams.emplace_back("instalment_day", std::to_string(params.installmentDay));
        };
        if (isValidArg(params.tag)) { bodyParams.emplace_back("tag", params.tag); };

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("mf.sip.place"), bodyParams);
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeMFSIP)"); };

        string rcvdOrdID, rcvdSipID;
        auto it = res.FindMember("data");
        rju::_getIfExists(it->value.GetObject(), rcvdOrdID, "order_id");
        rju::_getIfExists(it->value.GetObject(), rcvdSipID, "sip_id");
        return { rcvdOrdID, rcvdSipID };
    };

    /**
     * @brief modify a MF SIP order
     *
     * @param SIPID SIP ID
     * @param amount amount worth of units to purchase. It should be equal to or greated than
     * minimum_additional_purchase_amount and in multiple of purchase_amount_multiplier in the instrument master.
     * @param status pause or unpause an SIP (active or paused)
     * @param installments number of instalments to trigger. If set to -1, instalments are triggered idefinitely until
     * the SIP is cancelled
     * @param freq weekly, monthly, or quarterly
     * @param installDay installment day
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp modify mf sip order
     */
    void modifyMfSip(const modifyMfSipParams& params) {
        std::vector<std::pair<string, string>> bodyParams = {};

        if (isValidArg(params.amount)) { bodyParams.emplace_back("amount", std::to_string(params.amount)); }
        if (isValidArg(params.status)) { bodyParams.emplace_back("status", params.status); }
        if (isValidArg(params.installments)) {
            bodyParams.emplace_back("instalments", std::to_string(params.installments));
        }
        if (isValidArg(params.frequency)) { bodyParams.emplace_back("frequency", params.frequency); }
        if (isValidArg(params.installmentDay)) {
            bodyParams.emplace_back("instalment_day", std::to_string(params.installmentDay));
        }

        rj::Document res;
        _sendReq(res, _methods::PUT, FMT(_endpoints.at("mf.sip.modify"), "sip_id"_a = params.sipId), bodyParams);
    };

    /**
     * @brief cancel a MF SIP
     *
     * @param SIPID SIP ID
     *
     * @return SIP ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp cancel mf sip
     */
    string cancelMfSip(const string& SIPID) {

        rj::Document res;
        _sendReq(res, _methods::DEL, FMT(_endpoints.at("mf.sip.cancel"), "sip_id"_a = SIPID));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeMFSIP)"); };

        string rcvdSipID;
        rju::_getIfExists(res["data"].GetObject(), rcvdSipID, "sip_id");

        return rcvdSipID;
    };

    /**
     * @brief get list of SIPs
     *
     * @return std::vector<MFSIP>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get sips
     */
    std::vector<mfSip> getSips() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("mf.sips"));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getSIPs)"); };

        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getSIPs)"); };

        std::vector<mfSip> sipVec;
        for (auto& i : it->value.GetArray()) { sipVec.emplace_back(i.GetObject()); }

        return sipVec;
    };

    /**
     * @brief get details of a SIP
     *
     * @param SIPID SIP ID
     *
     * @return MFSIP
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get sip info
     */
    mfSip getSip(const string& SIPID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("mf.sip.info"), "sip_id"_a = SIPID));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getSIP)"); };

        return mfSip(res["data"].GetObject());
    };

    /**
     * @brief Get list of mutual fund instruments
     *
     * @return std::vector<MFInstrument>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf instruments
     */
    std::vector<mfInstrument> getMfInstruments() {

        const string rcvdData = _sendInstrumentsReq(_endpoints.at("mf.instruments"));

        if (rcvdData.empty()) { return {}; };
        std::vector<string> tokens = kc::_split(rcvdData, '\n');
        tokens.pop_back(); // because last token is empty since empty tokens aren't ignored

        for (auto& tok : tokens) {
            // sent data has \r\n as delimiter and _split only removes \n. pop_back() to remove that \r
            tok.pop_back();
        };

        return { ++tokens.begin(), tokens.end() }; //++ to skip first iteration (headers)
    };

    // others:

    /**
     * @brief get various margins required for orders
     *
     * @param params
     *
     * @return std::vector<orderMargins>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get order margins
     */
    std::vector<orderMargins> getOrderMargins(const std::vector<orderMarginsParams>& params) {

        rj::Document req;
        req.SetArray();
        auto& reqAlloc = req.GetAllocator();

        for (const auto& param : params) {
            rj::Value paramVal(rj::kObjectType);
            rj::Value strVal; // used for making string values

            strVal.SetString(param.exchange.c_str(), param.exchange.size(), reqAlloc);
            paramVal.AddMember("exchange", strVal, reqAlloc);

            strVal.SetString(param.tradingsymbol.c_str(), param.tradingsymbol.size(), reqAlloc);
            paramVal.AddMember("tradingsymbol", strVal, reqAlloc);

            strVal.SetString(param.transactionType.c_str(), param.transactionType.size(), reqAlloc);
            paramVal.AddMember("transaction_type", strVal, reqAlloc);

            strVal.SetString(param.variety.c_str(), param.variety.size(), reqAlloc);
            paramVal.AddMember("variety", strVal, reqAlloc);

            strVal.SetString(param.product.c_str(), param.product.size(), reqAlloc);
            paramVal.AddMember("product", strVal, reqAlloc);

            strVal.SetString(param.orderType.c_str(), param.orderType.size(), reqAlloc);
            paramVal.AddMember("order_type", strVal, reqAlloc);

            paramVal.AddMember("quantity", param.quantity, reqAlloc);
            paramVal.AddMember("price", param.price, reqAlloc);
            paramVal.AddMember("trigger_price", param.triggerPrice, reqAlloc);

            req.PushBack(paramVal, reqAlloc);
        };

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("order.margins"), { { "", rju::_dump(req) } }, true);
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (getOrderMargis)");
        };

        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getOrderMargis)"); };

        std::vector<orderMargins> marginsVec;
        for (auto& i : it->value.GetArray()) { marginsVec.emplace_back(i.GetObject()); }

        return marginsVec;
    };

  private:
    // member variables:

    string _apiKey = "";
    string _accessToken = "";

    const string _kiteVersion = "3";
    const string _rootURL = "https://api.kite.trade";
    const string _loginURLFmt = "https://kite.zerodha.com/connect/login?v=3&api_key={api_key}";
    const std::unordered_map<string, string> _endpoints = {
        // api
        { "api.token", "/session/token" },
        { "api.token.invalidate", "/session/token?api_key={api_key}&access_token={access_token}" },
        // x{"api.token.renew", "/session/refresh_token"},

        // user
        { "user.profile", "/user/profile" },
        { "user.margins", "/user/margins" },
        { "user.margins.segment", "/user/margins/{segment}" },

        // orders
        { "orders", "/orders" },
        { "trades", "/trades" },

        { "order.info", "/orders/{order_id}" },
        { "order.place", "/orders/{variety}" },
        { "order.modify", "/orders/{variety}/{order_id}" },
        { "order.cancel", "/orders/{variety}/{order_id}" },
        { "order.cancel.bo", "/orders/{variety}/{order_id}?parent_order_id={parent_order_id}" },
        { "order.trades", "/orders/{order_id}/trades" },

        // portfolio
        { "portfolio.positions", "/portfolio/positions" },
        { "portfolio.holdings", "/portfolio/holdings" },
        { "portfolio.positions.convert", "/portfolio/positions" },

        // MF api endpoints
        { "mf.orders", "/mf/orders" },
        { "mf.order.info", "/mf/orders/{order_id}" },
        { "mf.order.place", "/mf/orders" },
        { "mf.order.cancel", "/mf/orders/{order_id}" },

        { "mf.sips", "/mf/sips" },
        { "mf.sip.info", "/mf/sips/{sip_id}" },
        { "mf.sip.place", "/mf/sips" },
        { "mf.sip.modify", "/mf/sips/{sip_id}" },
        { "mf.sip.cancel", "/mf/sips/{sip_id}" },

        { "mf.holdings", "/mf/holdings" },
        { "mf.instruments", "/mf/instruments" },

        // market endpoints
        { "market.instruments.all", "/instruments" },
        { "market.instruments", "/instruments/{exchange}" },
        { "market.margins", "/margins/{segment}" },
        { "market.historical", "/instruments/historical/{instrument_token}/"
                               "{interval}?from={from}&to={to}&continuous={continuous}&oi={oi}" },
        { "market.trigger_range", "/instruments/trigger_range/{transaction_type}" },

        // x{"market.quote", "/quote"},
        // x{"market.quote.ohlc", "/quote/ohlc"},
        // x{"market.quote.ltp", "/quote/ltp"},

        { "market.quote", "/quote?{symbols_list}" },
        { "market.quote.ohlc", "/quote/ohlc?{symbols_list}" },
        { "market.quote.ltp", "/quote/ltp?{symbols_list}" },

        // GTT endpoints
        { "gtt", "/gtt/triggers" },
        { "gtt.place", "/gtt/triggers" },
        { "gtt.info", "/gtt/triggers/{trigger_id}" },
        { "gtt.modify", "/gtt/triggers/{trigger_id}" },
        { "gtt.delete", "/gtt/triggers/{trigger_id}" },

        // Margin computation endpoints
        { "order.margins", "/margins/orders" },
    };

    httplib::Client _httpClient;

    // methods:

    string _getAuthStr() const { return FMT("token {0}:{1}", _apiKey, _accessToken); };

    static string _encodeSymbolsList(const std::vector<string>& symbols) {

        string str;

        for (const auto& symbol : symbols) {
            //! could cause problems because there will be extra `&` after last query. can be solved by scraping the
            //! last char of string after the for loop, if necessary
            str.append(FMT("i={0}&", symbol));
        };

        return str;
    }

    // GMock requires mock methods to be virtual
    virtual void _sendReq(rj::Document& data, const _methods& mtd, const string& endpoint,
        const std::vector<std::pair<string, string>>& bodyParams = {}, bool isJson = false) {

        /*
        If the endpoint expects pure JSON body, set isJson to true and put the json body in second element of
        bodyParam's first pair with first element being empty string. see orderMargins() function
        */

        // create request
        const httplib::Headers headers = { { "Authorization", _getAuthStr() }, { "X-Kite-Version", _kiteVersion } };
        httplib::Params params;
        int code = 0;
        string dataRcvd;

        // this code can obviously be shortened but return type of client.Get() etc. doesn't have a default constructor,
        // which means we cannot init an instance and assign to it later
        if (mtd == _methods::GET) {
            if (auto res = _httpClient.Get(endpoint.c_str(), headers)) {
                code = res->status;
                dataRcvd = res->body;
            } else {
                throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
            };
        } else if (mtd == _methods::POST) {
            if (!isJson) {
                for (const auto& i : bodyParams) { params.emplace(i.first, i.second); };
                if (auto res = _httpClient.Post(endpoint.c_str(), headers, params)) {
                    code = res->status;
                    dataRcvd = res->body;
                } else {
                    throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
                };
            } else {
                if (auto res = _httpClient.Post(endpoint.c_str(), headers, bodyParams[0].second, "application/json")) {
                    code = res->status;
                    dataRcvd = res->body;
                } else {
                    throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
                };
            };
        } else if (mtd == _methods::PUT) {
            if (!isJson) {
                for (const auto& i : bodyParams) { params.emplace(i.first, i.second); };

                if (auto res = _httpClient.Put(endpoint.c_str(), headers, params)) {
                    code = res->status;
                    dataRcvd = res->body;
                } else {
                    throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
                };
            } else {
                if (auto res = _httpClient.Put(endpoint.c_str(), headers, bodyParams[0].second, "application/json")) {
                    code = res->status;
                    dataRcvd = res->body;
                } else {
                    throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
                };
            };

        } else if (mtd == _methods::DEL) {
            if (auto res = _httpClient.Delete(endpoint.c_str(), headers)) {
                code = res->status;
                dataRcvd = res->body;
            } else {
                throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
            };
        };

        //?std::cout << dataRcvd << std::endl;

        if (!dataRcvd.empty()) {
            rju::_parse(data, dataRcvd);
            if (code != 200) {
                string excpStr;
                string message;

                try {
                    if (!rju::_getIfExists(data, excpStr, "error_type")) { excpStr = "NoException"; };
                    rju::_getIfExists(data, message, "message");

                } catch (const std::exception& e) {
                    throw libException(FMT("{0} was thrown while extracting excpStr({1}) and message({2}) (_sendReq)",
                        e.what(), excpStr, message));
                };

                kc::_throwException(excpStr, code, message);
            };
        } else {
            // sets the document to a non-object entity on failure. array was chosen because no kite method returns
            // `data` field with an array
            data.Parse("[]");
        };
    };

    // GMock requires mock methods to be virtual
    virtual string _sendInstrumentsReq(const string& endpoint) {

        // create request and send req
        httplib::Headers headers = { { "Authorization", _getAuthStr() }, { "X-Kite-Version", _kiteVersion } };
        int code = 0;
        string dataRcvd;

        if (auto res = _httpClient.Get(endpoint.c_str(), headers)) {
            code = res->status;
            if (code == 200) { dataRcvd = res->body; };
        } else {
            throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
        };

        // get data
        if (!dataRcvd.empty()) {
            if (code == 200) { return dataRcvd; }
            kc::_throwException("NoException", code, "");
        } else {
            return "";
        };

        return "";
    };

}; // namespace kitepp

} // namespace kiteconnect