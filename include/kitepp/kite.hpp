/*
 *   Copyright (c) 2020 Bhumit Attarde

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <algorithm> //for_each
#include <array>
#include <boost/token_functions.hpp>
#include <cmath>    //isnan()
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
#include "responses.hpp"
#include "rjutils.hpp"
#include "utils.hpp"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace rjh = kc::RJHelper;
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
                // TODO add unit test for checking hash
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
     * @return njson
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
     * @param variety
     * @param exchange
     * @param symbol trading symbol
     * @param txnType transaction type
     * @param quantity
     * @param product
     * @param orderType
     * @param price
     * @param validity
     * @param trigPrice trigger price
     * @param sqOff square off
     * @param SL stoploss
     * @param trailSL trailing stoploss
     * @param discQuantity disclosed quantity
     * @param tag
     *
     * @return string orderID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp placing an order
     */
    string placeOrder(const string& variety, const string& exchange, const string& symbol, const string& txnType,
        int quantity, const string& product, const string& orderType, double price = DEFAULTDOUBLE,
        const string& validity = "", double trigPrice = DEFAULTDOUBLE, double sqOff = DEFAULTDOUBLE,
        double SL = DEFAULTDOUBLE, double trailSL = DEFAULTDOUBLE, int discQuantity = DEFAULTINT,
        const string& tag = "") {

        std::vector<std::pair<string, string>> bodyParams = {

            { "exchange", exchange },
            { "tradingsymbol", symbol },
            { "transaction_type", txnType },
            { "quantity", std::to_string(quantity) },
            { "product", product },
            { "order_type", orderType },

        };

        if (!std::isnan(price)) { bodyParams.emplace_back("price", std::to_string(price)); }
        if (!validity.empty()) { bodyParams.emplace_back("validity", validity); }
        if (!std::isnan(discQuantity)) { bodyParams.emplace_back("disclosed_quantity", std::to_string(discQuantity)); }
        if (!std::isnan(trigPrice)) { bodyParams.emplace_back("trigger_price", std::to_string(trigPrice)); }
        if (!std::isnan(sqOff)) { bodyParams.emplace_back("squareoff", std::to_string(sqOff)); }
        if (!std::isnan(SL)) { bodyParams.emplace_back("stoploss", std::to_string(SL)); }
        if (!std::isnan(trailSL)) { bodyParams.emplace_back("trailing_stoploss", std::to_string(trailSL)); }
        if (!tag.empty()) { bodyParams.emplace_back("tag", tag); }

        rj::Document res;
        _sendReq(res, _methods::POST, FMT(_endpoints.at("order.place"), "variety"_a = variety), bodyParams);

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeOrder)"); };

        string rcvdOrdID;
        rjh::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief modify an order
     *
     * @param variety
     * @param ordID order ID
     * @param parentOrdID parent order ID
     * @param quantity
     * @param price
     * @param ordType order type
     * @param trigPrice trigger price
     * @param validity
     * @param discQuantity disclosed quantity
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp modifying an order
     */
    string modifyOrder(const string& variety, const string& ordID, const string& parentOrdID = "",
        int quantity = DEFAULTINT, double price = DEFAULTDOUBLE, const string& ordType = "",
        double trigPrice = DEFAULTDOUBLE, const string& validity = "", int discQuantity = DEFAULTINT) {

        std::vector<std::pair<string, string>> bodyParams = {};

        if (!parentOrdID.empty()) { bodyParams.emplace_back("parent_order_id", parentOrdID); }
        if (!std::isnan(quantity)) { bodyParams.emplace_back("quantity", std::to_string(quantity)); }
        if (!std::isnan(price)) { bodyParams.emplace_back("price", std::to_string(price)); }
        if (!ordType.empty()) { bodyParams.emplace_back("order_type", ordType); }
        if (!std::isnan(trigPrice)) { bodyParams.emplace_back("trigger_price", std::to_string(trigPrice)); }
        if (!validity.empty()) { bodyParams.emplace_back("validity", validity); }
        if (!std::isnan(discQuantity)) { bodyParams.emplace_back("disclosed_quantity", std::to_string(discQuantity)); }

        rj::Document res;
        _sendReq(res, _methods::PUT, FMT(_endpoints.at("order.modify"), "variety"_a = variety, "order_id"_a = ordID),
            bodyParams);

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (modifyOrder)"); };

        string rcvdOrdID;
        rjh::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief cancel an order
     *
     * @param variety
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
        rjh::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

        return rcvdOrdID;
    };

    /**
     * @brief exit an order
     *
     * @param variety
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
        if (!res["data"].IsArray()) { throw libException("Array was expected (orders())"); };

        std::vector<order> orderVec;
        for (auto& i : res["data"].GetArray()) { orderVec.emplace_back(i.GetObject()); }

        return orderVec;
    };

    /**
     * @brief get history of an order
     *
     * @param ordID order ID
     *
     *  @return std::vector<order>
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

        if (!res["data"].IsArray()) { throw libException("Array was expected (orderHistory())"); };

        std::vector<order> orderVec;
        for (auto& i : res["data"].GetArray()) { orderVec.emplace_back(i.GetObject()); }

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
        if (!res["data"].IsArray()) { throw libException("Array was expected (trades())"); };

        std::vector<trade> tradeVec;
        for (auto& i : res["data"].GetArray()) { tradeVec.emplace_back(i.GetObject()); }

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
        if (!res["data"].IsArray()) { throw libException("Array was expected (orderTrades())"); };

        std::vector<trade> tradeVec;
        for (auto& i : res["data"].GetArray()) { tradeVec.emplace_back(i.GetObject()); }

        return tradeVec;
    };

    // gtt:

    /**
     * @brief place GTT order
     *
     * @param trigType trigger type
     * @param symbol trading symbol
     * @param exchange
     * @param trigValues trigger values
     * @param lastPrice last price
     * @param gttParams vector of GTTParams
     *
     * @return int trigger ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp placing a gtt
     */
    int placeGTT(const string& trigType, const string& symbol, const string& exchange,
        const std::vector<double>& trigValues, double lastPrice, const std::vector<GTTParams>& gttParams) {

        // make condition json
        rj::Document condition;
        condition.SetObject();
        auto& condnAlloc = condition.GetAllocator();

        rj::Value val; // used for making string values
        rj::Value trigValArr(rj::kArrayType);

        val.SetString(exchange.c_str(), exchange.size(), condnAlloc);
        condition.AddMember("exchange", val, condnAlloc);

        val.SetString(symbol.c_str(), symbol.size(), condnAlloc);
        condition.AddMember("tradingsymbol", val, condnAlloc);

        for (const double& i : trigValues) { trigValArr.PushBack(i, condnAlloc); };
        condition.AddMember("trigger_values", trigValArr, condnAlloc);

        condition.AddMember("last_price", lastPrice, condnAlloc);

        // make orders json
        rj::Document params;
        params.SetArray();
        auto& paramsAlloc = params.GetAllocator();

        for (const GTTParams& param : gttParams) {

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

            strVal.SetString(exchange.c_str(), exchange.size(), paramsAlloc);
            tmpVal.AddMember("exchange", strVal, paramsAlloc);

            strVal.SetString(symbol.c_str(), symbol.size(), paramsAlloc);
            tmpVal.AddMember("tradingsymbol", strVal, paramsAlloc);

            params.PushBack(tmpVal, paramsAlloc);
        };

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("gtt.place"),
            {

                { "type", trigType }, { "condition", rjh::_dump(condition) }, { "orders", rjh::_dump(params) }

            });

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeGTT)"); };

        int rcvdTrigID = DEFAULTINT;
        rjh::_getIfExists(res["data"].GetObject(), rcvdTrigID, "trigger_id");

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
    std::vector<GTT> getGTTs() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("gtt"));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getGTTs)"); };
        if (!res["data"].IsArray()) { throw libException("Array was expected (getGTTs())"); };

        std::vector<GTT> gttVec;
        for (auto& i : res["data"].GetArray()) { gttVec.emplace_back(i.GetObject()); }

        return gttVec;
    };

    /**
     * @brief get details of a GTT
     *
     * @param trigID
     * @return GTT
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get gtt info
     */
    GTT getGTT(int trigID) {

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
     * @param exchange
     * @param trigValues trigger values
     * @param lastPrice last traded price of the instrument
     * @param gttParams vector of GTTParams
     *
     * @return int trigger ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp modifying a gtt
     */
    int modifyGTT(int trigID, const string& trigType, const string& symbol, const string& exchange,
        const std::vector<double>& trigValues, double lastPrice, const std::vector<GTTParams>& gttParams) {

        // make condition json
        rj::Document condition;
        condition.SetObject();
        auto& condnAlloc = condition.GetAllocator();
        rj::Value val;
        rj::Value trigValArr(rj::kArrayType);

        val.SetString(exchange.c_str(), exchange.size(), condnAlloc);
        condition.AddMember("exchange", val, condnAlloc);

        val.SetString(symbol.c_str(), symbol.size(), condnAlloc);
        condition.AddMember("tradingsymbol", val, condnAlloc);

        for (const double& i : trigValues) { trigValArr.PushBack(i, condnAlloc); };
        condition.AddMember("trigger_values", trigValArr, condnAlloc);

        condition.AddMember("last_price", lastPrice, condnAlloc);

        // make orders json
        rj::Document params;
        params.SetArray();
        auto& paramsAlloc = params.GetAllocator();

        for (const GTTParams& param : gttParams) {

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

            strVal.SetString(exchange.c_str(), exchange.size(), paramsAlloc);
            tmpVal.AddMember("exchange", strVal, paramsAlloc);

            strVal.SetString(symbol.c_str(), symbol.size(), paramsAlloc);
            tmpVal.AddMember("tradingsymbol", strVal, paramsAlloc);

            params.PushBack(tmpVal, paramsAlloc);
        };

        rj::Document res;
        _sendReq(res, _methods::PUT, FMT(_endpoints.at("gtt.modify"), "trigger_id"_a = trigID),
            {

                { "type", trigType }, { "condition", rjh::_dump(condition) }, { "orders", rjh::_dump(params) }

            });

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (modifyGTT)"); };

        int rcvdTrigID = DEFAULTINT;
        rjh::_getIfExists(res["data"].GetObject(), rcvdTrigID, "trigger_id");

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
    int deleteGTT(int trigID) {

        rj::Document res;
        _sendReq(res, _methods::DEL, FMT(_endpoints.at("gtt.delete"), "trigger_id"_a = trigID));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (deleteGTT)"); };

        int rcvdTrigID = DEFAULTINT;
        rjh::_getIfExists(res["data"].GetObject(), rcvdTrigID, "trigger_id");

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
        if (!res["data"].IsArray()) { throw libException("Array was expected (holdinga)"); };

        std::vector<holding> holdingsVec;
        for (auto& i : res["data"].GetArray()) { holdingsVec.emplace_back(i.GetObject()); }

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
     * @param exchange
     * @param symbol
     * @param txnType transaction type
     * @param posType position type
     * @param quantity
     * @param oldProduct old product
     * @param newProduct new product
     *
     * @return bool true if position was successfully modified
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp convert position
     */
    bool convertPosition(const string& exchange, const string& symbol, const string& txnType, const string& posType,
        int quantity, const string& oldProduct, const string& newProduct) {

        std::vector<std::pair<string, string>> bodyParams = {

            { "exchange", exchange },
            { "tradingsymbol", symbol },
            { "transaction_type", txnType },
            { "position_type", posType },
            { "quantity", std::to_string(quantity) },
            { "old_product", oldProduct },
            { "new_product", newProduct },

        };

        rj::Document res;
        _sendReq(res, _methods::PUT, _endpoints.at("portfolio.positions.convert"), bodyParams);
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (convertPosition)");
        };
        if (!res["data"].IsBool()) {
            throw libException("data type wasn't the one expected. Expected bool (convertPosition)");
        };

        return res["data"].GetBool();
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
    std::unordered_map<string, OHLCQuote> getOHLC(const std::vector<string>& symbols) {

        rj::Document res;
        _sendReq(res, _methods::GET,
            FMT(_endpoints.at("market.quote.ohlc"), "symbols_list"_a = _encodeSymbolsList(symbols)));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getOHLC)"); };

        std::unordered_map<string, OHLCQuote> quoteMap;
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
    std::unordered_map<string, LTPQuote> getLTP(const std::vector<string>& symbols) {

        rj::Document res;
        _sendReq(
            res, _methods::GET, FMT(_endpoints.at("market.quote.ltp"), "symbols_list"_a = _encodeSymbolsList(symbols)));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getLTP)"); };

        std::unordered_map<string, LTPQuote> quoteMap;
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
    std::vector<historicalData> getHistoricalData(int instrumentTok, const string& from, const string& to,
        const string& interval, bool continuous = false, bool oi = false) {

        rj::Document res;
        _sendReq(res, _methods::GET,
            FMT(_endpoints.at("market.historical"), "instrument_token"_a = instrumentTok, "interval"_a = interval,
                "from"_a = from, "to"_a = to, "continuous"_a = static_cast<int>(continuous),
                "oi"_a = static_cast<int>(oi)));
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (getHistoricalData)");
        };
        if (!res["data"]["candles"].IsArray()) { throw libException("Array was expected (trades())"); };

        std::vector<historicalData> dataVec;
        for (auto& i : res["data"]["candles"].GetArray()) { dataVec.emplace_back(i.GetArray()); };

        return dataVec;
    };

    // MF:

    /**
     * @brief place a mutual fund order
     *
     * @param symbol
     * @param txnType transaction type
     * @param quantity
     * @param amount
     * @param tag
     *
     * @return string order ID
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp place mf order
     */
    string placeMFOrder(const string& symbol, const string& txnType, int quantity = DEFAULTINT,
        double amount = DEFAULTDOUBLE, const string& tag = "") {

        std::vector<std::pair<string, string>> bodyParams = {

            { "tradingsymbol", symbol },
            { "transaction_type", txnType },

        };

        if (!std::isnan(quantity)) { bodyParams.emplace_back("quantity", std::to_string(quantity)); }
        if (!std::isnan(amount)) { bodyParams.emplace_back("amount", std::to_string(amount)); }
        if (!tag.empty()) { bodyParams.emplace_back("tag", tag); }

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("mf.order.place"), bodyParams);
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeMFOrder)"); };

        string rcvdOrdID;
        rjh::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

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
    string cancelMFOrder(const string& ordID) {

        rj::Document res;
        _sendReq(res, _methods::DEL, FMT(_endpoints.at("mf.order.cancel"), "order_id"_a = ordID));
        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (cancelMFOrder)");
        };

        string rcvdOrdID;
        rjh::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");

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
    std::vector<MFOrder> getMFOrders() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("mf.orders"));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getMFOrders)"); };
        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getMFOrders"); };

        std::vector<MFOrder> ordersVec;
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
    MFOrder getMFOrder(const string& ordID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("mf.order.info"), "order_id"_a = ordID));

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (cancelMFOrder)");
        };

        return MFOrder(res["data"].GetObject());
    };

    /**
     * @brief get mutual fund holdings
     *
     * @return njson
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf holdings
     */
    std::vector<MFHolding> getMFHoldings() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("mf.holdings"));

        if (!res.IsObject()) {
            throw libException("Empty data was received where it wasn't expected (getMFHoldings)");
        };

        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getMFHoldings"); };

        std::vector<MFHolding> holdingsVec;
        for (auto& i : it->value.GetArray()) { holdingsVec.emplace_back(i.GetObject()); }

        return holdingsVec;
    };

    // SIP:

    /**
     * @brief place MF SIP
     *
     * @param symbol
     * @param amount
     * @param installments
     * @param freq frequency
     * @param initAmount initial amount
     * @param installDay installment day
     * @param tag
     *
     * @return std::pair<string, string>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp place mf sip order
     */
    std::pair<string, string> placeMFSIP(const string& symbol, double amount, int installments, const string& freq,
        double initAmount = DEFAULTDOUBLE, int installDay = DEFAULTINT, const string& tag = "") {

        std::vector<std::pair<string, string>> bodyParams = {

            { "tradingsymbol", symbol },
            { "amount", std::to_string(amount) },
            { "instalments", std::to_string(installments) },
            { "frequency", freq },

        };

        if (!std::isnan(initAmount)) { bodyParams.emplace_back("initial_amount", std::to_string(initAmount)); };
        if (!std::isnan(installDay)) { bodyParams.emplace_back("instalment_day", std::to_string(installDay)); };
        if (!tag.empty()) { bodyParams.emplace_back("tag", tag); };

        rj::Document res;
        _sendReq(res, _methods::POST, _endpoints.at("mf.sip.place"), bodyParams);
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeMFSIP)"); };

        string rcvdOrdID, rcvdSipID;
        rjh::_getIfExists(res["data"].GetObject(), rcvdOrdID, "order_id");
        rjh::_getIfExists(res["data"].GetObject(), rcvdSipID, "sip_id");

        return { rcvdOrdID, rcvdSipID };
    };

    /**
     * @brief modify a MF SIP order
     *
     * @param SIPID SIP ID
     * @param amount
     * @param status
     * @param installments
     * @param freq frequency
     * @param installDay installment day
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp modify mf sip order
     */
    void modifyMFSIP(const string& SIPID, double amount = DEFAULTDOUBLE, const string& status = "",
        int installments = DEFAULTINT, const string& freq = "", int installDay = DEFAULTINT) {

        std::vector<std::pair<string, string>> bodyParams = {};

        if (!std::isnan(amount)) { bodyParams.emplace_back("amount", std::to_string(amount)); }
        if (!status.empty()) { bodyParams.emplace_back("status", status); }
        if (!std::isnan(installments)) { bodyParams.emplace_back("instalments", std::to_string(installments)); }
        if (!freq.empty()) { bodyParams.emplace_back("frequency", freq); }
        if (!std::isnan(installDay)) { bodyParams.emplace_back("instalment_day", std::to_string(installDay)); }

        rj::Document res;
        _sendReq(res, _methods::PUT, FMT(_endpoints.at("mf.sip.modify"), "sip_id"_a = SIPID), bodyParams);
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
    string cancelMFSIP(const string& SIPID) {

        rj::Document res;
        _sendReq(res, _methods::DEL, FMT(_endpoints.at("mf.sip.cancel"), "sip_id"_a = SIPID));
        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (placeMFSIP)"); };

        string rcvdSipID;
        rjh::_getIfExists(res["data"].GetObject(), rcvdSipID, "sip_id");

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
    std::vector<MFSIP> getSIPs() {

        rj::Document res;
        _sendReq(res, _methods::GET, _endpoints.at("mf.sips"));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getSIPs)"); };
        auto it = res.FindMember("data");
        if (!(it->value.IsArray())) { throw libException("Array was expected (getSIPs)"); };

        std::vector<MFSIP> sipVec;
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
    MFSIP getSIP(const string& SIPID) {

        rj::Document res;
        _sendReq(res, _methods::GET, FMT(_endpoints.at("mf.sip.info"), "sip_id"_a = SIPID));

        if (!res.IsObject()) { throw libException("Empty data was received where it wasn't expected (getSIP)"); };

        return MFSIP(res["data"].GetObject());
    };

    /**
     * @brief Get list of mutual fund instruments
     *
     * @return std::vector<MFInstrument>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf instruments
     */
    std::vector<MFInstrument> getMFInstruments() {

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
        _sendReq(res, _methods::POST, _endpoints.at("order.margins"), { { "", rjh::_dump(req) } }, true);
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

            //! could cause problems because there will that `&` after last query. can be solved by scraping the last
            //! char of string after the for loop
            str.append(FMT("i={0}&", symbol));
        };

        return str;
    }

    static string _encodeBody(const std::vector<std::pair<string, string>>& params) {

        // encodes body in `urlencoded` form

        string str;

        for (const auto& param : params) {

            //! could cause problems because there will that `&` after last query. can be solved by scraping the last
            //! char of string after the for loop
            str.append(FMT("{0}={1}&", param.first, param.second));
        };

        return str;
    };

    // GMock requires mock methods to be virtual
    virtual void _sendReq(rj::Document& data, const _methods& mtd, const string& endpoint,
        const std::vector<std::pair<string, string>>& bodyParams = {}, bool isJson = false) {

        /*
        If the endpoint expects pure JSON body, set isJson to true and put the json body in second element of
        bodyParam's first pair with first element being empty string. see orderMargins() function
        */

        // create request
        const httplib::Headers headers = {

            { "Authorization", _getAuthStr() }, { "X-Kite-Version", _kiteVersion }
        };
        int code = 0;
        string dataRcvd;

        // this code can obviously be shortened but return type of client.Get() etc. doesn't have a default constructor,
        // which means we cannot init an instance and then assign to it later
        if (mtd == _methods::GET) {

            if (auto res = _httpClient.Get(endpoint.c_str(), headers)) {

                code = res->status;
                dataRcvd = res->body;
            } else {

                throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
            };
        } else if (mtd == _methods::POST) {

            if (auto res = _httpClient.Post(endpoint.c_str(), headers,
                    (isJson) ? bodyParams[0].second : _encodeBody(bodyParams),
                    (isJson) ? "application/json" : "application/x-www-form-urlencoded")) {

                code = res->status;
                dataRcvd = res->body;
            } else {

                throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
            };
        } else if (mtd == _methods::PUT) {

            if (auto res = _httpClient.Put(endpoint.c_str(), headers,
                    (isJson) ? bodyParams[0].second : _encodeBody(bodyParams),
                    (isJson) ? "application/json" : "application/x-www-form-urlencoded")) {

                code = res->status;
                dataRcvd = res->body;
            } else {

                throw libException(FMT("Failed to send http/https request (enum code: {0})", res.error()));
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

            rjh::_parse(data, dataRcvd);

            if (code != 200) {

                string excpStr;
                string message;

                try {

                    if (!rjh::_getIfExists(data, excpStr, "error_type")) { excpStr = "NoException"; };
                    rjh::_getIfExists(data, message, "message");

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

// TODO *rectify double lookup in functions

// TODO change namespace to kiteconnect
// TODO change RJHelper namespace to rjutil

// TODO fix build system
// TODO add copyight
// TODO write README & doxygen main (sep branch)
// TODO delete compile_comands.json & .vscode/ symlink from project directory
