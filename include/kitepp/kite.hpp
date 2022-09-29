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

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "cpp-httplib/httplib.h"

#include "config.hpp"
#include "responses/responses.hpp"
#include "utils.hpp"

namespace kiteconnect {

using std::string;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/**
 * @brief Used for accessing REST interface provided by Kite API.
 *
 */
class kite {

  public:
    /**
     * @brief Construct a new kite object
     *
     * @param apikey
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp initializing kite
     */
    explicit kite(string apikey);

    // api

    /**
     * @brief Set the API key
     *
     * @param arg The string that should be set as API key
     */
    void setApiKey(const string& arg);

    /**
     * @brief Fetch current API key
     *
     * @return string
     */
    string getApiKey() const;

    /**
     * @brief Get the remote login url to which a user should be redirected to initiate the login flow.
     *
     * @return string
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp obtaining login url
     */
    string loginURL() const;

    /**
     * @brief Set the Access Token
     *
     * @param arg the string you want to set as access token
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp settting access token
     */
    void setAccessToken(const string& arg);

    /**
     * @brief Get the Access Token set currently
     *
     * @return string
     */
    string getAccessToken() const;

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
    userSession generateSession(const string& requestToken, const string& apiSecret);

    /**
     * @brief This call invalidates the access_token and destroys the API session. After this, the user should be sent
     * through a new login flow before further interactions. This does not log the user out of the official Kite web or
     * mobile applications.
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp invalidate session
     */
    bool invalidateSession();

    // user

    /**
     * @brief returns user profile
     *
     * @return userProfile
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get user profile
     */
    userProfile profile();

    /**
     * @brief Get margins for all segments
     *
     * @return allMargins
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get margins
     */
    allMargins getMargins();

    /**
     * @brief Get margins for a particular segment
     *
     * @param segment
     * @return margins
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get margins
     */
    margins getMargins(const string& segment);

    // orders

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
    string placeOrder(const placeOrderParams& params);

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
    string modifyOrder(const modifyOrderParams& params);

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
    string cancelOrder(const string& variety, const string& orderId, const string& parentOrderId = "");

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
    string exitOrder(const string& variety, const string& orderId, const string& parentOrderId = "");

    /**
     * @brief get list of orders
     *
     * @return std::vector<order>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get orders
     */
    std::vector<order> orders();

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
    std::vector<order> orderHistory(const string& orderId);

    /**
     * @brief get list of trades
     *
     * @return std::vector<trade>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get trades
     */
    std::vector<trade> trades();

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
    std::vector<trade> orderTrades(const string& orderId);

    // gtt

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
    int placeGtt(const placeGttParams& params);

    /**
     * @brief get list of GTTs
     *
     * @return std::vector<GTT>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get gtts
     */
    std::vector<GTT> triggers();

    /**
     * @brief get details of a GTT
     *
     * @param trigID trigger ID
     * @return GTT
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get gtt info
     */
    GTT getGtt(int triggerId);

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
    int modifyGtt(const kc::modifyGttParams& params);

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
    int deleteGtt(int triggerId);

    // portfolio

    /**
     * @brief get holdings
     *
     * @return std::vector<holding>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get holdings
     */
    std::vector<holding> holdings();

    /**
     * @brief get positions
     *
     * @return positions
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get positions
     */
    positions getPositions();

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
    bool convertPosition(const convertPositionParams& params);

    // market

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
    std::vector<instrument> getInstruments(const string& exchange = "");

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
    std::unordered_map<string, quote> getQuote(const std::vector<string>& symbols);

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
    std::unordered_map<string, ohlcQuote> getOhlc(const std::vector<string>& symbols);

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
    std::unordered_map<string, ltpQuote> getLtp(const std::vector<string>& symbols);

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
    std::vector<historicalData> getHistoricalData(const historicalDataParams& params);

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
    std::vector<orderMargins> getOrderMargins(const std::vector<orderMarginsParams>& params);

    // MF

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
    string placeMfOrder(const placeMfOrderParams& params);

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
    string cancelMfOrder(const string& orderId);

    /**
     * @brief get all mutual fund orders
     *
     * @return std::vector<MFOrder>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf orders
     */
    std::vector<mfOrder> getMfOrders();

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
    mfOrder getMfOrder(const string& orderId);

    /**
     * @brief get mutual fund holdings
     *
     * @return std::vector<MFHolding>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf holdings
     */
    std::vector<mfHolding> getMfHoldings();

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
    placeMfSipResponse placeMfSip(const placeMfSipParams& params);

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
    string modifyMfSip(const modifyMfSipParams& params);

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
    string cancelMfSip(const string& sipId);

    /**
     * @brief get list of SIPs
     *
     * @return std::vector<MFSIP>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get sips
     */
    std::vector<mfSip> getSips();

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
    mfSip getSip(const string& sipId);

    /**
     * @brief Get list of mutual fund instruments
     *
     * @return std::vector<MFInstrument>
     *
     * @paragraph ex1 Example
     * @snippet example2.cpp get mf instruments
     */
    std::vector<mfInstrument> getMfInstruments();

  private:
    static string encodeSymbolsList(const std::vector<string>& symbols);

    string getAuth() const;

    template <class Res, class Data, bool UseCustomParser = false>
    inline Res callApi(const string& service, const utils::http::Params& body = {}, const utils::FmtArgs& fmtArgs = {},
        utils::json::CustomParser<Res, Data, UseCustomParser> customParser = {});

    const string version = "3";
    const string root = "https://api.kite.trade";
    const string loginUrlFmt = "https://kite.zerodha.com/connect/login?v=3&api_key={api_key}";
    const std::unordered_map<string, utils::http::endpoint> endpoints {
        // api
        { "api.token", { utils::http::METHOD::POST, "/session/token" } },
        { "api.token.invalidate", { utils::http::METHOD::DEL, "/session/token?api_key={0}&access_token={1}" } },
        // user
        { "user.profile", { utils::http::METHOD::GET, "/user/profile" } },
        { "user.margins", { utils::http::METHOD::GET, "/user/margins" } },
        { "user.margins.segment", { utils::http::METHOD::GET, "/user/margins/{0}" } },
        // order
        { "order.info", { utils::http::METHOD::GET, "/orders/{0}" } },
        { "order.place", { utils::http::METHOD::POST, "/orders/{0}" } },
        { "order.modify", { utils::http::METHOD::PUT, "/orders/{0}/{1}" } },
        { "order.cancel", { utils::http::METHOD::DEL, "/orders/{0}/{1}" } },
        { "order.cancel.bo", { utils::http::METHOD::DEL, "/orders/{0}/{1}?parent_order_id={1}" } },
        { "order.trades", { utils::http::METHOD::GET, "/orders/{0}/trades" } },
        { "orders", { utils::http::METHOD::GET, "/orders" } },
        { "trades", { utils::http::METHOD::GET, "/trades" } },
        { "order.margins", { utils::http::METHOD::POST, "/margins/orders", utils::http::CONTENT_TYPE::JSON } },
        // gtt
        { "gtt", { utils::http::METHOD::GET, "/gtt/triggers" } },
        { "gtt.place", { utils::http::METHOD::POST, "/gtt/triggers" } },
        { "gtt.info", { utils::http::METHOD::GET, "/gtt/triggers/{0}" } },
        { "gtt.modify", { utils::http::METHOD::PUT, "/gtt/triggers/{0}" } },
        { "gtt.delete", { utils::http::METHOD::DEL, "/gtt/triggers/{0}" } },
        // mf
        { "mf.orders", { utils::http::METHOD::GET, "/mf/orders" } },
        { "mf.order.info", { utils::http::METHOD::GET, "/mf/orders/{0}" } },
        { "mf.order.place", { utils::http::METHOD::POST, "/mf/orders" } },
        { "mf.order.cancel", { utils::http::METHOD::DEL, "/mf/orders/{0}" } },
        { "mf.holdings", { utils::http::METHOD::GET, "/mf/holdings" } },
        { "mf.instruments", { utils::http::METHOD::GET, "/mf/instruments", utils::http::CONTENT_TYPE::NON_JSON,
                                utils::http::CONTENT_TYPE::JSON } },
        { "mf.sips", { utils::http::METHOD::GET, "/mf/sips" } },
        { "mf.sip.info", { utils::http::METHOD::GET, "/mf/sips/{0}" } },
        { "mf.sip.place", { utils::http::METHOD::POST, "/mf/sips" } },
        { "mf.sip.modify", { utils::http::METHOD::PUT, "/mf/sips/{0}" } },
        { "mf.sip.cancel", { utils::http::METHOD::DEL, "/mf/sips/{0}" } },
        // portfolio
        { "portfolio.holdings", { utils::http::METHOD::GET, "/portfolio/holdings" } },
        { "portfolio.positions", { utils::http::METHOD::GET, "/portfolio/positions" } },
        { "portfolio.positions.convert", { utils::http::METHOD::PUT, "/portfolio/positions" } },
        // market endpoints
        { "market.instruments.all", { utils::http::METHOD::GET, "/instruments", utils::http::CONTENT_TYPE::NON_JSON,
                                        utils::http::CONTENT_TYPE::JSON } },
        { "market.instruments", { utils::http::METHOD::GET, "/instruments/{0}", utils::http::CONTENT_TYPE::NON_JSON,
                                    utils::http::CONTENT_TYPE::JSON } },
        { "market.margins", { utils::http::METHOD::GET, "/margins/{0}" } },
        { "market.historical", { utils::http::METHOD::GET, "/instruments/historical/{0}/"
                                                           "{1}?from={2}&to={3}&continuous={4}&oi={5}" } },
        { "market.trigger_range", { utils::http::METHOD::GET, "/instruments/trigger_range/{9}" } },
        { "market.quote", { utils::http::METHOD::GET, "/quote?{0}" } },
        { "market.quote.ohlc", { utils::http::METHOD::GET, "/quote/ohlc?{0}" } },
        { "market.quote.ltp", { utils::http::METHOD::GET, "/quote/ltp?{0}" } },
    };
    string key;
    string token;
    string authorization;
    httplib::Client client;

  protected:
#ifdef KITE_UNIT_TEST
    virtual utils::http::response sendReq(
        const utils::http::endpoint& endpoint, const utils::http::Params& body, const utils::FmtArgs& fmtArgs);
#else
    /**
     * \brief send a http request with the context used by \a kite
     *
     * \param endpoint request endpoint
     * \param body     body of the request (sent as form url encoded)
     *
     * \return utils::http::response response received
     */
    utils::http::response sendReq(
        const utils::http::endpoint& endpoint, const utils::http::Params& body, const utils::FmtArgs& fmtArgs);
#endif
};

} // namespace kiteconnect