#pragma once

#include <algorithm> //for_each
#include <array>
#include <cmath>    //isnan()
#include <iostream> //debug
#include <limits>   //nan
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility> //pair<>
#include <vector>

#include "PicoSHA2/picosha2.h"
#include "nlohmann/json.hpp"
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

#include "config.hpp"
#include "kiteppexceptions.hpp"


namespace kitepp {


namespace http = web::http;
using std::string;
using njson = nlohmann::json;
using kitepp::_throwException;


class kite {


  public:
    // member variables:


    // constructors and destructor:

    /**
     * @brief Construct a new kite object
     *
     * @param apikey API key
     *
     * @paragraph ex1 example
     * @snippet example2.cpp initializing kite
     */
    explicit kite(string apikey): _apiKey(std::move(apikey)), _httpClient(U(_rootURL)) {};


    // methods:

    // api:

    /**
     * @brief Get the remote login url to which a user should be redirected to initiate the login flow.
     *
     * @return string
     *
     * @paragraph ex1 example
     * @snippet example2.cpp obtaining login url
     */
    string loginURL() const { return FMT(_loginURLFmt, "api_key"_a = _apiKey); };

    /**
     * @brief Generate user session details like `access_token`
     *
     * @param requestToken request token obtained after from login flow
     * @param apiSecret API secret string obtained with API key
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp obtaining access token
     */
    njson generateSession(const string& requestToken, const string& apiSecret) {

        return _sendReq(http::methods::POST, _endpoints.at("api.token"),
            {

                { "api_key", _apiKey },
                { "request_token", requestToken },
                { "checksum", picosha2::hash256_hex_string(_apiKey + requestToken + apiSecret) },


            });
    };

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
     * @brief This call invalidates the access_token and destroys the API session. After this, the user should be sent through a new login flow before
     * further interactions. This does not log the user out of the official Kite web or mobile applications.
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp invalidate session
     */
    njson invalidateSession() {

        return _sendReq(http::methods::DEL, FMT(_endpoints.at("api.token.invalidate"), "api_key"_a = _apiKey, "access_token"_a = _accessToken));
    };

    // user:

    /**
     * @brief returns user profile
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get user profile
     */
    njson profile() { return _sendReq(http::methods::GET, _endpoints.at("user.profile")); };

    /**
     * @brief get account balance and cash margin details for a particular segment.
     *
     * @param segment Returns all segments if none specified.
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get margins
     */
    njson margins(const string& segment = "") {

        return (segment.empty()) ? _sendReq(http::methods::GET, _endpoints.at("user.margins")) :
                                   _sendReq(http::methods::GET, FMT(_endpoints.at("user.margins.segment"), "segment"_a = segment));
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
     * @param orderType order type
     * @param price
     * @param validity
     * @param trigPrice trigger price
     * @param sqOff square off
     * @param SL stoploss
     * @param trailSL trailing stoploss
     * @param discQuantity discloed quantity
     * @param tag
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp placing an order
     */
    njson placeOrder(const string& variety, const string& exchange, const string& symbol, const string& txnType, const string& quantity,
        const string& product, const string& orderType, const string& price = "", const string& validity = "", const string& trigPrice = "",
        const string& sqOff = "", const string& SL = "", const string& trailSL = "", const string& discQuantity = "", const string& tag = "") {

        std::vector<std::pair<string, string>> bodyParams = {

            { "exchange", exchange },
            { "tradingsymbol", symbol },
            { "transaction_type", txnType },
            { "quantity", quantity },
            { "product", product },
            { "order_type", orderType },

        };

        if (!price.empty()) { bodyParams.emplace_back("price", price); }
        if (!validity.empty()) { bodyParams.emplace_back("validity", validity); }
        if (!discQuantity.empty()) { bodyParams.emplace_back("disclosed_quantity", discQuantity); }
        if (!trigPrice.empty()) { bodyParams.emplace_back("trigger_price", trigPrice); }
        if (!sqOff.empty()) { bodyParams.emplace_back("squareoff", sqOff); }
        if (!SL.empty()) { bodyParams.emplace_back("stoploss", SL); }
        if (!trailSL.empty()) { bodyParams.emplace_back("trailing_stoploss", trailSL); }
        if (!tag.empty()) { bodyParams.emplace_back("tag", tag); }

        return _sendReq(http::methods::POST, FMT(_endpoints.at("order.place"), "variety"_a = variety), bodyParams);
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
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp modifying an order
     */
    njson modifyOrder(const string& variety, const string& ordID, const string& parentOrdID = "", const string& quantity = "",
        const string& price = "", const string& ordType = "", const string& trigPrice = "", const string& validity = "",
        const string& discQuantity = "") {

        std::vector<std::pair<string, string>> bodyParams = {};

        if (!parentOrdID.empty()) { bodyParams.emplace_back("parent_order_id", parentOrdID); }
        if (!quantity.empty()) { bodyParams.emplace_back("quantity", quantity); }
        if (!price.empty()) { bodyParams.emplace_back("price", price); }
        if (!ordType.empty()) { bodyParams.emplace_back("order_type", ordType); }
        if (!trigPrice.empty()) { bodyParams.emplace_back("trigger_price", trigPrice); }
        if (!validity.empty()) { bodyParams.emplace_back("validity", validity); }
        if (!discQuantity.empty()) { bodyParams.emplace_back("disclosed_quantity", discQuantity); }

        return _sendReq(http::methods::PUT, FMT(_endpoints.at("order.modify"), "variety"_a = variety, "order_id"_a = ordID), bodyParams);
    };

    /**
     * @brief cancel an order
     *
     * @param variety
     * @param ordID order ID
     * @param parentOrdID parent order ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp cancelling an order
     */
    njson cancelOrder(const string& variety, const string& ordID, const string& parentOrdID = "") {

        return (variety == "bo") ? _sendReq(http::methods::DEL, FMT(_endpoints.at("order.cancel.bo"), "variety"_a = variety, "order_id"_a = ordID,
                                                                    "parent_order_id"_a = parentOrdID)) :
                                   _sendReq(http::methods::DEL, FMT(_endpoints.at("order.cancel"), "variety"_a = variety, "order_id"_a = ordID));
    };

    /**
     * @brief exit an order
     *
     * @param variety
     * @param ordID order ID
     * @param parentOrdID parent order ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp exiting an order
     */
    njson exitOrder(const string& variety, const string& ordID, const string& parentOrdID = "") { return cancelOrder(variety, ordID, parentOrdID); };

    /**
     * @brief get list of orders
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get orders
     */
    njson orders() { return _sendReq(http::methods::GET, _endpoints.at("orders")); };

    /**
     * @brief get history of an order
     *
     * @param ordID order ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get order history
     */
    njson orderHistory(const string& ordID) { return _sendReq(http::methods::GET, FMT(_endpoints.at("order.info"), "order_id"_a = ordID)); };

    /**
     * @brief get list of trades
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get trades
     */
    njson trades() { return _sendReq(http::methods::GET, _endpoints.at("trades")); };

    /**
     * @brief get the list of trades executed for a particular order.
     *
     * @param ordID order ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get order trades
     */
    njson orderTrades(const string& ordID) { return _sendReq(http::methods::GET, FMT(_endpoints.at("order.trades"), "order_id"_a = ordID)); };


    // gtt:

    /**
     * @brief place GTT order
     *
     * @param trigType trigger type
     * @param symbol trading symbol
     * @param exchange
     * @param trigValues trigger values
     * @param lastPrice last price
     * @param orders njson array of orders
     * @return njson
     *
     * @note
     * Function expects a njson array `orders` with following params: `transaction_type`, `quantity`, `order_type`, `product`, `price`. Users can
     * form the array like
     * @code
     * auto gttOrds = kitepp::njson::array();
     * gttOrds.push_back({
     *
     *    {"transaction_type", "BUY"},
     *    {"quantity", 10},
     *    {"order_type", "LIMIT"},
     *    {"product", "CNC"},
     *    {"price", 199.10},
     *
     *});
     * @endcode
     *and pass gttOrds to the function.
     *
     * @paragraph ex1 example
     * @snippet example2.cpp placing a gtt
     */
    njson placeGTT(const string& trigType, const string& symbol, const string& exchange, const std::vector<double>& trigValues,
        const string& lastPrice, njson& orders) {

        njson condition = {

            { "exchange", exchange },
            { "tradingsymbol", symbol },
            { "trigger_values", trigValues },
            { "last_price", std::stod(lastPrice) },

        };

        for (auto& order : orders) {

            order["exchange"] = exchange;
            order["tradingsymbol"] = symbol;
        };

        return _sendReq(http::methods::POST, _endpoints.at("gtt.place"),
            {

                { "type", trigType }, { "condition", condition.dump() }, { "orders", orders.dump() }

            });
    };

    /**
     * @brief get list of GTTs
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get gtts
     */
    njson getGTTs() { return _sendReq(http::methods::GET, _endpoints.at("gtt")); }

    /**
     * @brief get details of a GTT
     *
     * @param trigID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get gtt info
     */
    njson getGTT(string const& trigID) { return _sendReq(http::methods::GET, FMT(_endpoints.at("gtt.info"), "trigger_id"_a = trigID)); }

    /**
     * @brief modify a GTT
     *
     * @param trigID trigger ID
     * @param trigType trigger type
     * @param symbol trading symbol
     * @param exchange
     * @param trigValues trigger values
     * @param lastPrice last traded price of the instrument
     * @param orders njson array of orders
     * @return njson
     *
     * @note
     * Function expects a njson array `orders` with following params: `transaction_type`, `quantity`, `order_type`, `product`, `price`. Users can
     * form the array like
     *
     * @code
     * auto gttOrds = kitepp::njson::array();
     * gttOrds.push_back({
     *
     * {"transaction_type", "BUY"},
     * {"quantity", 10},
     * {"order_type", "LIMIT"},
     * {"product", "CNC"},
     * {"price", 199.10},
     *
     * });
     * @endcode
     * and pass gttOrds to the function.
     *
     * @paragraph ex1 example
     * @snippet example2.cpp modifying a gtt
     */
    njson modifyGTT(const string& trigID, const string& trigType, const string& symbol, const string& exchange, const std::vector<double>& trigValues,
        const string& lastPrice, njson& orders) {

        njson condition = {

            { "exchange", exchange },
            { "tradingsymbol", symbol },
            { "trigger_values", trigValues },
            { "last_price", std::stod(lastPrice) },

        };

        for (auto& order : orders) {

            order["exchange"] = exchange;
            order["tradingsymbol"] = symbol;
        };

        return _sendReq(http::methods::PUT, FMT(_endpoints.at("gtt.modify"), "trigger_id"_a = trigID),
            {

                { "type", trigType }, { "condition", condition.dump() }, { "orders", orders.dump() }

            });
    };

    /**
     * @brief delete a GTT order.
     *
     * @param trigID trigger ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp delete a gtt
     */
    njson deleteGTT(const string& trigID) { return _sendReq(http::methods::DEL, FMT(_endpoints.at("gtt.delete"), "trigger_id"_a = trigID)); }


    // portfolio:

    /**
     * @brief get holdings
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get holdings
     */
    njson holdings() { return _sendReq(http::methods::GET, _endpoints.at("portfolio.holdings")); };

    /**
     * @brief get positions
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get positions
     */
    njson positions() { return _sendReq(http::methods::GET, _endpoints.at("portfolio.positions")); };

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
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp convert position
     */
    njson convertPosition(const string& exchange, const string& symbol, const string& txnType, const string& posType, const string& quantity,
        const string& oldProduct, const string& newProduct) {


        std::vector<std::pair<string, string>> bodyParams = {

            { "exchange", exchange },
            { "tradingsymbol", symbol },
            { "transaction_type", txnType },
            { "position_type", posType },
            { "quantity", quantity },
            { "old_product", oldProduct },
            { "new_product", newProduct },

        };

        return _sendReq(http::methods::PUT, _endpoints.at("portfolio.positions.convert"), bodyParams);
    };


    // quotes and instruments:

    /**
     * @brief Retrieve the list of market instruments available to trade.
     *
     * @param exchange returns all instruments if none specified
     * @return string
     *
     * @attention Note that the results could be large, with tens of thousands of entries in the list.
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get instruments
     */
    string instruments(const string& exchange = "") {

        return (exchange.empty()) ? _sendInstrumentsReq(_endpoints.at("market.instruments.all")) :
                                    _sendInstrumentsReq(FMT(_endpoints.at("market.instruments"), "exchange"_a = exchange));
    };

    /**
     * @brief Retrieve quote for list of instruments
     *
     * @param symbols vector of trading symbols in `exchange:tradingsymbol` (NSE:INFY) format
     * @return njson
     *
     * @attention if there are spaces in symbol name, they should be replaced with `+`. Example: `NSE:NIFTY 50` becomes `NSE:NIFTY+50`
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get quote
     */
    njson quote(const std::vector<string>& symbols) {

        return _sendReq(http::methods::GET, FMT(_endpoints.at("market.quote"), "symbols_list"_a = _encodeSymbolsList(symbols)));
    };

    /**
     * @brief Retrieve OHLC and market depth for list of instruments
     *
     * @param symbols vector of trading symbols in `exchange:tradingsymbol` (NSE:INFY) format
     * @return njson
     *
     * @attention if there are spaces in symbol name, they should be replaced with `+`. Example: `NSE:NIFTY 50` becomes `NSE:NIFTY+50`
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get ohlc
     */
    njson ohlc(const std::vector<string>& symbols) {

        return _sendReq(http::methods::GET, FMT(_endpoints.at("market.quote.ohlc"), "symbols_list"_a = _encodeSymbolsList(symbols)));
    };

    /**
     * @brief Retrieve last price for list of instruments
     *
     * @param symbols vector of trading symbols in `exchange:tradingsymbol` (NSE:INFY) format
     * @return njson
     *
     * @attention if there are spaces in symbol name, they should be replaced with `+`. Example: `NSE:NIFTY 50` becomes `NSE:NIFTY+50`
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get ltp
     */
    njson ltp(const std::vector<string>& symbols) {

        return _sendReq(http::methods::GET, FMT(_endpoints.at("market.quote.ltp"), "symbols_list"_a = _encodeSymbolsList(symbols)));
    };


    // historical:

    /**
     * @brief Retrieve historical data (candles) for an instrument
     *
     * @param instrumentTok instrument token (NOT trading symbol)
     * @param from from date in following format: yyyy-mm-dd HH:MM:SS
     * @param to to date in following format: yyyy-mm-dd HH:MM:SS
     * @param interval candle interval
     * @param continuous boolean flag to get continuous data for futures and options instruments
     * @param oi boolean flag to get open interest data
     * @return njson
     *
     * @attention if there are spaces in symbol name, they should be replaced with `+`. Example: `NIFTY 50` becomes `NIFTY+50`
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get historical data
     */
    njson historicalData(
        const string& instrumentTok, const string& from, const string& to, const string& interval, bool continuous = false, bool oi = false) {

        return _sendReq(
            http::methods::GET, FMT(_endpoints.at("market.historical"), "instrument_token"_a = instrumentTok, "interval"_a = interval,
                                    "from"_a = from, "to"_a = to, "continuous"_a = static_cast<int>(continuous), "oi"_a = static_cast<int>(oi)));
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
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp place a mf order
     */
    njson placeMFOrder(const string& symbol, const string& txnType, const string& quantity = "", const string& amount = "", const string& tag = "") {

        std::vector<std::pair<string, string>> bodyParams = {

            { "tradingsymbol", symbol },
            { "transaction_type", txnType },

        };

        if (!quantity.empty()) { bodyParams.emplace_back("quantity", quantity); }
        if (!amount.empty()) { bodyParams.emplace_back("amount", amount); }
        if (!tag.empty()) { bodyParams.emplace_back("tag", tag); }

        return _sendReq(http::methods::POST, _endpoints.at("mf.order.place"), bodyParams);
    };

    /**
     * @brief cancel a mutual fund order
     *
     * @param ordID order ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp cancel a mf order
     */
    njson cancelMFOrder(const string& ordID) { return _sendReq(http::methods::DEL, FMT(_endpoints.at("mf.order.cancel"), "order_id"_a = ordID)); };

    /**
     * @brief get all mutual fund orders
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get mf orders
     */
    njson MFOrders() { return _sendReq(http::methods::GET, _endpoints.at("mf.orders")); };

    /**
     * @brief get details of a mutual fund order
     *
     * @param ordID order ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get mf order info
     */
    njson MFOrder(const string& ordID) { return _sendReq(http::methods::GET, FMT(_endpoints.at("mf.order.info"), "order_id"_a = ordID)); };

    /**
     * @brief get mutual fund holdings
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get mf holdings
     */
    njson MFHoldings() { return _sendReq(http::methods::GET, _endpoints.at("portfolio.holdings")); };


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
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp place mf sip order
     */
    njson placeMFSIP(const string& symbol, const string& amount, const string& installments, const string& freq, const string& initAmount = "",
        const string& installDay = "", const string& tag = "") {

        std::vector<std::pair<string, string>> bodyParams = {

            { "tradingsymbol", symbol },
            { "amount", amount },
            { "instalments", installments },
            { "frequency", freq },

        };

        if (!initAmount.empty()) { bodyParams.emplace_back("initial_amount", initAmount); }
        if (!installDay.empty()) { bodyParams.emplace_back("instalment_day", installDay); }
        if (!tag.empty()) { bodyParams.emplace_back("tag", tag); }

        return _sendReq(http::methods::POST, _endpoints.at("mf.sip.place"), bodyParams);
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
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp modify mf sip order
     */
    njson modifyMFSIP(const string& SIPID, const string& amount = "", const string& status = "", const string& installments = "",
        const string& freq = "", const string& installDay = "") {

        std::vector<std::pair<string, string>> bodyParams = {};

        if (!amount.empty()) { bodyParams.emplace_back("amount", amount); }
        if (!status.empty()) { bodyParams.emplace_back("status", status); }
        if (!installments.empty()) { bodyParams.emplace_back("instalments", installments); }
        if (!freq.empty()) { bodyParams.emplace_back("frequency", freq); }
        if (!installDay.empty()) { bodyParams.emplace_back("instalment_day", installDay); }

        return _sendReq(http::methods::PUT, FMT(_endpoints.at("mf.sip.modify"), "sip_id"_a = SIPID), bodyParams);
    };

    /**
     * @brief cancel a MF SIP
     *
     * @param SIPID SIP ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp cancel mf sip
     */
    njson cancelMFSIP(const string& SIPID) { return _sendReq(http::methods::DEL, FMT(_endpoints.at("mf.sip.cancel"), "sip_id"_a = SIPID)); };

    /**
     * @brief get list of SIPs
     *
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get sips
     */
    njson SIPs() { return _sendReq(http::methods::GET, _endpoints.at("mf.sips")); };

    /**
     * @brief get details of a SIP
     *
     * @param SIPID SIP ID
     * @return njson
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get sip info
     */
    njson SIP(const string& SIPID) { return _sendReq(http::methods::GET, FMT(_endpoints.at("mf.sip.info"), "sip_id"_a = SIPID)); };

    /**
     * @brief Get list of mutual fund instruments
     *
     * @return string
     *
     * @paragraph ex1 example
     * @snippet example2.cpp get mf instruments
     */
    string MFInstruments() { return _sendInstrumentsReq(_endpoints.at("mf.instruments")); };


    // others:

    /**
     * @brief get various margins required for orders
     *
     * @param orders
     * @return njson
     *
     * @attention
     * Function expects a njson array of orders. The array can be formed like
     * @code
     * auto ords = njson::array();
     * ords.push_back({

     * {"exchange", "NSE"},
     * {"tradingsymbol", "INFY"},
     * {"transaction_type", "BUY"},
     * {"variety", "regular"},
     * {"product", "CNC"},
     * {"order_type", "MARKET"},
     * {"quantity", 1},
     * {"price", 0},
     * {"trigger_price", 0}
     *
     * });
     * @endcode
     * and passed to the function like
     *
     * std::cout<<Kite.orderMargins(ords).dump(4)<<std::endl;
     *
     * Alternatively, users can create the array in-place.
     *
     *
     */
    njson orderMargins(const njson& orders) {

        /*
        Function expects a njson array of orders. The array can be formed like

        auto ords = njson::array();
        ords.push_back({

        {"exchange", "NSE"},
        {"tradingsymbol", "INFY"},
        {"transaction_type", "BUY"},
        {"variety", "regular"},
        {"product", "CNC"},
        {"order_type", "MARKET"},
        {"quantity", 1},
        {"price", 0},
        {"trigger_price", 0}

        });

        and passed to the function like

        std::cout<<Kite.orderMargins(ords).dump(4)<<std::endl;

        Alternatively, users can create the array in-place.
        */

        return _sendReq(http::methods::POST, _endpoints.at("order.margins"), { { "", orders.dump() } }, true);
    };


  private:
    // member variables:

    string _apiKey = "";
    string _accessToken = "";

    const string _kiteVersion = "3";
    const string _rootURL = "https://api.kite.trade/";
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
        { "market.historical", "/instruments/historical/{instrument_token}/{interval}?from={from}&to={to}&continuous={continuous}&oi={oi}" },
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

    http::client::http_client _httpClient;


    // methods:

    string _getAuthStr() const { return FMT("token {0}:{1}", _apiKey, _accessToken); };

    static string _encodeSymbolsList(const std::vector<string>& symbols) {

        string str;

        for (const auto& symbol : symbols) {

            //! could cause problems because there will that `&` after last query. can be solved by scraping the last char of string after the for
            //! loop
            str.append(FMT("i={0}&", symbol));
        };

        return str;
    }

    static string _encodeBody(const std::vector<std::pair<string, string>>& params) {

        // encodes body in `urlencoded` form

        string str;

        for (const auto& param : params) {

            //! could cause problems because there will that `&` after last query. can be solved by scraping the last char of string after the for
            //! loop
            str.append(FMT("{0}={1}&", param.first, param.second));
        };

        return str;
    };

    njson _sendReq(
        const http::method& mtd, const string& endpoint, const std::vector<std::pair<string, string>>& bodyParams = {}, bool isJson = false) {

        /*
        If the endpoint expects pure JSON body, set isJson to true and put the json body in second element of bodyParam's first pair with first
        element being empty string. see orderMargins() function
        */

        // create request

        http::http_request req(mtd);
        req.set_request_uri(U(endpoint));

        req.headers().set_content_type(U((isJson) ? "application/json" : "application/x-www-form-urlencoded"));
        req.headers().add(U("Authorization"), U(_getAuthStr()));
        req.headers().add(U("X-Kite-Version"), U(_kiteVersion));

        if ((mtd != http::methods::GET && mtd != http::methods::HEAD) && !bodyParams.empty()) {

            req.set_body(U((isJson) ? bodyParams[0].second : _encodeBody(bodyParams)));
        };


        // send request, get response and parse json

        http::http_response res = _httpClient.request(req).get();
        string dataRcvd = res.extract_string().get();

        if (!dataRcvd.empty()) {

            njson data;

            try {

                data = njson::parse(dataRcvd);

            } catch (const std::exception& e) { throw libException(FMT("{0} was thrown while parsing json (_sendReq-njson::parse)", e.what())); };

            if (res.status_code() == http::status_codes::OK) {

                // TODO can return njson["data"] & save users a step if it is established that every ok request has `data` field. It seems it is
                return data;
            }

            int code = 0;
            string excpStr;
            string message;

            try {

                code = res.status_code();
                excpStr = (data.contains("error_type")) ? data["error_type"] : "NoException";
                message = data["message"];

            } catch (const std::exception& e) {

                throw libException(
                    FMT("{0} was thrown while extracting code({1}), excpStr({2}) and message({3}) (_sendReq)", e.what(), code, excpStr, message));
            };

            _throwException(excpStr, code, message);


        } else {

            return njson();
        };

        return njson();
    };

    string _sendInstrumentsReq(const string& endpoint) {

        // create request

        http::http_request req(http::methods::GET);
        req.set_request_uri(U(endpoint));

        req.headers().set_content_type(U("application/x-www-form-urlencoded"));
        req.headers().add(U("Authorization"), U(_getAuthStr()));
        req.headers().add(U("X-Kite-Version"), U(_kiteVersion));

        // send request, get data

        http::http_response res = _httpClient.request(req).get();
        string dataRcvd = res.extract_string().get();

        if (!dataRcvd.empty()) {

            if (res.status_code() == http::status_codes::OK) { return dataRcvd; }

            int code = res.status_code();
            string excpStr = "NoException";

            _throwException(excpStr, code, "");


        } else {

            return "";
        };

        return "";
    };
};


} // namespace kitepp