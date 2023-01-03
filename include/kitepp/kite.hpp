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

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "httplib.h"

#include "responses/responses.hpp"
#include "utils.hpp"

namespace kiteconnect {

using std::string;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

///
/// \brief \a kite represents a KiteConnect session. It wraps around the
///        KiteConnect REST API and provides a native interface.
///
class kite {

  public:
    ///
    /// \brief Construct a new kite object.
    ///
    /// \param apikey kiteconnect api key
    ///
    explicit kite(string apikey);

    // api

    ///
    /// \brief Set the API key for current session.
    ///
    /// \param arg API key is set to \a arg
    ///
    void setApiKey(const string& arg);

    ///
    /// \brief Get current session's API key.
    ///
    /// \return string API key
    ///
    string getApiKey() const;

    ///
    /// \brief Get the login URL to which the user should be redirected to
    /// initiate the login flow.
    ///
    /// \return string login URL
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp obtaining login url
    ///
    string loginURL() const;

    ///
    /// \brief Set the access token current session.
    ///
    /// \param arg access token is set to \a arg
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp settting access token
    ///
    void setAccessToken(const string& arg);

    ///
    /// \brief Get access token set for current session.
    ///
    /// \return string access token
    ///
    string getAccessToken() const;

    ///
    /// \brief Generate an user session. Use this method to generate an access
    ///        token.
    ///
    /// \param requestToken request token to use
    /// \param apiSecret corresponding API secret
    /// \return userSession session details
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp obtaining access token
    ///
    userSession generateSession(
        const string& requestToken, const string& apiSecret);

    ///
    /// \brief This method invalidates the access token and destroys current
    ///        session.
    ///
    /// \note After this, the user should be sent through a new login flow
    ///       before further interactions.
    ///       This does not log the user out of the
    ///       official Kite web or mobile applications.
    ///
    /// \return `true` if session was invalidated successfully, `false`
    ///         otherwise.
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp invalidate session
    ///
    bool invalidateSession();

    // user

    ///
    /// \brief Get user's profile.
    ///
    /// \return userProfile user profile
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get user profile
    ///
    userProfile profile();

    /// \brief Get margins for all segments.
    ///
    /// \return allMargins margins
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get margins
    ///
    allMargins getMargins();

    /// \brief Get margins for a particular segment.
    ///
    /// \param segment segment whose margins should be fetched
    ///
    /// \return margins segment margins
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get margins
    ///
    margins getMargins(const string& segment);

    // orders

    ///
    /// \brief Place an order.
    ///
    /// \param params parameters of order to place
    ///
    /// \return string ID of placed order
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp placing an order
    ///
    string placeOrder(const placeOrderParams& params);

    /**
     * \brief modify an order
     *
     * \param variety variety of the order
     * \param ordID order ID
     * \param parentOrdID parent order ID
     * \param quantity quantity to transact
     * \param price the min or max price to execute the order at (for LIMIT
     * orders)
     * \param ordType order type
     * \param trigPrice trigger price
     * \param validity order validity
     * \param discQuantity disclosed quantity
     *
     * \return string order ID
     *
     * \paragraph ex1 Example
     * \snippet example2.cpp modifying an order
     */

    ///
    /// \brief Modify an order.
    ///
    /// \param params parameters of the order to modify
    ///
    /// \return string ID of modified order
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp modifying an order
    ///
    string modifyOrder(const modifyOrderParams& params);

    /**
     * \brief cancel an order
     *
     * \param variety variety of the order
     * \param ordID order ID
     * \param parentOrdID parent order ID
     *
     * \return string order ID
     *
     * \paragraph ex1 Example
     * \snippet example2.cpp cancelling an order
     */

    ///
    /// \brief Cancel an order.
    ///
    /// \param variety       variety of order to cancel
    /// \param orderId       ID of order to cancel
    /// \param parentOrderId parent order ID of the order to cancel (if any)
    ///
    /// \return string ID of the cancelled order
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp cancelling an order
    ///
    string cancelOrder(const string& variety, const string& orderId,
        const string& parentOrderId = "");

    ///
    /// \brief Exit an order.
    ///
    /// \param variety       variety of the order to exit
    /// \param orderId       ID of the order to exit
    /// \param parentOrderId parent order ID of the order to exit (if any)
    ///
    /// \return string ID of the order exited
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp exiting an order
    ///
    string exitOrder(const string& variety, const string& orderId,
        const string& parentOrderId = "");

    ///
    /// \brief Get list of orders.
    ///
    /// \return std::vector<order> orders
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get orders
    ///
    std::vector<order> orders();

    ///
    /// \brief Get history of an order.
    ///
    /// \param orderId ID of order whose history should be fetched.
    ///
    /// \return std::vector<order> order history
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get order history
    ///
    std::vector<order> orderHistory(const string& orderId);

    ///
    /// \brief Get list of trades.
    ///
    /// \return std::vector<trade> trades
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get trades
    ///
    std::vector<trade> trades();

    ///
    /// \brief Get the list of trades executed for a particular order.
    ///
    /// \param orderId ID of order whose trades should be fetched
    ///
    ///  \return std::vector<trade> trades
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get order trades
    ///
    std::vector<trade> orderTrades(const string& orderId);

    // gtt

    ///
    /// \brief Place a GTT.
    ///
    /// \param params parameters of the GTT to place.
    ///
    /// \return int trigger ID
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp placing a gtt
    ///
    int placeGtt(const placeGttParams& params);

    ///
    /// \brief Get list of GTTs.
    ///
    /// \return std::vector<GTT> triggers
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get gtts
    ///
    std::vector<GTT> triggers();

    ///
    /// \brief Get details of a particular GTT.
    ///
    /// \param triggerId ID of trigger whose details should be fetched
    /// \return GTT trigger details
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get gtt info
    ///
    GTT getGtt(int triggerId);

    ///
    /// \brief Modify a GTT.
    ///
    /// \param params parameters required to modify a GTT
    ///
    /// \return int ID of modified trigger
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp modifying a gtt
    ///
    int modifyGtt(const kc::modifyGttParams& params);

    ///
    /// \brief Delete a GTT.
    ///
    /// \param triggerId ID of trigger to delete
    ///
    /// \return int ID of deleted trigger
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp delete a gtt
    ///
    int deleteGtt(int triggerId);

    // portfolio

    ///
    /// \brief Get holdings.
    ///
    /// \return std::vector<holding> holdings
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get holdings
    ///
    std::vector<holding> holdings();

    ///
    /// \brief Get positions.
    ///
    /// \return positions positions
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get positions
    ///
    positions getPositions();

    ///
    /// \brief Convert an open position to a different product type.
    ///
    /// \param params parameters required to convert a position
    ///
    /// \return bool `true` the if position was successfully modified, `false`
    ///              otherwise.
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp convert position
    ///
    bool convertPosition(const convertPositionParams& params);

    // market

    ///
    /// \brief Retrieve the list of market instruments available to trade.
    ///
    /// \param exchange if specified, only instruments available on this
    ///                 exchange are fetched.
    ///
    /// \return std::vector<instrument> instruments
    ///
    /// \attention Results could be large, with tens of thousands of entries.
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get instruments
    ///
    std::vector<instrument> getInstruments(const string& exchange = "");

    ///
    /// \brief Retrieve quote for a list of instruments.
    ///
    /// \param symbols list of instruments whose quotes should be fetched.
    ///                format of each entry should be `exchange:tradingsymbol`.
    ///                example: `NSE:INFY`.
    ///
    /// \return std::unordered_map<string, quote> quotes mapped to respective
    ///                                           insruments. format of
    ///                                           keys is similar to
    ///                                           \a symbols.
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get quote
    ///
    std::unordered_map<string, quote> getQuote(
        const std::vector<string>& symbols);

    ///
    /// \brief Retrieve OHLC info for a list of instruments.
    ///
    /// \param symbols list of instruments whose OHLC info should be fetched.
    ///                format of each entry should be `exchange:tradingsymbol`.
    ///                example: `NSE:INFY`.
    ///
    /// \return std::unordered_map<string, OHLCQuote> OHLC info mapped to
    ///                                               respective insruments.
    ///                                               format of keys is similar
    ///                                               to \a symbols.
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get ohlc
    ///
    std::unordered_map<string, ohlcQuote> getOhlc(
        const std::vector<string>& symbols);

    ///
    /// \brief Retrieve Last Traded Price for a list of instruments.
    ///
    /// \param symbols list of instruments whose OHLC info should be fetched.
    ///                format of each entry should be `exchange:tradingsymbol`.
    ///                example: `NSE:INFY`.
    ///
    /// \return std::unordered_map<string, LTPQuote> LTP info mapped to
    ///                                              respective insruments.
    ///                                              format of keys is similar
    ///                                              to \a symbols.
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get ltp
    ///
    std::unordered_map<string, ltpQuote> getLtp(
        const std::vector<string>& symbols);

    ///
    /// \brief Retrieve historical data of an instrument.
    ///
    /// \param params paramters required to fetch the data.
    ///
    /// \return std::vector<historicalData> historical data
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get historical data
    ///
    std::vector<historicalData> getHistoricalData(
        const historicalDataParams& params);

    ///
    /// \brief Get margins required for placing particular orders.
    ///
    /// \param params list of paramters required to fetch margins. each entry
    ///               represents an order.
    ///
    /// \return std::vector<orderMargins> margins
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get order margins
    ///
    std::vector<orderMargins> getOrderMargins(
        const std::vector<orderMarginsParams>& params);

    // MF

    ///
    /// \brief Place a mutual fund order.
    ///
    /// \param params parameters required to place the order
    ///
    /// \return string ID of the order placed
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp place mf order
    ///
    string placeMfOrder(const placeMfOrderParams& params);

    ///
    /// \brief Cancel a mutual fund order.
    ///
    /// \param orderId ID of order to cancel
    ///
    /// \return string ID of cancelled order
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp cancel a mf order
    ///
    string cancelMfOrder(const string& orderId);

    ///
    /// \brief Get mutual fund orders.
    ///
    /// \return std::vector<mfOrder> orders
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get mf orders
    ///
    std::vector<mfOrder> getMfOrders();

    ///
    /// \brief Get details of a particular mutual fund order
    ///
    /// \param orderId ID of the order whose details should be fetched
    ///
    /// \return mfOrder order details
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get mf order info
    ///
    mfOrder getMfOrder(const string& orderId);

    ///
    /// \brief Get mutual fund holdings.
    ///
    /// \return std::vector<mfHolding> holdings
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get mf holdings
    ///
    std::vector<mfHolding> getMfHoldings();

    ///
    /// \brief Place a mutual fund SIP order.
    ///
    /// \param symbol tradingsymbol (ISIN) of the fund
    ///
    /// \return placeMfSipResponse IDs of the placed order
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp place mf sip order
    ///
    placeMfSipResponse placeMfSip(const placeMfSipParams& params);

    ///
    /// \brief Modify a mutual SIP order.
    ///
    /// \param params parameters required to modify the order
    ///
    /// \return string ID of the modified order
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp modify mf sip order
    ////
    string modifyMfSip(const modifyMfSipParams& params);

    ///
    /// \brief Cancel a mutual fund SIP.
    ///
    /// \param sipId ID of SIP to cancel
    ///
    /// \return string ID of cancelled SIP
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp cancel mf sip
    ///
    string cancelMfSip(const string& sipId);

    ///
    /// \brief Get list of SIPs.
    ///
    /// \return std::vector<mfSips> SIPs
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get sips
    ///
    std::vector<mfSip> getSips();

    ///
    /// \brief Get details of a particular SIP.
    ///
    /// \param sipId ID of the SIP whose details should be fetched
    ///
    /// \return mfSip SIP details
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get sip info
    ///
    mfSip getSip(const string& sipId);

    ///
    /// \brief Get the list of mutual fund instruments available for trading.
    ///
    /// \return std::vector<mfInstrument> instruments
    ///
    /// \paragraph ex1 Example
    /// \snippet example2.cpp get mf instruments
    ///
    std::vector<mfInstrument> getMfInstruments();

  private:
    static string encodeSymbolsList(const std::vector<string>& symbols);

    string getAuth() const;

    template <class Res, class Data, bool UseCustomParser = false>
    inline Res callApi(const string& service,
        const utils::http::Params& body = {},
        const utils::FmtArgs& fmtArgs = {},
        utils::json::CustomParser<Res, Data, UseCustomParser>
            customParser = {});

    const string version = "3";
    const string root = "https://api.kite.trade";
    const string loginUrlFmt =
        "https://kite.zerodha.com/connect/login?v=3&api_key={api_key}";
    const std::unordered_map<string, utils::http::endpoint> endpoints {
        // api
        { "api.token", { utils::http::METHOD::POST, "/session/token" } },
        { "api.token.invalidate",
            { utils::http::METHOD::DEL,
                "/session/token?api_key={0}&access_token={1}" } },
        // user
        { "user.profile", { utils::http::METHOD::GET, "/user/profile" } },
        { "user.margins", { utils::http::METHOD::GET, "/user/margins" } },
        { "user.margins.segment",
            { utils::http::METHOD::GET, "/user/margins/{0}" } },
        // order
        { "order.info", { utils::http::METHOD::GET, "/orders/{0}" } },
        { "order.place", { utils::http::METHOD::POST, "/orders/{0}" } },
        { "order.modify", { utils::http::METHOD::PUT, "/orders/{0}/{1}" } },
        { "order.cancel", { utils::http::METHOD::DEL, "/orders/{0}/{1}" } },
        { "order.cancel.bo", { utils::http::METHOD::DEL,
                                 "/orders/{0}/{1}?parent_order_id={1}" } },
        { "order.trades", { utils::http::METHOD::GET, "/orders/{0}/trades" } },
        { "orders", { utils::http::METHOD::GET, "/orders" } },
        { "trades", { utils::http::METHOD::GET, "/trades" } },
        { "order.margins", { utils::http::METHOD::POST, "/margins/orders",
                               utils::http::CONTENT_TYPE::JSON } },
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
        { "mf.instruments", { utils::http::METHOD::GET, "/mf/instruments",
                                utils::http::CONTENT_TYPE::NON_JSON,
                                utils::http::CONTENT_TYPE::JSON } },
        { "mf.sips", { utils::http::METHOD::GET, "/mf/sips" } },
        { "mf.sip.info", { utils::http::METHOD::GET, "/mf/sips/{0}" } },
        { "mf.sip.place", { utils::http::METHOD::POST, "/mf/sips" } },
        { "mf.sip.modify", { utils::http::METHOD::PUT, "/mf/sips/{0}" } },
        { "mf.sip.cancel", { utils::http::METHOD::DEL, "/mf/sips/{0}" } },
        // portfolio
        { "portfolio.holdings",
            { utils::http::METHOD::GET, "/portfolio/holdings" } },
        { "portfolio.positions",
            { utils::http::METHOD::GET, "/portfolio/positions" } },
        { "portfolio.positions.convert",
            { utils::http::METHOD::PUT, "/portfolio/positions" } },
        // market endpoints
        { "market.instruments.all", { utils::http::METHOD::GET, "/instruments",
                                        utils::http::CONTENT_TYPE::NON_JSON,
                                        utils::http::CONTENT_TYPE::JSON } },
        { "market.instruments", { utils::http::METHOD::GET, "/instruments/{0}",
                                    utils::http::CONTENT_TYPE::NON_JSON,
                                    utils::http::CONTENT_TYPE::JSON } },
        { "market.margins", { utils::http::METHOD::GET, "/margins/{0}" } },
        { "market.historical",
            { utils::http::METHOD::GET,
                "/instruments/historical/{0}/"
                "{1}?from={2}&to={3}&continuous={4}&oi={5}" } },
        { "market.trigger_range",
            { utils::http::METHOD::GET, "/instruments/trigger_range/{9}" } },
        { "market.quote", { utils::http::METHOD::GET, "/quote?{0}" } },
        { "market.quote.ohlc",
            { utils::http::METHOD::GET, "/quote/ohlc?{0}" } },
        { "market.quote.ltp", { utils::http::METHOD::GET, "/quote/ltp?{0}" } },
    };
    string key;
    string token;
    string authorization;
    httplib::Client client;

  protected:
#ifdef KITE_UNIT_TEST
    virtual utils::http::response sendReq(const utils::http::endpoint& endpoint,
        const utils::http::Params& body, const utils::FmtArgs& fmtArgs);
#else
    ///
    /// \brief send a http request with the context used by \a kite
    ///
    /// \param endpoint request endpoint
    /// \param body     body of the request (sent as form url encoded)
    ///
    /// \return utils::http::response response received
    ///
    utils::http::response sendReq(const utils::http::endpoint& endpoint,
        const utils::http::Params& body, const utils::FmtArgs& fmtArgs);
#endif
};

} // namespace kiteconnect