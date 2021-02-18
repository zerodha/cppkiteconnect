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

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

//! [adding kitepp header]
#include "kitepp.hpp"
#include "kitepp/responses.hpp"
//! [adding kitepp header]

namespace kc = kiteconnect;

int main() {

    std::cout << "RUNNING\n";

    //! Content is commented out on purpose so you can enable the parts you want

    try {

        //! [initializing kite]
        kc::kite Kite(std::getenv("KITE_API_KEY"));
        // kc::kite Kite("---apikey---");
        //! [initializing kite]

        //! [obtaining login url]
        std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";
        //! [obtaining login url]

        //! [obtaining access token]
        std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;

        // std::cout << "Enter API secret: ";
        // std::cin >> apiSecret;
        apiSecret = std::getenv("KITE_API_SECRET");

        std::string accessToken = Kite.generateSession(reqToken, apiSecret).tokens.accessToken;
        std::cout << "access token is " << accessToken << "\n";
        //! [obtaining access token]

        //! [settting access token]
        Kite.setAccessToken(accessToken);
        // Kite.setAccessToken("---accesstoken---");
        //! [settting access token]

        //! [invalidate session]
        // Kite.invalidateSession();
        //! [invalidate session]

        //! [get user profile]
        kc::userProfile profile = Kite.profile();
        std::cout << "email is :" << profile.email << "\n";
        std::cout << "Order types are: \n";
        for (const std::string& type : profile.orderTypes) { std::cout << type << ", "; };
        //! [get user profile]

        //! [get margins]
        /*kc::allMargins Margins = Kite.getMargins();
        std::cout << "Available cash for equity segment is " << Margins.equity.available.cash << "\n";

        kc::margins eqMargins = Kite.getMargins("equity");
        std::cout << "adhoc margin for equity segment is " << eqMargins.available.adHocMargin << "\n";*/
        //! [get margins]

        //! [placing an order]
        /*std::string ordID = Kite.placeOrder("co", "NSE", "BHEL", "BUY", 1, "MIS", "LIMIT", 33.5, "DAY", 32,
        kc::DEFAULTDOUBLE, std::string ordID2 = Kite.placeOrder("regular", "NSE", "BHEL", "SELL", 1, "MIS",
        "MARKET", 33.60, "DAY"); kc::DEFAULTDOUBLE, kc::DEFAULTDOUBLE); if (!ordID.empty()) {

            std::cout << "order ID: " << ordID << "\n";
        } else {

            std::cout << "failed to place order"
                      << "\n";
        };*/
        //! [placing an order]

        //! [modifying an order]
        // std::cout << "modify order response: " << Kite.modifyOrder("regular", "151220000000000", "", "2", "",
        // "MARKET", "", "DAY").dump(4) << "\n";

        /*std::string modOrdID = Kite.modifyOrder("regular", "201223002459577", "", 2, Kite.DEFAULTDOUBLE, "MARKET",
        Kite.DEFAULTDOUBLE, "DAY"); if (!modOrdID.empty()) {

            std::cout << "order ID: " << modOrdID << "\n";
        } else {

            std::cout << "failed to modify order"
                      << "\n";
        };*/
        //! [modifying an order]

        //! [cancelling an order]
        /*std::string canOrdID = Kite.cancelOrder("regular", "201223002513052");

        if (!canOrdID.empty()) {

            std::cout << "order ID: " << canOrdID << "\n";
        } else {

            std::cout << "failed to cancel order"
                      << "\n";
        };*/
        //! [cancelling an order]

        //! [exiting an order]
        /*std::string exitOrdID = Kite.exitOrder("regular", "201223002546330", "201223002546328");

        if (!exitOrdID.empty()) {

            std::cout << "order ID: " << exitOrdID << "\n";
        } else {

            std::cout << "failed to exit order"
                      << "\n";
        };*/
        //! [exiting an order]

        //! [get orders]
        /*std::vector<kc::order> orderVec = Kite.orders();
        for (const auto& i : orderVec) { std::cout << "Order ID: " << i.orderID << " & status: " << i.status << "\n";
        }*/
        //! [get orders]

        //! [get order history]
        /*std::vector<kc::order> orderHistoryVec = Kite.orderHistory("201223002380513");
        for (const auto& i : orderHistoryVec) { std::cout << "status: " << i.status << " & filled quantity: " <<
        i.filledQuantity << "\n"; };*/
        //! [get order history]

        //! [get trades]
        /*std::vector<kc::trade> tradesVec = Kite.trades();
        for (const auto& i : tradesVec) { std::cout << "trade ID: " << i.tradeID << " & filled at: " << i.fillTimestamp
        << "\n"; };*/
        //! [get trades]

        //! [get order trades]
        /*std::vector<kc::trade> tradesVec = Kite.orderTrades("201223002511160");
        for (const auto& i : tradesVec) { std::cout << "trade ID: " << i.tradeID << " & filled at: " << i.fillTimestamp
        << "\n"; };*/
        //! [get order trades]

        //! [placing a gtt]
        /*kc::GTTParams gttparam("BUY", 1, "LIMIT", "CNC", 21100);
        std::vector<kc::GTTParams> gttparams = { gttparam };
        // gttparams.emplace_back("BUY", 10, "LIMIT", "CNC", 21105);
        // int trigID = Kite.placeGTT("two-leg", "NESTLEIND", "NSE", { 15000, 20007 }, 18580.0, gttparams);

        int trigID = Kite.placeGTT("single", "NESTLEIND", "NSE", { 20000 }, 18450.70, gttparams);

        if (!kc::isValid(trigID)) {

            std::cout << "trigger ID: " << trigID << "\n";
        } else {

            std::cout << "failed to place gtt"
                      << "\n";
        };*/
        //! [placing a gtt]

        //! [modifying a gtt]
        /*kc::GTTParams gttparam("BUY", 1, "LIMIT", "CNC", 21100);
        std::vector<kc::GTTParams> gttparams = { gttparam };

        int modTrigID = Kite.modifyGTT(24028028, "single", "NESTLEIND", "NSE", { 23500 }, 18450.70, gttparams);

        if (!kc::isValid(modTrigID)) {

            std::cout << "trigger ID: " << modTrigID << "\n";
        } else {

            std::cout << "failed to modify gtt"
                      << "\n";
        };*/
        //! [modifying a gtt]

        //! [get gtts]
        /*std::vector<kc::GTT> gttVec = Kite.getGTTs();
        for (const auto& gtt : gttVec) { std::cout << "ID: " << gtt.ID << " & created at: " << gtt.createdAt << "\n";
        };*/
        //! [get gtts]

        //! [get gtt info]
        /*kc::GTT gtt = Kite.getGTT(23271899);
        std::cout << "Created at: " << gtt.createdAt << " & first trigger value: " << gtt.condition.triggerValues[0] <<
        "\n";*/
        //! [get gtt info]

        //! [delete a gtt]
        /*int trigID = Kite.deleteGTT(24028028);

        if (!kc::isValid(trigID)) {

            std::cout << "trigger ID: " << trigID << "\n";
        } else {

            std::cout << "failed to delete gtt"
                      << "\n";
        };*/
        //! [delete a gtt]

        //! [get holdings]
        /*std::vector<kc::holding> holdings = Kite.holdings();
        for (const auto& i : holdings) { std::cout << "trading symbol: " << i.tradingsymbol << " & average price: " <<
        i.averagePrice << "\n"; };*/
        //! [get holdings]

        //! [get positions]
        /*kc::positions pos = Kite.getPositions();
        if (!pos.day.empty()) { std::cout << "tradingsymbol: " << pos.day[0].tradingsymbol << " & quantity: " <<
        pos.day[0].quantity << "\n"; };*/
        //! [get positions]

        //! [convert position]
        /*if (Kite.convertPosition("NSE", "BHEL", "BUY", "day", 1, "MIS", "CNC")) {

            std::cout << "Successfully converted the position\n";
        } else {

            std::cout << "Failed to convert the position\n";
        };*/
        //! [convert position]

        //! [get quote]
        /*std::unordered_map<std::string, kc::quote> quotes = Kite.getQuote({ "NSE:INFY", "NSE:NIFTY 50",
        "NSE:RELIANCE" }); std::cout << "nifty50 close: " << quotes["NSE:NIFTY 50"].OHLC.close << "\n"; if
        (quotes.find("NSE:INFY") != quotes.end()) {

            std::cout << "INFY open: " << quotes["NSE:INFY"].OHLC.open << "\n";

        } else {

            std::cout << "INFY quote not found\n";
        };*/
        //! [get quote]

        //! [get ohlc]
        /*std::unordered_map<std::string, kc::OHLCQuote> ohlcQuotes = Kite.getOHLC({ "NSE:INFY", "NSE:NIFTY 50",
        "NSE:RELIANCE" }); std::cout << "nifty50 close: " << ohlcQuotes["NSE:NIFTY 50"].OHLC.close << "\n";*/
        //! [get ohlc]

        //! [get ltp]
        /*std::unordered_map<std::string, kc::LTPQuote> ltpQuotes = Kite.getLTP({ "NSE:INFY", "NSE:NIFTY 50",
        "NSE:RELIANCE" }); std::cout << "RELIANCE LTP: " << ltpQuotes["NSE:RELIANCE"].lastPrice << "\n";*/
        //! [get ltp]

        //! [get historical data]
        /*std::vector<kc::historicalData> histData = Kite.getHistoricalData(5633, "2020-12-02 09:30:00", "2020-12-10
        10:30:00", "minute");
        for (const auto& i : histData) { std::cout << "datetime: " << i.datetime << " close: " << i.close << "\n"; };*/
        //! [get historical data]

        //! [place mf order]
        /*std::string MFOrdID = Kite.placeMFOrder("INF174K01LS2", "BUY", kc::DEFAULTINT, 10000);
        if (!MFOrdID.empty()) {

            std::cout << "order ID: " << MFOrdID << "\n";
        } else {

            std::cout << "failed to place order"
                      << "\n";
        };*/
        //! [place mf order]

        //! [get mf order info]
        /*kc::MFOrder mforder = Kite.getMFOrder("51ac086a-0e39-42b5-a725-157507614d79");
        std::cout << "status: " << mforder.status << "\n";*/
        //! [get mf order info]

        //! [get mf orders]
        /*std::vector<kc::MFOrder> mforders = Kite.getMFOrders();
        for (const auto& i : mforders) { std::cout << "order ID: " << i.orderID << " status: " << i.status << "\n"; };*/
        //! [get mf orders]

        //! [cancel a mf order]
        /*std::string canMforder = Kite.cancelMFOrder("51ac086a-0e39-42b5-a725-157507614d79");
        if (!canMforder.empty()) {

            std::cout << "order ID: " << canMforder << "\n";
        } else {

            std::cout << "failed to cancel order"
                      << "\n";
        };*/
        //! [cancel a mf order]

        //! [place mf sip order]
        /*std::pair<std::string, std::string> sipRes = Kite.placeMFSIP("INF174K01LS2", 10000, -1, "monthly", 50000, 1);
        if (!sipRes.first.empty() && !sipRes.second.empty()) {

            std::cout << "order ID: " << sipRes.first << " SIP ID: " << sipRes.second << "\n";
        } else {

            std::cout << "failed to place SIP order\n";
        };*/
        //! [place mf sip order]

        //! [modify mf sip order]
        // Kite.modifyMFSIP("555299391983723", 5000, "paused", 10, "monthly", 5);
        //! [modify mf sip order]

        //! [get sip info]
        /*kc::MFSIP sip = Kite.getSIP("555299391983723");
        std::cout << "SIP ID: " << sip.ID << " status:" << sip.status << "\n";*/
        //! [get sip info]

        //! [cancel mf sip]
        /*std::string canSipID = Kite.cancelMFSIP("823098377587704");
        if (!canSipID.empty()) {

            std::cout << "cancelled SIP\n";
        } else {

            std::cout << "failed to cancel SIP\n";
        };*/
        //! [cancel mf sip]

        //! [get sips]
        /*std::vector<kc::MFSIP> sips = Kite.getSIPs();
        for (const auto& sip : sips) { std::cout << "ID: " << sip.ID << " status: " << sip.status << "\n"; };*/
        //! [get sips]

        //! [get mf holdings]
        /*std::vector<kc::MFHolding> mfholdings = Kite.getMFHoldings();
        for (const auto& i : mfholdings) { std::cout << "fund: " << i.fund << " last price: " << i.lastPrice << "\n";
        };*/
        //! [get mf holdings]

        //! [get instruments]
        /*std::vector<kc::instrument> inst = Kite.getInstruments();
        for (auto const& i : inst) { std::cout << "symbol: " << i.tradingsymbol << " token: " << i.instrumentToken <<
        "\n"; };*/
        //! [get instruments]

        //! [get mf instruments]
        /*std::vector<kc::MFInstrument> inst = Kite.getMFInstruments();
        for (auto const& i : inst) { std::cout << "symbol: " << i.tradingsymbol << " last price: " << i.lastPrice <<
        "\n"; };*/

        //! [get mf instruments]

        //! [get order margins]
        /*kc::orderMarginsParams params;
        params.exchange = "NSE";
        params.tradingsymbol = "INFY";
        params.orderType = "MARKET";
        params.transactionType = "BUY";
        params.variety = "regular";
        params.product = "CNC";
        params.quantity = 1;
        params.price = 0;
        params.triggerPrice = 0;

        std::vector<kc::orderMargins> ordMargins = Kite.getOrderMargins({ params });

        std::cout << "trading symbol: " << ordMargins[0].tradingSymbol << " total: " << ordMargins[0].total << "\n";*/
        //! [get order margins]

        //! [dealing with kitepp exceptions]
    } catch (kc::kiteppException& e) {
        std::cerr << e.what() << ", " << e.code() << ", " << e.message() << "\n";
    } catch (kc::libException& e) {
        std::cerr << e.what() << "\n";
    }
    //! [dealing with kitepp exceptions]

    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    };

    return 0;
}
