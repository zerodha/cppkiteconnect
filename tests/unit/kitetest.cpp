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

#include <fstream>
#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ios>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "kitepp.hpp"
#include "kitepp/responses.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using std::string;
using ::testing::_;
namespace rj = rapidjson;
namespace kc = kiteconnect;

// Mocks

class mockKite : public kc::kite {

  public:
    mockKite(): kite("apiKey123") {};

    MOCK_METHOD(void, _sendReq,
        (rj::Document & data, const kc::_methods& mtd, const string& endpoint,
            (const std::vector<std::pair<string, string>>) &bodyParams, bool isJson),
        (override));

    MOCK_METHOD(string, _sendInstrumentsReq, (const string& endpoint), (override));
};

// Constructor tests

TEST(kiteTest, constructorTest) {

    mockKite Kite;
    const string APIKey_expected = "apiKey123";

    EXPECT_EQ(Kite.getAPIKey(), APIKey_expected);
};

// User tests

TEST(kiteTest, loginURLTest) {

    mockKite Kite;
    const string loginURL_expected = "https://kite.zerodha.com/connect/login?v=3&api_key=apiKey123";

    EXPECT_EQ(Kite.loginURL(), loginURL_expected);
};

TEST(kiteTest, generateSessionTest) {

    std::ifstream jsonFile("../../tests/mock_custom/generate_session.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    // We need to pass an lambda instead of directly modifying the argument because rapidjson::Document objcect's copy
    // constructor is deleted and setArgumentRefree<> copies the object passed internally
    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::POST, "/session/token", _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::userSession session = Kite.generateSession("arg1", "arg2");

    // Expected values
    EXPECT_EQ(session.profile.userID, "XX000");
    EXPECT_EQ(session.profile.userName, "Kite Connect");
    EXPECT_EQ(session.profile.userShortName, "Kite");
    EXPECT_EQ(session.profile.avatarURL, "");
    EXPECT_EQ(session.profile.userType, "investor");
    EXPECT_EQ(session.profile.email, "kite@kite.trade");
    EXPECT_EQ(session.profile.broker, "ZERODHA");
    EXPECT_THAT(session.profile.products, ::testing::ElementsAre("BO", "CNC", "CO", "MIS", "NRML"));
    EXPECT_THAT(session.profile.orderTypes, ::testing::ElementsAre("LIMIT", "MARKET", "SL", "SL-M"));
    EXPECT_THAT(session.profile.exchanges, ::testing::ElementsAre("MCX", "BSE", "NSE", "BFO", "NFO", "CDS"));

    EXPECT_EQ(session.tokens.userID, "XX000");
    EXPECT_EQ(session.tokens.accessToken, "yyyyyy");
    EXPECT_EQ(session.tokens.refreshToken, "");

    EXPECT_EQ(session.apiKey, "xxxxxx");
    EXPECT_EQ(session.publicToken, "zzzzzz");
    EXPECT_EQ(session.loginTime, "2018-01-01 16:15:14");
};

TEST(kiteTest, invalidateSessionTest) {

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::DEL, _, _, _)).WillOnce(::testing::Return());

    Kite.invalidateSession();
};

TEST(kiteTest, profile) {

    std::ifstream jsonFile("../../tests/mock_responses/profile.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::userProfile profile = Kite.profile();

    // Expected values
    EXPECT_EQ(profile.userID, "AB1234");
    EXPECT_EQ(profile.userName, "AxAx Bxx");
    EXPECT_EQ(profile.userShortName, "abc");
    EXPECT_EQ(profile.avatarURL, "");
    EXPECT_EQ(profile.userType, "investor");
    EXPECT_EQ(profile.email, "xxxyyy@gmail.com");
    EXPECT_EQ(profile.broker, "ZERODHA");
    EXPECT_THAT(profile.products, ::testing::ElementsAre("BO", "CNC", "CO", "MIS", "NRML"));
    EXPECT_THAT(profile.orderTypes, ::testing::ElementsAre("LIMIT", "MARKET", "SL", "SL-M"));
    EXPECT_THAT(profile.exchanges, ::testing::ElementsAre("BSE", "BFO", "NFO", "MCX", "CDS", "NSE"));
    EXPECT_EQ(profile.meta.dematConsent, "physical");
};

TEST(kiteTest, getMarginsTest1) {

    std::ifstream jsonFile("../../tests/mock_responses/margins.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::allMargins margins = Kite.getMargins();

    // Expected values
    EXPECT_EQ(margins.equity.enabled, true);
    EXPECT_DOUBLE_EQ(margins.equity.net, 99725.050000000017);

    EXPECT_DOUBLE_EQ(margins.equity.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.equity.available.cash, 245431.60000000001);
    EXPECT_DOUBLE_EQ(margins.equity.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.equity.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.equity.used.debits, 145706.54999999999);
    EXPECT_DOUBLE_EQ(margins.equity.used.exposure, 38981.25);
    EXPECT_DOUBLE_EQ(margins.equity.used.M2MRealised, 761.70000000000005);
    EXPECT_DOUBLE_EQ(margins.equity.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.span, 101989);
    EXPECT_DOUBLE_EQ(margins.equity.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.turnover, 0);

    EXPECT_EQ(margins.commodity.enabled, true);
    EXPECT_DOUBLE_EQ(margins.commodity.net, 100661.7);

    EXPECT_DOUBLE_EQ(margins.commodity.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.available.cash, 100661.7);
    EXPECT_DOUBLE_EQ(margins.commodity.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.commodity.used.debits, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.exposure, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.M2MRealised, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.span, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.turnover, 0);
}

TEST(kiteTest, getMarginsTest2) {

    std::ifstream jsonFile("../../tests/mock_responses/margins_equity.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::margins margins = Kite.getMargins("equity");

    // Expected values
    EXPECT_EQ(margins.enabled, true);
    EXPECT_DOUBLE_EQ(margins.net, 99725.050000000017);

    EXPECT_DOUBLE_EQ(margins.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.available.cash, 245431.60000000001);
    EXPECT_DOUBLE_EQ(margins.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.used.debits, 145706.54999999999);
    EXPECT_DOUBLE_EQ(margins.used.exposure, 38981.25);
    EXPECT_DOUBLE_EQ(margins.used.M2MRealised, 761.70000000000005);
    EXPECT_DOUBLE_EQ(margins.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.used.span, 101989);
    EXPECT_DOUBLE_EQ(margins.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.used.turnover, 0);
}

// Orders tests

TEST(kiteTest, placeOrderTest) {
    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.placeOrder(kc::placeOrderParams()
                                         .Quantity(10)
                                         .Variety("regular")
                                         .Exchange("NSE")
                                         .Symbol("TCS")
                                         .TransactionType("BUY")
                                         .Product("NRML")
                                         .OrderType("MARKET")
                                         .Validity("day")
                                         .Tag("order1"));

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, modifyOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    // clang-format off
    string orderID = Kite.modifyOrder(kc::modifyOrderParams()
                                        .Variety("regular")
                                        .OrderId("1111111000101010")
                                        .Quantity(69));
    // clang-format on

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, cancelOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.cancelOrder("arg1", "arg2");

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, exitOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.exitOrder("arg1", "arg2");

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, ordersTest) {

    std::ifstream jsonFile("../../tests/mock_responses/orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::order> Orders = Kite.orders();

    // Expected values
    ASSERT_THAT(Orders.size(), 5);

    kc::order order1 = Orders[0];
    EXPECT_EQ(order1.accountID, "");
    EXPECT_EQ(order1.placedBy, "XXXXXX");
    EXPECT_EQ(order1.orderID, "100000000000000");
    EXPECT_EQ(order1.exchangeOrderID, "200000000000000");
    EXPECT_EQ(order1.parentOrderID, "");
    EXPECT_EQ(order1.status, "CANCELLED");
    EXPECT_EQ(order1.statusMessage, "");
    EXPECT_EQ(order1.orderTimestamp, "2021-05-31 09:18:57");
    EXPECT_EQ(order1.exchangeUpdateTimestamp, "2021-05-31 09:18:58");
    EXPECT_EQ(order1.exchangeTimestamp, "2021-05-31 09:15:38");
    EXPECT_EQ(order1.rejectedBy, "");
    EXPECT_EQ(order1.variety, "regular");
    EXPECT_EQ(order1.exchange, "CDS");
    EXPECT_EQ(order1.tradingSymbol, "USDINR21JUNFUT");
    EXPECT_EQ(order1.instrumentToken, 412675);
    EXPECT_EQ(order1.orderType, "LIMIT");
    EXPECT_EQ(order1.transactionType, "BUY");
    EXPECT_EQ(order1.validity, "DAY");
    EXPECT_EQ(order1.product, "NRML");
    EXPECT_EQ(order1.quantity, 1);
    EXPECT_EQ(order1.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order1.price, 72);
    EXPECT_DOUBLE_EQ(order1.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order1.averagePrice, 0);
    EXPECT_EQ(order1.filledQuantity, 0);
    EXPECT_EQ(order1.pendingQuantity, 1);
    EXPECT_EQ(order1.cancelledQuantity, 1);
}

TEST(kiteTest, orderHistoryTest) {

    std::ifstream jsonFile("../../tests/mock_responses/orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::order> Orders = Kite.orderHistory("arg1");

    // Expected values
    ASSERT_THAT(Orders.size(), 5);

    kc::order order1 = Orders[0];
    EXPECT_EQ(order1.accountID, "");
    EXPECT_EQ(order1.placedBy, "XXXXXX");
    EXPECT_EQ(order1.orderID, "100000000000000");
    EXPECT_EQ(order1.exchangeOrderID, "200000000000000");
    EXPECT_EQ(order1.parentOrderID, "");
    EXPECT_EQ(order1.status, "CANCELLED");
    EXPECT_EQ(order1.statusMessage, "");
    EXPECT_EQ(order1.orderTimestamp, "2021-05-31 09:18:57");
    EXPECT_EQ(order1.exchangeUpdateTimestamp, "2021-05-31 09:18:58");
    EXPECT_EQ(order1.exchangeTimestamp, "2021-05-31 09:15:38");
    EXPECT_EQ(order1.rejectedBy, "");
    EXPECT_EQ(order1.variety, "regular");
    EXPECT_EQ(order1.exchange, "CDS");
    EXPECT_EQ(order1.tradingSymbol, "USDINR21JUNFUT");
    EXPECT_EQ(order1.instrumentToken, 412675);
    EXPECT_EQ(order1.orderType, "LIMIT");
    EXPECT_EQ(order1.transactionType, "BUY");
    EXPECT_EQ(order1.validity, "DAY");
    EXPECT_EQ(order1.product, "NRML");
    EXPECT_EQ(order1.quantity, 1);
    EXPECT_EQ(order1.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order1.price, 72);
    EXPECT_DOUBLE_EQ(order1.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order1.averagePrice, 0);
    EXPECT_EQ(order1.filledQuantity, 0);
    EXPECT_EQ(order1.pendingQuantity, 1);
    EXPECT_EQ(order1.cancelledQuantity, 1);
}

TEST(kiteTest, tradesTest) {

    std::ifstream jsonFile("../../tests/mock_responses/trades.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::trade> Trades = Kite.trades();

    // Expected values
    ASSERT_EQ(Trades.size(), 4);

    kc::trade trade1 = Trades[0];
    EXPECT_DOUBLE_EQ(trade1.averagePrice, 420.64999999999998);
    EXPECT_EQ(trade1.exchange, "NSE");
    EXPECT_EQ(trade1.exchangeOrderID, "300000000000000");
    EXPECT_EQ(trade1.exchangeTimestamp, "2021-05-31 09:16:39");
    EXPECT_EQ(trade1.InstrumentToken, 779521);
    EXPECT_EQ(trade1.orderID, "200000000000000");
    EXPECT_EQ(trade1.product, "CNC");
    EXPECT_DOUBLE_EQ(trade1.quantity, 1);
    EXPECT_EQ(trade1.tradeID, "10000000");
    EXPECT_EQ(trade1.tradingSymbol, "SBIN");
    EXPECT_EQ(trade1.transactionType, "BUY");
}

TEST(kiteTest, orderTradesTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_trades.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::trade> Trades = Kite.orderTrades("arg1");

    // Expected values
    ASSERT_EQ(Trades.size(), 1);

    kc::trade trade1 = Trades[0];
    EXPECT_DOUBLE_EQ(trade1.averagePrice, 4852);
    EXPECT_EQ(trade1.exchange, "MCX");
    EXPECT_EQ(trade1.exchangeOrderID, "300000000000000");
    EXPECT_EQ(trade1.exchangeTimestamp, "2021-05-31 16:00:36");
    EXPECT_EQ(trade1.InstrumentToken, 58424839);
    EXPECT_EQ(trade1.orderID, "200000000000000");
    EXPECT_EQ(trade1.product, "NRML");
    EXPECT_DOUBLE_EQ(trade1.quantity, 1);
    EXPECT_EQ(trade1.tradeID, "10000000");
    EXPECT_EQ(trade1.tradingSymbol, "GOLDPETAL21JUNFUT");
    EXPECT_EQ(trade1.transactionType, "BUY");
}

// GTT tests

TEST(kiteTest, placeGttTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_place_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));
    // clang-format off
    int orderID = Kite.placeGtt(kc::placeGttParams()
                                    .LastPrice(100)
                                    .TriggerType("single")
                                    .Symbol("TCS")
                                    .Exchange("NSE")
                                    .TriggerValues({ 110.2 })
                                    .GttParamsList({
                                        kc::gttParams()
                                        .Quantity(416)
                                        .Price(111)
                                        .TransactionType("BUY")
                                        .OrderType("LIMIT")
                                        .Product("CNC")
                                    }));
    // clang-format on

    // Expected value
    EXPECT_EQ(orderID, 123);
}

TEST(kiteTest, getGTTsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_get_orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::GTT> Orders = Kite.getGtts();

    // Exptected values
    ASSERT_EQ(Orders.size(), 2);

    kc::GTT order1 = Orders[0];
    EXPECT_EQ(order1.ID, 112127);
    EXPECT_EQ(order1.userID, "XX0000");
    EXPECT_EQ(order1.type, "single");
    EXPECT_EQ(order1.createdAt, "2019-09-12 13:25:16");
    EXPECT_EQ(order1.updatedAt, "2019-09-12 13:25:16");
    EXPECT_EQ(order1.expiresAt, "2020-09-12 13:25:16");
    EXPECT_EQ(order1.status, "active");
    EXPECT_EQ(order1.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order1.condition.lastPrice, 798);
    EXPECT_EQ(order1.condition.tradingsymbol, "INFY");
    EXPECT_DOUBLE_EQ(order1.condition.triggerValues[0], 702);
    EXPECT_EQ(order1.orders[0].exchange, "NSE");
    EXPECT_EQ(order1.orders[0].tradingSymbol, "INFY");
    EXPECT_EQ(order1.orders[0].product, "CNC");
    EXPECT_EQ(order1.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order1.orders[0].transactionType, "BUY");
    EXPECT_EQ(order1.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order1.orders[0].price, 702.5);

    kc::GTT order2 = Orders[1];
    EXPECT_EQ(order2.ID, 105099);
    EXPECT_EQ(order2.userID, "XX0000");
    EXPECT_EQ(order2.type, "two-leg");
    EXPECT_EQ(order2.createdAt, "2019-09-09 15:13:22");
    EXPECT_EQ(order2.updatedAt, "2019-09-09 15:15:08");
    EXPECT_EQ(order2.expiresAt, "2020-01-01 12:00:00");
    EXPECT_EQ(order2.status, "triggered");
    EXPECT_EQ(order2.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order2.condition.lastPrice, 102.6);
    EXPECT_EQ(order2.condition.tradingsymbol, "RAIN");
    EXPECT_THAT(order2.condition.triggerValues, ::testing::ElementsAre(102.0, 103.7));
    EXPECT_EQ(order2.orders[0].tradingSymbol, "RAIN");
    EXPECT_EQ(order2.orders[0].product, "CNC");
    EXPECT_EQ(order2.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order2.orders[0].transactionType, "SELL");
    EXPECT_EQ(order2.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order2.orders[0].price, 1);
    EXPECT_EQ(order2.orders[1].tradingSymbol, "RAIN");
    EXPECT_EQ(order2.orders[1].product, "CNC");
    EXPECT_EQ(order2.orders[1].orderType, "LIMIT");
    EXPECT_EQ(order2.orders[1].transactionType, "SELL");
    EXPECT_EQ(order2.orders[1].quantity, 1);
    EXPECT_DOUBLE_EQ(order2.orders[1].price, 1);
}

TEST(kiteTest, getGTTTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_get_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::GTT order = Kite.getGtt(0);

    // Expected values
    EXPECT_EQ(order.ID, 123);
    EXPECT_EQ(order.userID, "XX0000");
    EXPECT_EQ(order.type, "two-leg");
    EXPECT_EQ(order.createdAt, "2019-09-09 15:13:22");
    EXPECT_EQ(order.updatedAt, "2019-09-09 15:15:08");
    EXPECT_EQ(order.expiresAt, "2020-01-01 12:00:00");
    EXPECT_EQ(order.status, "triggered");
    EXPECT_EQ(order.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order.condition.lastPrice, 102.6);
    EXPECT_EQ(order.condition.tradingsymbol, "RAIN");
    EXPECT_THAT(order.condition.triggerValues, ::testing::ElementsAre(102.0, 103.7));
    EXPECT_EQ(order.orders[0].tradingSymbol, "RAIN");
    EXPECT_EQ(order.orders[0].product, "CNC");
    EXPECT_EQ(order.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order.orders[0].transactionType, "SELL");
    EXPECT_EQ(order.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order.orders[0].price, 1);
}

TEST(kiteTest, modifyGttTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_modify_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    // clang-format off
    int orderID = Kite.modifyGtt(
        kc::modifyGttParams()
            .TriggerId(11111)
            .LastPrice(100)
            .TriggerType("single")
            .Symbol("TCS")
            .Exchange("NSE")
            .TriggerValues({ 111.2 })
            .GttParamsList(
                { kc::gttParams()
                .Quantity(4116)
                .Price(110)
                .TransactionType("BUY")
                .OrderType("LIMIT")
                .Product("CNC") }
            ));
    // clang-format on

    // Expected values
    EXPECT_EQ(orderID, 123);
}

TEST(kiteTest, deleteGTTTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_delete_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    int orderID = Kite.deleteGtt(0);

    // Expected values
    EXPECT_EQ(orderID, 123);
}

// Market tests

TEST(kiteTest, holdingsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/holdings.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::holding> Holdings = Kite.holdings();

    // Expected values
    ASSERT_EQ(Holdings.size(), 19);

    kc::holding holding1 = Holdings[0];
    EXPECT_EQ(holding1.tradingsymbol, "AXTEL");
    EXPECT_EQ(holding1.exchange, "BSE");
    EXPECT_EQ(holding1.instrumentToken, 134105604);
    EXPECT_EQ(holding1.ISIN, "INE767C01012");
    EXPECT_EQ(holding1.product, "CNC");
    EXPECT_DOUBLE_EQ(holding1.price, 0);
    EXPECT_EQ(holding1.quantity, 1);
    EXPECT_EQ(holding1.t1Quantity, 0);
    EXPECT_EQ(holding1.realisedQuantity, 1);
    EXPECT_EQ(holding1.collateralQuantity, 0);
    EXPECT_EQ(holding1.collateralType, "");
    EXPECT_DOUBLE_EQ(holding1.averagePrice, 261.5);
    EXPECT_DOUBLE_EQ(holding1.lastPrice, 313);
    EXPECT_DOUBLE_EQ(holding1.closePrice, 308.8);
    EXPECT_DOUBLE_EQ(holding1.PnL, 51.5);
    EXPECT_DOUBLE_EQ(holding1.dayChange, 4.1999999999999886);
    EXPECT_DOUBLE_EQ(holding1.dayChangePercentage, 1.3601036269430016);
}

TEST(kiteTest, getPositionsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/positions.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::positions Positions = Kite.getPositions();

    // Expected values
    ASSERT_EQ(Positions.net.size(), 3);
    ASSERT_EQ(Positions.day.size(), 3);

    kc::position netPosition1 = Positions.net[0];
    EXPECT_EQ(netPosition1.tradingsymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(netPosition1.exchange, "MCX");
    EXPECT_EQ(netPosition1.instrumentToken, 53496327);
    EXPECT_EQ(netPosition1.product, "NRML");
    EXPECT_EQ(netPosition1.quantity, 1);
    EXPECT_EQ(netPosition1.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.multiplier, 1000);
    EXPECT_DOUBLE_EQ(netPosition1.averagePrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.closePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.lastPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.value, -161050);
    EXPECT_DOUBLE_EQ(netPosition1.PnL, 0);
    EXPECT_DOUBLE_EQ(netPosition1.M2M, 0);
    EXPECT_DOUBLE_EQ(netPosition1.unrealised, 0);
    EXPECT_DOUBLE_EQ(netPosition1.realised, 0);
    EXPECT_EQ(netPosition1.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition1.buyPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.buyValue, 161050);
    EXPECT_DOUBLE_EQ(netPosition1.buyM2MValue, 161050);
    EXPECT_EQ(netPosition1.sellQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellPrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellValue, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellM2MValue, 0);
    EXPECT_EQ(netPosition1.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition1.dayBuyPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.dayBuyValue, 161050);
    EXPECT_EQ(netPosition1.daySellQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.daySellPrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.daySellValue, 0);

    kc::position netPosition2 = Positions.net[1];
    EXPECT_EQ(netPosition2.tradingsymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(netPosition2.exchange, "MCX");
    EXPECT_EQ(netPosition2.instrumentToken, 53505799);
    EXPECT_EQ(netPosition2.product, "NRML");
    EXPECT_EQ(netPosition2.quantity, 0);
    EXPECT_EQ(netPosition2.overnightQuantity, 3);
    EXPECT_DOUBLE_EQ(netPosition2.multiplier, 1);
    EXPECT_DOUBLE_EQ(netPosition2.averagePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition2.closePrice, 23232);
    EXPECT_DOUBLE_EQ(netPosition2.lastPrice, 23355);
    EXPECT_DOUBLE_EQ(netPosition2.value, 801);
    EXPECT_DOUBLE_EQ(netPosition2.PnL, 801);
    EXPECT_DOUBLE_EQ(netPosition2.M2M, 276);
    EXPECT_DOUBLE_EQ(netPosition2.unrealised, 801);
    EXPECT_DOUBLE_EQ(netPosition2.realised, 0);
    EXPECT_EQ(netPosition2.buyQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.buyPrice, 23139.75);
    EXPECT_DOUBLE_EQ(netPosition2.buyValue, 92559);
    EXPECT_DOUBLE_EQ(netPosition2.buyM2MValue, 93084);
    EXPECT_EQ(netPosition2.sellQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.sellPrice, 23340);
    EXPECT_DOUBLE_EQ(netPosition2.sellValue, 93360);
    EXPECT_DOUBLE_EQ(netPosition2.sellM2MValue, 93360);
    EXPECT_EQ(netPosition2.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition2.dayBuyPrice, 23388);
    EXPECT_DOUBLE_EQ(netPosition2.dayBuyValue, 23388);
    EXPECT_EQ(netPosition2.daySellQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.daySellPrice, 23340);
    EXPECT_DOUBLE_EQ(netPosition2.daySellValue, 93360);

    kc::position netPosition3 = Positions.net[2];
    EXPECT_EQ(netPosition3.tradingsymbol, "SBIN");
    EXPECT_EQ(netPosition3.exchange, "NSE");
    EXPECT_EQ(netPosition3.instrumentToken, 779521);
    EXPECT_EQ(netPosition3.product, "CO");
    EXPECT_EQ(netPosition3.quantity, 0);
    EXPECT_EQ(netPosition3.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition3.multiplier, 1);
    EXPECT_DOUBLE_EQ(netPosition3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition3.closePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition3.lastPrice, 308.4);
    EXPECT_DOUBLE_EQ(netPosition3.value, -2);
    EXPECT_DOUBLE_EQ(netPosition3.PnL, -2);
    EXPECT_DOUBLE_EQ(netPosition3.M2M, -2);
    EXPECT_DOUBLE_EQ(netPosition3.unrealised, -2);
    EXPECT_DOUBLE_EQ(netPosition3.realised, 0);
    EXPECT_EQ(netPosition3.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.buyPrice, 311);
    EXPECT_DOUBLE_EQ(netPosition3.buyValue, 311);
    EXPECT_DOUBLE_EQ(netPosition3.buyM2MValue, 311);
    EXPECT_EQ(netPosition3.sellQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.sellPrice, 309);
    EXPECT_DOUBLE_EQ(netPosition3.sellValue, 309);
    EXPECT_DOUBLE_EQ(netPosition3.sellM2MValue, 309);
    EXPECT_EQ(netPosition3.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.dayBuyPrice, 311);
    EXPECT_DOUBLE_EQ(netPosition3.dayBuyValue, 311);
    EXPECT_EQ(netPosition3.daySellQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.daySellPrice, 309);
    EXPECT_DOUBLE_EQ(netPosition3.daySellValue, 309);

    kc::position dayPosition1 = Positions.day[0];
    EXPECT_EQ(dayPosition1.tradingsymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(dayPosition1.exchange, "MCX");
    EXPECT_EQ(dayPosition1.instrumentToken, 53505799);
    EXPECT_EQ(dayPosition1.product, "NRML");
    EXPECT_EQ(dayPosition1.quantity, -3);
    EXPECT_EQ(dayPosition1.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition1.multiplier, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.averagePrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.closePrice, 23232);
    EXPECT_DOUBLE_EQ(dayPosition1.lastPrice, 23355);
    EXPECT_DOUBLE_EQ(dayPosition1.value, 69972);
    EXPECT_DOUBLE_EQ(dayPosition1.PnL, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.M2M, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.unrealised, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.realised, 0);
    EXPECT_EQ(dayPosition1.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.buyPrice, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.buyValue, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.buyM2MValue, 23388);
    EXPECT_EQ(dayPosition1.sellQuantity, 4);
    EXPECT_DOUBLE_EQ(dayPosition1.sellPrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.sellValue, 93360);
    EXPECT_DOUBLE_EQ(dayPosition1.sellM2MValue, 93360);
    EXPECT_EQ(dayPosition1.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.dayBuyPrice, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.dayBuyValue, 23388);
    EXPECT_EQ(dayPosition1.daySellQuantity, 4);
    EXPECT_DOUBLE_EQ(dayPosition1.daySellPrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.daySellValue, 93360);

    kc::position dayPosition2 = Positions.day[1];
    EXPECT_EQ(dayPosition2.tradingsymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(dayPosition2.exchange, "MCX");
    EXPECT_EQ(dayPosition2.instrumentToken, 53496327);
    EXPECT_EQ(dayPosition2.product, "NRML");
    EXPECT_EQ(dayPosition2.quantity, 1);
    EXPECT_EQ(dayPosition2.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.multiplier, 1000);
    EXPECT_DOUBLE_EQ(dayPosition2.averagePrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.closePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.lastPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.value, -161050);
    EXPECT_DOUBLE_EQ(dayPosition2.PnL, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.M2M, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.unrealised, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.realised, 0);
    EXPECT_EQ(dayPosition2.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition2.buyPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.buyValue, 161050);
    EXPECT_DOUBLE_EQ(dayPosition2.buyM2MValue, 161050);
    EXPECT_EQ(dayPosition2.sellQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellPrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellValue, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellM2MValue, 0);
    EXPECT_EQ(dayPosition2.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition2.dayBuyPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.dayBuyValue, 161050);
    EXPECT_EQ(dayPosition2.daySellQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.daySellPrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.daySellValue, 0);

    kc::position dayPosition3 = Positions.day[2];
    EXPECT_EQ(dayPosition3.tradingsymbol, "SBIN");
    EXPECT_EQ(dayPosition3.exchange, "NSE");
    EXPECT_EQ(dayPosition3.instrumentToken, 779521);
    EXPECT_EQ(dayPosition3.product, "CO");
    EXPECT_EQ(dayPosition3.quantity, 0);
    EXPECT_EQ(dayPosition3.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.multiplier, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.closePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.lastPrice, 308.4);
    EXPECT_DOUBLE_EQ(dayPosition3.value, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.PnL, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.M2M, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.unrealised, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.realised, 0);
    EXPECT_EQ(dayPosition3.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.buyPrice, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.buyValue, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.buyM2MValue, 311);
    EXPECT_EQ(dayPosition3.sellQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.sellPrice, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.sellValue, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.sellM2MValue, 309);
    EXPECT_EQ(dayPosition3.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.dayBuyPrice, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.dayBuyValue, 311);
    EXPECT_EQ(dayPosition3.daySellQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.daySellPrice, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.daySellValue, 309);
}

TEST(kiteTest, convertPositionTest) {

    std::ifstream jsonFile("../../tests/mock_custom/convert_position.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    bool result = Kite.convertPosition(kc::convertPositionParams()
                                           .Quantity(11)
                                           .Exchange("BSE")
                                           .Symbol("INFY")
                                           .TransactionType("SELL")
                                           .PositionType("overnight")
                                           .OldProduct("NRML")
                                           .NewProduct("MIS"));

    // Expected values
    EXPECT_EQ(result, true);
}

TEST(kiteTest, getQuoteTest) {

    std::ifstream jsonFile("../../tests/mock_responses/quote.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::unordered_map<string, kc::quote> quotes = Kite.getQuote({});

    // Expected values
    ASSERT_NE(quotes.find("NSE:INFY"), quotes.end());
    kc::quote Quote = quotes["NSE:INFY"];

    EXPECT_EQ(Quote.instrumentToken, 408065);
    EXPECT_EQ(Quote.timestamp, "2021-06-08 15:45:56");
    EXPECT_DOUBLE_EQ(Quote.lastPrice, 1412.95);
    EXPECT_EQ(Quote.lastQuantity, 5);
    EXPECT_EQ(Quote.lastTradeTime, "2021-06-08 15:45:52");
    EXPECT_EQ(Quote.averagePrice, 1412.47);
    EXPECT_EQ(Quote.volume, 7360198);
    EXPECT_EQ(Quote.buyQuantity, 0);
    EXPECT_EQ(Quote.sellQuantity, 5191);

    EXPECT_EQ(Quote.OHLC.open, 1396);
    EXPECT_EQ(Quote.OHLC.high, 1421.75);
    EXPECT_EQ(Quote.OHLC.low, 1395.55);
    EXPECT_EQ(Quote.OHLC.close, 1389.65);

    EXPECT_DOUBLE_EQ(Quote.netChange, 0);
    EXPECT_DOUBLE_EQ(Quote.OI, 0);
    EXPECT_DOUBLE_EQ(Quote.OIDayHigh, 0);
    EXPECT_DOUBLE_EQ(Quote.OIDayLow, 0);

    ASSERT_EQ(Quote.marketDepth.buy.size(), 5);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.buy[0].price, 0);
    EXPECT_EQ(Quote.marketDepth.buy[0].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.buy[0].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.buy[1].price, 0);
    EXPECT_EQ(Quote.marketDepth.buy[1].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.buy[1].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.buy[2].price, 0);
    EXPECT_EQ(Quote.marketDepth.buy[2].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.buy[2].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.buy[3].price, 0);
    EXPECT_EQ(Quote.marketDepth.buy[3].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.buy[3].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.buy[4].price, 0);
    EXPECT_EQ(Quote.marketDepth.buy[4].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.buy[4].orders, 0);

    ASSERT_EQ(Quote.marketDepth.sell.size(), 5);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.sell[0].price, 1412.95);
    EXPECT_EQ(Quote.marketDepth.sell[0].quantity, 5191);
    EXPECT_EQ(Quote.marketDepth.sell[0].orders, 13);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.sell[1].price, 0);
    EXPECT_EQ(Quote.marketDepth.sell[1].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.sell[1].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.sell[2].price, 0);
    EXPECT_EQ(Quote.marketDepth.sell[2].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.sell[2].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.sell[3].price, 0);
    EXPECT_EQ(Quote.marketDepth.sell[3].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.sell[3].orders, 0);
    EXPECT_DOUBLE_EQ(Quote.marketDepth.sell[4].price, 0);
    EXPECT_EQ(Quote.marketDepth.sell[4].quantity, 0);
    EXPECT_EQ(Quote.marketDepth.sell[4].orders, 0);
}

TEST(kiteTest, getOHLCTest) {

    std::ifstream jsonFile("../../tests/mock_responses/ohlc.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::unordered_map<string, kc::ohlcQuote> quotes = Kite.getOhlc({});

    // Expected values
    ASSERT_NE(quotes.find("NSE:INFY"), quotes.end());
    kc::ohlcQuote Quote = quotes["NSE:INFY"];

    EXPECT_EQ(Quote.instrumentToken, 408065);
    EXPECT_DOUBLE_EQ(Quote.lastPrice, 1075);
    EXPECT_DOUBLE_EQ(Quote.OHLC.open, 1085.8);
    EXPECT_DOUBLE_EQ(Quote.OHLC.high, 1085.9);
    EXPECT_DOUBLE_EQ(Quote.OHLC.low, 1070.9);
    EXPECT_DOUBLE_EQ(Quote.OHLC.close, 1075.8);
}

TEST(kiteTest, getLTPTest) {

    std::ifstream jsonFile("../../tests/mock_responses/ltp.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::unordered_map<string, kc::ltpQuote> quotes = Kite.getLtp({});

    // Expected values
    ASSERT_NE(quotes.find("NSE:INFY"), quotes.end());
    kc::ltpQuote Quote = quotes["NSE:INFY"];

    EXPECT_EQ(Quote.instrumentToken, 408065);
    EXPECT_DOUBLE_EQ(Quote.lastPrice, 1074.35);
}

// Historical data tests

TEST(kiteTest, getHistoricalDataTest) {

    std::ifstream jsonFile("../../tests/mock_responses/historical_minute.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::historicalData> data = Kite.getHistoricalData(kc::historicalDataParams()
                                                                      .InstrumentToken(5633)
                                                                      .Interval("minute")
                                                                      .From("2017-12-15+09:15:00")
                                                                      .To("2017-12-15+09:20:00"));

    // Expected values
    ASSERT_EQ(data.size(), 3000);

    kc::historicalData data1 = data[0];
    EXPECT_EQ(data1.datetime, "2017-12-15T09:15:00+0530");
    EXPECT_DOUBLE_EQ(data1.open, 1704.5);
    EXPECT_DOUBLE_EQ(data1.high, 1705);
    EXPECT_DOUBLE_EQ(data1.low, 1699.25);
    EXPECT_DOUBLE_EQ(data1.close, 1702.8);
    EXPECT_EQ(data1.volume, 2499);
    EXPECT_DOUBLE_EQ(data1.OI, 0);

    kc::historicalData data2 = data[1];
    EXPECT_EQ(data2.datetime, "2017-12-15T09:16:00+0530");
    EXPECT_DOUBLE_EQ(data2.open, 1702);
    EXPECT_DOUBLE_EQ(data2.high, 1702);
    EXPECT_DOUBLE_EQ(data2.low, 1698.15);
    EXPECT_DOUBLE_EQ(data2.close, 1698.15);
    EXPECT_EQ(data2.volume, 1271);
    EXPECT_DOUBLE_EQ(data2.OI, 0);

    kc::historicalData data3 = data[2];
    EXPECT_EQ(data3.datetime, "2017-12-15T09:17:00+0530");
    EXPECT_DOUBLE_EQ(data3.open, 1698.15);
    EXPECT_DOUBLE_EQ(data3.high, 1700.25);
    EXPECT_DOUBLE_EQ(data3.low, 1698);
    EXPECT_DOUBLE_EQ(data3.close, 1699.25);
    EXPECT_EQ(data3.volume, 831);
    EXPECT_DOUBLE_EQ(data3.OI, 0);
}

// MF tests

TEST(kiteTest, placeMFOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    // clang-format off
    string orderID = Kite.placeMfOrder(kc::placeMfOrderParams()
                                        .Quantity(10)
                                        .Amount(100)
                                        .Symbol("INF174K01LS2")
                                        .Tag("sss")
                                        .TransactionType("BUY")
                                    );
    // clang-format on

    // Expected values
    EXPECT_EQ(orderID, "123457");
}

TEST(kiteTest, cancelMFOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.cancelMfOrder("arg1");

    // Expected values
    EXPECT_EQ(orderID, "123457");
}

TEST(kiteTest, getMFOrdersTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::mfOrder> orders = Kite.getMfOrders();

    // Expected values
    ASSERT_EQ(orders.size(), 5);

    kc::mfOrder order1 = orders[0];
    EXPECT_EQ(order1.orderID, "271989e0-a64e-4cf3-b4e4-afb8f38dd203");
    EXPECT_EQ(order1.exchangeOrderID, "254657127");
    EXPECT_EQ(order1.tradingsymbol, "INF179K01VY8");
    EXPECT_EQ(order1.status, "REJECTED");
    EXPECT_EQ(order1.statusMessage, "AMC SIP: Insufficient balance.");
    EXPECT_EQ(order1.folio, "");
    EXPECT_EQ(order1.fund, "HDFC Balanced Advantage Fund - Direct Plan");
    EXPECT_EQ(order1.orderTimestamp, "2021-06-30 08:33:07");
    EXPECT_EQ(order1.exchangeTimestamp, "2021-06-30");
    EXPECT_EQ(order1.settlementID, "2122061");
    EXPECT_EQ(order1.transactionType, "BUY");
    EXPECT_EQ(order1.variety, "amc_sip");
    EXPECT_EQ(order1.purchaseType, "FRESH");
    EXPECT_EQ(order1.quantity, 0.0);
    EXPECT_DOUBLE_EQ(order1.amount, 1000.0);
    EXPECT_DOUBLE_EQ(order1.lastPrice, 30.6800);
    EXPECT_DOUBLE_EQ(order1.averagePrice, 0.0);
    EXPECT_EQ(order1.placedBy, "ZV8062");
    EXPECT_EQ(order1.tag, "");

    kc::mfOrder order2 = orders[1];
    EXPECT_EQ(order2.orderID, "ef7e696c-2fa6-400b-b180-eb25e6a04ccf");
    EXPECT_EQ(order2.exchangeOrderID, "");
    EXPECT_EQ(order2.tradingsymbol, "INF174K01LS2");
    EXPECT_EQ(order2.status, "REJECTED");
    EXPECT_EQ(order2.statusMessage, "SIP: Insufficient balance.");
    EXPECT_EQ(order2.folio, "");
    EXPECT_EQ(order2.fund, "Kotak Flexicap Fund - Direct Plan");
    EXPECT_EQ(order2.orderTimestamp, "2021-06-30 01:30:02");
    EXPECT_EQ(order2.exchangeTimestamp, "");
    EXPECT_EQ(order2.settlementID, "");
    EXPECT_EQ(order2.transactionType, "BUY");
    EXPECT_EQ(order2.variety, "sip");
    EXPECT_EQ(order2.purchaseType, "ADDITIONAL");
    EXPECT_EQ(order2.quantity, 0.0);
    EXPECT_DOUBLE_EQ(order2.amount, 2000.0);
    EXPECT_DOUBLE_EQ(order2.lastPrice, 52.7980);
    EXPECT_DOUBLE_EQ(order2.averagePrice, 0.0);
    EXPECT_EQ(order2.placedBy, "ZV8062");
    EXPECT_EQ(order2.tag, "coinandroidsip");
}

TEST(kiteTest, getMFOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_orders_info.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::mfOrder order = Kite.getMfOrder("arg1");

    // Expected values
    EXPECT_EQ(order.orderID, "2b6ad4b7-c84e-4c76-b459-f3a8994184f1");
    EXPECT_EQ(order.exchangeOrderID, "");
    EXPECT_EQ(order.tradingsymbol, "INF761K01EE1");
    EXPECT_EQ(order.status, "OPEN");
    EXPECT_EQ(order.statusMessage, "Insufficient fund. 1/5");
    EXPECT_EQ(order.folio, "");
    EXPECT_EQ(order.fund, "BOI AXA Arbitrage Fund - Direct Plan");
    EXPECT_EQ(order.orderTimestamp, "2021-06-29 12:20:28");
    EXPECT_EQ(order.exchangeTimestamp, "");
    EXPECT_EQ(order.settlementID, "");
    EXPECT_EQ(order.transactionType, "BUY");
    EXPECT_EQ(order.variety, "regular");
    EXPECT_EQ(order.purchaseType, "FRESH");
    EXPECT_EQ(order.quantity, 0);
    EXPECT_DOUBLE_EQ(order.amount, 5000);
    EXPECT_DOUBLE_EQ(order.lastPrice, 10.432399999999999);
    EXPECT_DOUBLE_EQ(order.averagePrice, 0);
    EXPECT_EQ(order.placedBy, "ZV8062");
    EXPECT_EQ(order.tag, "");
}

TEST(kiteTest, getMFHoldingsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_holdings.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::mfHolding> Holdings = Kite.getMfHoldings();

    // Expected values
    ASSERT_EQ(Holdings.size(), 5);

    kc::mfHolding holding1 = Holdings[0];
    EXPECT_EQ(holding1.folio, "123123/123");
    EXPECT_EQ(holding1.fund, "Kotak Select Focus Fund - Direct Plan");
    EXPECT_EQ(holding1.tradingsymbol, "INF174K01LS2");
    EXPECT_DOUBLE_EQ(holding1.averagePrice, 30.729);
    EXPECT_DOUBLE_EQ(holding1.lastPrice, 33.014);
    EXPECT_DOUBLE_EQ(holding1.Pnl, 594.769);
    EXPECT_EQ(holding1.lastPriceDate, "2016-11-11");
    EXPECT_DOUBLE_EQ(holding1.quantity, 260.337);

    kc::mfHolding holding2 = Holdings[1];
    EXPECT_EQ(holding2.folio, "385080203");
    EXPECT_EQ(holding2.fund, "DSP BlackRock Money Manager Fund");
    EXPECT_EQ(holding2.tradingsymbol, "INF740K01QQ3");
    EXPECT_DOUBLE_EQ(holding2.averagePrice, 2146.131);
    EXPECT_DOUBLE_EQ(holding2.lastPrice, 2277.0708);
    EXPECT_DOUBLE_EQ(holding2.Pnl, 61.018);
    EXPECT_EQ(holding2.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding2.quantity, 0.466);

    kc::mfHolding holding3 = Holdings[2];
    EXPECT_EQ(holding3.folio, "1052046771");
    EXPECT_EQ(holding3.fund, "HDFC TaxSaver - Regular Plan");
    EXPECT_EQ(holding3.tradingsymbol, "INF179K01BB8");
    EXPECT_DOUBLE_EQ(holding3.averagePrice, 345.849);
    EXPECT_DOUBLE_EQ(holding3.lastPrice, 559.081);
    EXPECT_DOUBLE_EQ(holding3.Pnl, 61963.074);
    EXPECT_EQ(holding3.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding3.quantity, 290.59);

    kc::mfHolding holding4 = Holdings[3];
    EXPECT_EQ(holding4.folio, "91022348426");
    EXPECT_EQ(holding4.fund, "Axis Long Term Equity Fund");
    EXPECT_EQ(holding4.tradingsymbol, "INF846K01131");
    EXPECT_DOUBLE_EQ(holding4.averagePrice, 28.779);
    EXPECT_DOUBLE_EQ(holding4.lastPrice, 41.3876);
    EXPECT_DOUBLE_EQ(holding4.Pnl, 44467.717);
    EXPECT_EQ(holding4.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding4.quantity, 3526.834);

    kc::mfHolding holding5 = Holdings[4];
    EXPECT_EQ(holding5.folio, "488155267386");
    EXPECT_EQ(holding5.fund, "Reliance Money Manager Fund");
    EXPECT_EQ(holding5.tradingsymbol, "INF204K01EY0");
    EXPECT_DOUBLE_EQ(holding5.averagePrice, 1002.948);
    EXPECT_DOUBLE_EQ(holding5.lastPrice, 1007.5645);
    EXPECT_DOUBLE_EQ(holding5.Pnl, 2.304);
    EXPECT_EQ(holding5.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding5.quantity, 0.499);
}

// SIP tests

TEST(kiteTest, placeMFSIPTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    auto res = Kite.placeMfSip(kc::placeMfSipParams()
                                   .Symbol("INF174K01LS2")
                                   .Amount(900)
                                   .Frequency("monthly")
                                   .Installments(-1)
                                   .InstallmentDay(10));

    // Expected values
    EXPECT_EQ(res.first, "123457");
    EXPECT_EQ(res.second, "123457");
}

TEST(kiteTest, modifyMFSIPTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    Kite.modifyMfSip(kc::modifyMfSipParams().SipId("11123").Status("paused"));
}

TEST(kiteTest, cancelMFSIPTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string SIPID = Kite.cancelMfSip("arg1");

    // Expected values
    EXPECT_EQ(SIPID, "123457");
}

TEST(kiteTest, getSIPsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_sips.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::mfSip> SIPs = Kite.getSips();

    // Expected values
    ASSERT_EQ(SIPs.size(), 5);

    kc::mfSip sip1 = SIPs[0];
    EXPECT_EQ(sip1.ID, "892741486820670");
    EXPECT_EQ(sip1.tradingsymbol, "INF209K01VD7");
    EXPECT_EQ(sip1.fundName, "Aditya Birla Sun Life Liquid Fund - Direct Plan");
    EXPECT_EQ(sip1.dividendType, "idcw");
    EXPECT_EQ(sip1.transactionType, "BUY");
    EXPECT_EQ(sip1.status, "ACTIVE");
    EXPECT_EQ(sip1.created, "2021-05-05 05:56:27");
    EXPECT_EQ(sip1.frequency, "weekly");
    EXPECT_DOUBLE_EQ(sip1.instalmentAmount, 500);
    EXPECT_EQ(sip1.instalments, -1);
    EXPECT_EQ(sip1.lastInstalment, "2021-05-05 05:56:27");
    EXPECT_EQ(sip1.pendingInstalments, -1);
    EXPECT_EQ(sip1.instalmentDay, 0);
    EXPECT_EQ(sip1.tag, "coiniossip");
}

TEST(kiteTest, getSIPTest) {

    std::ifstream jsonFile("../../tests/mock_responses/mf_sip_info.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kc::mfSip sip = Kite.getSip("arg1");

    // Expected values
    EXPECT_EQ(sip.ID, "846479755969168");
    EXPECT_EQ(sip.tradingsymbol, "INF179K01VY8");
    EXPECT_EQ(sip.fundName, "HDFC Balanced Advantage Fund - Direct Plan");
    EXPECT_EQ(sip.dividendType, "idcw");
    EXPECT_EQ(sip.transactionType, "BUY");
    EXPECT_EQ(sip.status, "ACTIVE");
    EXPECT_EQ(sip.created, "2021-05-22 10:45:29");
    EXPECT_EQ(sip.frequency, "monthly");
    EXPECT_DOUBLE_EQ(sip.instalmentAmount, 1000);
    EXPECT_EQ(sip.instalments, 9999);
    EXPECT_EQ(sip.lastInstalment, "2021-06-10 08:37:11.273142");
    EXPECT_EQ(sip.pendingInstalments, 9998);
    EXPECT_EQ(sip.instalmentDay, 10);
    EXPECT_EQ(sip.tag, "coinandroidsip");
}

// Order margins tests

TEST(kiteTest, getOrderMarginsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_margins.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kc::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kc::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kc::orderMargins> ordMargins = Kite.getOrderMargins({});

    // Expected values
    ASSERT_EQ(ordMargins.size(), 1);

    kc::orderMargins ordMargins1 = ordMargins[0];
    EXPECT_EQ(ordMargins1.type, "equity");
    EXPECT_EQ(ordMargins1.tradingSymbol, "INFY");
    EXPECT_EQ(ordMargins1.exchange, "NSE");
    EXPECT_DOUBLE_EQ(ordMargins1.span, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.exposure, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.optionPremium, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.additional, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.bo, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.cash, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.var, 961.45);
    EXPECT_DOUBLE_EQ(ordMargins1.pnl.realised, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.pnl.unrealised, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.total, 961.45);
}

// Instruments tests

TEST(kiteTest, getInstrumentsTest) {

    std::ifstream csvFile("../../tests/mock_responses/instruments_all.csv");
    ASSERT_TRUE(csvFile);
    string csv(std::istreambuf_iterator<char> { csvFile }, {});

    mockKite Kite;

    EXPECT_CALL(Kite, _sendInstrumentsReq(_)).WillOnce(testing::Return(csv));

    std::vector<kc::instrument> instruments = Kite.getInstruments();

    // Expected values
    ASSERT_EQ(instruments.size(), 99);

    kc::instrument instrument1 = instruments[0];
    EXPECT_EQ(instrument1.instrumentToken, 3813889);
    EXPECT_EQ(instrument1.exchangeToken, 14898);
    EXPECT_EQ(instrument1.tradingsymbol, "CENTRALBK-BE");
    EXPECT_EQ(instrument1.name, "CENTRAL BANK OF INDIA");
    EXPECT_DOUBLE_EQ(instrument1.lastPrice, 0.0);
    EXPECT_EQ(instrument1.expiry, "");
    EXPECT_DOUBLE_EQ(instrument1.strikePrice, 0.0);
    EXPECT_DOUBLE_EQ(instrument1.tickSize, 0.05);
    EXPECT_DOUBLE_EQ(instrument1.lotSize, 1);
    EXPECT_EQ(instrument1.instrumentType, "EQ");
    EXPECT_EQ(instrument1.segment, "NSE");
    EXPECT_EQ(instrument1.exchange, "NSE");

    kc::instrument instrument2 = instruments[1];
    EXPECT_EQ(instrument2.instrumentToken, 4645121);
    EXPECT_EQ(instrument2.exchangeToken, 18145);
    EXPECT_EQ(instrument2.tradingsymbol, "EMMBI-BL");
    EXPECT_EQ(instrument2.name, "EMMBI INDUSTRIES");
    EXPECT_DOUBLE_EQ(instrument2.lastPrice, 0.0);
    EXPECT_EQ(instrument2.expiry, "");
    EXPECT_DOUBLE_EQ(instrument2.strikePrice, 0.0);
    EXPECT_DOUBLE_EQ(instrument2.tickSize, 0.05);
    EXPECT_DOUBLE_EQ(instrument2.lotSize, 1);
    EXPECT_EQ(instrument2.instrumentType, "EQ");
    EXPECT_EQ(instrument2.segment, "NSE");
    EXPECT_EQ(instrument2.exchange, "NSE");
}

TEST(kiteTest, getMFInstrumentsTest) {

    std::ifstream csvFile("../../tests/mock_responses/mf_instruments.csv");
    ASSERT_TRUE(csvFile);
    string csv(std::istreambuf_iterator<char> { csvFile }, {});

    mockKite Kite;

    EXPECT_CALL(Kite, _sendInstrumentsReq(_)).WillOnce(testing::Return(csv));

    std::vector<kc::MFInstrument> instruments = Kite.getMfInstruments();

    // Expected values
    ASSERT_EQ(instruments.size(), 99);

    kc::MFInstrument instrument1 = instruments[0];
    EXPECT_EQ(instrument1.tradingsymbol, "INF209K01157");
    EXPECT_EQ(instrument1.AMC, "BirlaSunLifeMutualFund_MF");
    EXPECT_EQ(instrument1.name, "Aditya Birla Sun Life Advantage Fund");
    EXPECT_EQ(instrument1.purchaseAllowed, true);
    EXPECT_EQ(instrument1.redemtpionAllowed, true);
    EXPECT_EQ(instrument1.minimumPurchaseAmount, 1000.0);
    EXPECT_EQ(instrument1.purchaseAmountMultiplier, 1.0);
    EXPECT_EQ(instrument1.minimumAdditionalPurchaseAmount, 1000.0);
    EXPECT_EQ(instrument1.minimumRedemptionQuantity, 0.001);
    EXPECT_EQ(instrument1.redemptionQuantityMultiplier, 0.001);
    EXPECT_EQ(instrument1.dividendType, "payout");
    EXPECT_EQ(instrument1.schemeType, "equity");
    EXPECT_EQ(instrument1.plan, "regular");
    EXPECT_EQ(instrument1.settlementType, "T3");
    EXPECT_EQ(instrument1.lastPrice, 106.8);
    EXPECT_EQ(instrument1.lastPriceDate, "2017-11-23");

    kc::MFInstrument instrument2 = instruments[1];
    EXPECT_EQ(instrument2.tradingsymbol, "INF209K01165");
    EXPECT_EQ(instrument2.AMC, "BirlaSunLifeMutualFund_MF");
    EXPECT_EQ(instrument2.name, "Aditya Birla Sun Life Advantage Fund");
    EXPECT_EQ(instrument2.purchaseAllowed, true);
    EXPECT_EQ(instrument2.redemtpionAllowed, true);
    EXPECT_EQ(instrument2.minimumPurchaseAmount, 1000.0);
    EXPECT_EQ(instrument2.purchaseAmountMultiplier, 1.0);
    EXPECT_EQ(instrument2.minimumAdditionalPurchaseAmount, 1000.0);
    EXPECT_EQ(instrument2.minimumRedemptionQuantity, 0.001);
    EXPECT_EQ(instrument2.redemptionQuantityMultiplier, 0.001);
    EXPECT_EQ(instrument2.dividendType, "growth");
    EXPECT_EQ(instrument2.schemeType, "equity");
    EXPECT_EQ(instrument2.plan, "regular");
    EXPECT_EQ(instrument2.settlementType, "T3");
    EXPECT_EQ(instrument2.lastPrice, 436.72);
    EXPECT_EQ(instrument2.lastPriceDate, "2017-11-23");
}
