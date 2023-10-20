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

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

//! [adding kitepp header]
#include "kitepp.hpp"
//! [adding kitepp header]

namespace kc = kiteconnect;

int main() {
    try {

        //! [initializing kite]
        // kc::kite Kite(std::getenv("KITE_API_KEY"));
        //! [initializing kite]

        //! [obtaining login url]
        // std::cout << "login URL: " << Kite.loginURL() << '\n';
        // std::cout << "login with this URL and obtain the request token\n";
        //! [obtaining login url]

        //! [obtaining access token]
        // std::string apiSecret = std::getenv("KITE_API_SECRET");
        // std::string reqToken;
        // std::cout << "enter obtained request token: ";
        // std::cin >> reqToken;

        // std::string accessToken =
        //     Kite.generateSession(reqToken, apiSecret).tokens.accessToken;
        // std::cout << "access token: " << accessToken << "\n";
        //! [obtaining access token]

        //! [settting access token]
        // Kite.setAccessToken(accessToken);
        // Kite.setAccessToken(std::getenv("KITE_ACCESS_TOKEN"));
        //! [settting access token]

        //! [invalidate session]
        // if (Kite.invalidateSession()) {
        //     std::cout << "invalidated session\n";
        // } else {
        //     std::cout << "failed to invalid session\n";
        // }
        //! [invalidate session]

        //! [get user profile]
        // kc::userProfile profile = Kite.profile();
        // std::cout << "name: " << profile.userName << '\n';
        // std::cout << "email: " << profile.email << '\n';
        //! [get user profile]

        //! [get margins]
        // kc::allMargins Margins = Kite.getMargins();
        // std::cout << "available cash for equity segment is "
        //           << Margins.equity.available.cash << "\n";

        // kc::margins eqMargins = Kite.getMargins("equity");
        // std::cout << "adhoc margin for equity segment is "
        //           << eqMargins.available.adHocMargin << "\n";
        //! [get margins]

        //! [placing an order]
        // std::string orderId = Kite.placeOrder(kc::placeOrderParams()
        //                                           .Symbol("IOB")
        //                                           .Exchange("NSE")
        //                                           .TransactionType("BUY")
        //                                           .OrderType("MARKET")
        //                                           .Quantity(1)
        //                                           .Product("MIS")
        //                                           .Variety("amo")
        //                                           .Validity("DAY"));
        // if (!orderId.empty()) {
        //     std::cout << "order ID: " << orderId << "\n";
        // } else {
        //     std::cout << "failed to place order\n";
        // };
        //! [placing an order]

        //! [modifying an order]
        // std::string modifiedOrderId =
        //     Kite.modifyOrder(kc::modifyOrderParams()
        //                          .OrderId("220929004253728")
        //                          .OrderType("MARKET")
        //                          .Quantity(11)
        //                          .Variety("amo"));
        // if (!modifiedOrderId.empty()) {
        //     std::cout << "order ID: " << modifiedOrderId << "\n";
        // } else {
        //     std::cout << "failed to modify order\n";
        // };
        //! [modifying an order]

        //! [cancelling an order]
        // std::string cancelldOrderId =
        //     Kite.cancelOrder("amo", "220929004253728");
        // if (!cancelldOrderId.empty()) {
        //     std::cout << "order ID: " << cancelldOrderId << "\n";
        // } else {
        //     std::cout << "failed to cancel order\n";
        // };
        //! [cancelling an order]

        //! [get orders]
        // std::vector<kc::order> Orders = Kite.orders();
        // for (const auto& i : Orders) {
        //     std::cout << "order ID: " << i.orderID << " status: " << i.status
        //   << "\n";
        // }
        //! [get orders]

        //! [get order history]
        // std::vector<kc::order> OrderHistory =
        //     Kite.orderHistory("220929004253728");
        // for (const auto& i : OrderHistory) {
        //     std::cout << "status: " << i.status
        //               << " filled quantity: " << i.filledQuantity << '\n';
        // };
        //! [get order history]

        //! [get trades]
        // std::vector<kc::trade> Trades = Kite.trades();
        // for (const auto& i : Trades) {
        //     std::cout << "trade ID: " << i.tradeID
        //               << " filled at: " << i.fillTimestamp << '\n';
        // };
        //! [get trades]

        //! [get order trades]
        // std::vector<kc::trade> Trades = Kite.orderTrades("220929004253728");
        // for (const auto& i : Trades) {
        //     std::cout << "trade ID: " << i.tradeID
        //               << "filled at : " << i.fillTimestamp << '\n';
        // };
        //! [get order trades]

        //! [placing a gtt]
        // int triggerId =
        //     Kite.placeGtt(kc::placeGttParams()
        //                       .TriggerType("single")
        //                       .Symbol("NESTLEIND")
        //                       .Exchange("NSE")
        //                       .LastPrice(18979.15)
        //                       .TriggerValues({ 19200.65 })
        //                       .GttParamsList({ kc::gttParams()
        //                                            .Quantity(1)
        //                                            .Price(19000.75)
        //                                            .TransactionType("BUY")
        //                                            .OrderType("LIMIT")
        //                                            .Product("MIS") }));

        // std::cout << "trigger ID: " << triggerId << '\n';
        //! [placing a gtt]

        //! [modifying a gtt]
        // int modifiedTriggerId =
        //     Kite.modifyGtt(kc::modifyGttParams()
        //                        .TriggerId(119865602)
        //                        .TriggerType("single")
        //                        .Symbol("NESTLEIND")
        //                        .Exchange("NSE")
        //                        .LastPrice(18979.15)
        //                        .TriggerValues({ 19200.65 })
        //                        .GttParamsList({ kc::gttParams()
        //                                             .Quantity(10)
        //                                             .Price(19000.85)
        //                                             .TransactionType("BUY")
        //                                             .OrderType("LIMIT")
        //                                             .Product("MIS") }));

        // std::cout << "trigger ID: " << modifiedTriggerId << '\n';
        //! [modifying a gtt]

        //! [get gtts]
        // std::vector<kc::GTT> triggers = Kite.triggers();
        // for (const auto& gtt : triggers) {
        //     std::cout << "ID: " << gtt.ID
        //               << "
        //         created at : " << gtt.createdAt << '\n';
        // };
        //! [get gtts]

        //! [get gtt info]
        // kc::GTT gtt = Kite.getGtt(119865602);
        // std::cout << "created at: " << gtt.createdAt
        //           << " first trigger value : " <<
        //           gtt.condition.triggerValues[0]
        //           << '\n';
        //! [get gtt info]

        //! [delete a gtt]
        // int triggerId = Kite.deleteGtt(119865602);
        // std::cout << "trigger ID: " << triggerId << '\n';
        //! [delete a gtt]

        //! [get holdings]
        // std::vector<kc::holding> holdings = Kite.holdings();
        // for (const auto& i : holdings) {
        //     std::cout << "trading symbol: " << i.tradingsymbol << " average
        //     price: " << i.averagePrice << '\n';
        // };
        //! [get holdings]

        //! [get positions]
        // kc::positions Positions = Kite.getPositions();
        // if (!Positions.day.empty()) {
        //     std::cout << "tradingsymbol: " << Positions.day[0].tradingsymbol
        //               << " & quantity: " << Positions.day[0].quantity <<
        //               '\n';
        // };
        //! [get positions]

        //! [convert position]
        // if (Kite.convertPosition(kc::convertPositionParams()
        //                              .Symbol("BHEL")
        //                              .Exchange("NSE")
        //                              .TransactionType("BUY")
        //                              .Quantity(1)
        //                              .PositionType("day")
        //                              .OldProduct("MIS")
        //                              .NewProduct("CNC"))) {
        //     std::cout << "successfully converted the position\n";
        // } else {
        //     std::cout << "failed to convert the position\n";
        // };
        //! [convert position]

        //! [get quote]
        // std::unordered_map<std::string, kc::quote> quotes =
        //     Kite.getQuote({ "NSE:INFY", "NSE:NIFTY 50", "NSE:RELIANCE" });
        // std::cout << "NIFTY50 close: " << quotes["NSE:NIFTY 50"].OHLC.close
        // << "\n"; std::cout << "INFY close: " << quotes["NSE:INFY"].OHLC.close
        // << "\n";
        //! [get quote]

        //! [get ohlc]
        // std::unordered_map<std::string, kc::ohlcQuote> ohlcQuotes =
        //     Kite.getOhlc({ "NSE:INFY", "NSE:NIFTY 50", "NSE:RELIANCE" });
        // std::cout << "NIFTY50 close: " << ohlcQuotes["NSE:NIFTY
        // 50"].OHLC.close << '\n';
        //! [get ohlc]

        //! [get ltp]
        // std::unordered_map<std::string, kc::ltpQuote> ltpQuotes =
        //     Kite.getLtp({ "NSE:INFY", "NSE:NIFTY 50", "NSE:RELIANCE" });
        // std::cout << "RELIANCE LTP: " << ltpQuotes["NSE:RELIANCE"].lastPrice
        // << '\n';
        //! [get ltp]

        //! [get historical data]
        // std::vector<kc::historicalData> data =
        // Kite.getHistoricalData(kc::historicalDataParams()
        //                                                                   .InstrumentToken(5633)
        //                                                                   .From("2022-07-02
        //                                                                   09:30:00")
        //                                                                   .To("2022-08-10
        //                                                                   10:30:00")
        //                                                                   .Interval("minute"));
        // for (const auto& i : data) {
        //     std::cout << "datetime: " << i.datetime << " close: " << i.close
        //               << "\n";
        // };
        // ! [get historical data]

        //! [place mf order]
        // std::string mfOrderId = Kite.placeMfOrder(kc::placeMfOrderParams()
        //                                               .Symbol("INF174K01LS2")
        //                                               .TransactionType("BUY")
        //                                               .Amount(10000));
        // if (!mfOrderId.empty()) {
        //     std::cout << "order ID: " << mfOrderId << '\n';
        // } else {
        //     std::cout << "failed to place order\n";
        // };
        //! [place mf order]

        //! [get mf order info]
        // kc::mfOrder mforder =
        //     Kite.getMfOrder("51ac086a-0e39-42b5-a725-157507614d79");
        // std::cout << "status: " << mforder.status << '\n';
        //! [get mf order info]

        //! [get mf orders]
        // std::vector<kc::mfOrder> mforders = Kite.getMfOrders();
        // for (const auto& i : mforders) {
        //     std::cout << "order ID: " << i.orderID << " status: " << i.status
        //               << '\n';
        // };
        //! [get mf orders]

        //! [cancel a mf order]
        // std::string mfOrderId =
        //     Kite.cancelMfOrder("51ac086a-0e39-42b5-a725-157507614d79");
        // if (!mfOrderId.empty()) {
        //     std::cout << "order ID: " << mfOrderId << '\n';
        // } else {
        //     std::cout << "failed to cancel order\n";
        // };
        //! [cancel a mf order]

        //! [place mf sip order]
        // auto sipRes = Kite.placeMfSip(kc::placeMfSipParams()
        //                                   .Symbol("INF174K01LS2")
        //                                   .Amount(1000)
        //                                   .Frequency("monthly")
        //                                   .InitialAmount(1000)
        //                                   .InstallmentDay(2));
        // if (!sipRes.sipId.empty() && !sipRes.orderId.empty()) {
        //     std::cout << "order ID: " << sipRes.orderId
        //               << " SIP ID: " << sipRes.sipId << '\n';
        // } else {
        //     std::cout << "failed to place SIP order\n";
        // };
        //! [place mf sip order]

        //! [modify mf sip order]
        // std::string mfOrderId = Kite.modifyMfSip(kc::modifyMfSipParams()
        //                                              .SipId("555299391983723")
        //                                              .Amount(5000)
        //                                              .Status("paused")
        //                                              .InstallmentDay(10)
        //                                              .Frequency("monthly")
        //                                              .Installments(5));
        // if (!mfOrderId.empty()) {
        //     std::cout << "order ID: " << mfOrderId << '\n';
        // } else {
        //     std::cout << "failed to modify order\n";
        // };
        //! [modify mf sip order]

        //! [get sip info]
        // kc::mfSip sip = Kite.getSip("555299391983723");
        // std::cout << "SIP ID: " << sip.ID << " status:" << sip.status <<
        // '\n';
        //! [get sip info]

        //! [cancel mf sip]
        // std::string sipId = Kite.cancelMfSip("823098377587704");
        // if (!sipId.empty()) {
        //     std::cout << "cancelled SIP\n";
        // } else {
        //     std::cout << "failed to cancel SIP\n";
        // };
        //! [cancel mf sip]

        //! [get sips]
        // std::vector<kc::mfSip> sips = Kite.getSips();
        // for (const auto& sip : sips) {
        //     std::cout << "ID: " << sip.ID
        //               << "
        //         status : " << sip.status << "\n "; };
        //! [get sips]

        //! [get mf holdings]
        // std::vector<kc::mfHolding> mfholdings = Kite.getMfHoldings();
        // for (const auto& i : mfholdings) {
        //     std::cout << "fund: " << i.fund << " last price: " << i.lastPrice
        //               << '\n';
        // };
        //! [get mf holdings]

        //! [get instruments]
        // std::vector<kc::instrument> inst = Kite.getInstruments();
        // for (auto const& i : inst) {
        //     std::cout << "symbol: " << i.tradingsymbol
        //               << " token: " << i.instrumentToken << '\n';
        // };
        //! [get instruments]

        //! [get mf instruments]
        // std::vector<kc::mfInstrument> inst = Kite.getMfInstruments();
        // for (auto const& i : inst) {
        //     std::cout << "symbol: " << i.tradingsymbol
        //               << " last price: " << i.lastPrice << '\n';
        // };
        //! [get mf instruments]

        //! [get order margins]
        // std::vector<kc::orderMargins> ordMargins =
        //     Kite.getOrderMargins({ kc::marginsParams()
        //                                .Exchange("NSE")
        //                                .Tradingsymbol("INFY")
        //                                .OrderType("MARKET")
        //                                .TransactionType("BUY")
        //                                .Variety("regular")
        //                                .Product("CNC")
        //                                .Quantity(1)
        //                                .Price(0)
        //                                .TriggerPrice(0) });

        // std::cout << "trading symbol: " << ordMargins[0].tradingSymbol
        //           << " total: " << ordMargins[0].total << "\n";
        //! [get order margins]

        //! [get basket margins]
        // kc::basketMargins basketMargins =
        //     Kite.getBasketMargins({ {
        //                                 kc::marginsParams()
        //                                     .Exchange("NSE")
        //                                     .Tradingsymbol("INFY")
        //                                     .OrderType("MARKET")
        //                                     .TransactionType("BUY")
        //                                     .Variety("regular")
        //                                     .Product("CNC")
        //                                     .Quantity(1)
        //                                     .Price(0)
        //                                     .TriggerPrice(0),
        //                             },
        //                               {
        //                                   kc::marginsParams()
        //                                       .Exchange("NSE")
        //                                       .Tradingsymbol("HDFCBANK")
        //                                       .OrderType("SL")
        //                                       .TransactionType("BUY")
        //                                       .Variety("regular")
        //                                       .Product("MIS")
        //                                       .Quantity(100)
        //                                       .Price(1510)
        //                                       .TriggerPrice(1500),
        //                               } },
        //         true);

        // std::cout << "final total: " << basketMargins.final.total << "\n";
        //! [get basket margins]

        //! [dealing with kitepp exceptions]
    } catch (kc::kiteppException& e) {
        std::cerr << e.what() << ", " << e.code() << ", " << e.message()
                  << '\n';
    } catch (kc::libException& e) {
        std::cerr << e.what() << '\n';
    }
    //! [dealing with kitepp exceptions]

    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    };

    return 0;
}
