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

#include <string>
#include <utility>
#include <vector>

#include <gmock/gmock-actions.h>
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock-generated-function-mockers.h>
#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../kitepp.hpp"
#include "../utils.hpp"

using std::string;
using ::testing::ByMove;
using ::testing::Return;
using ::testing::StrictMock;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

TEST(kiteTest, placeOrderTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/order_response.json");
    const int QUANTITY = 10;
    const string VARIETY = "regular";
    const string EXCHANGE = "NSE";
    const string SYMBOL = "TCS";
    const string TRANSACTION_TYPE = "BUY";
    const string PRODUCT = "NRML";
    const string ORDER_TYPE = "MARKET";
    const string VALIDITY = "day";
    const string TAG = "order1";
    const auto PLACE_ORDER_PARAMS = kc::placeOrderParams()
                                        .Quantity(QUANTITY)
                                        .Variety(VARIETY)
                                        .Exchange(EXCHANGE)
                                        .Symbol(SYMBOL)
                                        .TransactionType(TRANSACTION_TYPE)
                                        .Product(PRODUCT)
                                        .OrderType(ORDER_TYPE)
                                        .Validity(VALIDITY)
                                        .Tag(TAG);
    const string EXPECTED_ORDER_ID = "151220000000000";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::POST,
                                  "/orders/{0}" },
                          utils::http::Params {
                              { "exchange", EXCHANGE },
                              { "order_type", ORDER_TYPE },
                              { "product", PRODUCT },
                              { "tradingsymbol", SYMBOL },
                              { "transaction_type", TRANSACTION_TYPE },
                              { "tag", TAG },
                              { "validity", VALIDITY },
                              { "quantity", std::to_string(QUANTITY) },
                          },
                          utils::FmtArgs { VARIETY }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));
    const string orderID = Kite.placeOrder(PLACE_ORDER_PARAMS);

    EXPECT_EQ(orderID, EXPECTED_ORDER_ID);
}

TEST(kiteTest, modifyOrderTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/order_response.json");
    const int QUANTITY = 11;
    const string VARIETY = "regular";
    const string ORDER_ID = "151220000000000";
    const string VALIDITY = "IOC";
    // clang-format off
    const auto MODIFY_ORDER_PARAMS = kc::modifyOrderParams()
                                         .Quantity(QUANTITY)
                                         .Variety(VARIETY)
                                         .Validity(VALIDITY)
                                         .OrderId(ORDER_ID);
    // clang-format on
    const string EXPECTED_ORDER_ID = "151220000000000";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::PUT,
                                  "/orders/{0}/{1}" },
                          utils::http::Params {
                              { "validity", VALIDITY },
                              { "quantity", std::to_string(QUANTITY) },
                          },
                          utils::FmtArgs { VARIETY, ORDER_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));
    const string RECEIVED_ORDER_ID = Kite.modifyOrder(MODIFY_ORDER_PARAMS);

    EXPECT_EQ(RECEIVED_ORDER_ID, "151220000000000");
}

TEST(kiteTest, cancelOrderTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/order_response.json");
    const string VARIETY = "regular";
    const string ORDER_ID = "151220000000000";
    const string EXPECTED_ORDER_ID = "151220000000000";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(
        Kite, sendReq(utils::http::endpoint { utils::http::METHOD::DEL,
                          "/orders/{0}/{1}" },
                  utils::http::Params {}, utils::FmtArgs { VARIETY, ORDER_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));
    const string RECEIVED_ORDER_ID = Kite.cancelOrder(VARIETY, ORDER_ID);

    EXPECT_EQ(RECEIVED_ORDER_ID, "151220000000000");
}

TEST(kiteTest, ordersTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/orders.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/orders" },
            utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::order> Orders = Kite.orders();

    ASSERT_THAT(Orders.size(), 5);
    const kc::order order1 = Orders[0];
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
    const string JSON =
        kc::test::readFile("../tests/mock_responses/orders.json");
    const string ORDER_ID = "100000000000000";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/orders/{0}" },
                          utils::http::Params {}, utils::FmtArgs { ORDER_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::order> Orders = Kite.orderHistory(ORDER_ID);

    ASSERT_THAT(Orders.size(), 5);
    const kc::order order1 = Orders[0];
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
    const string JSON =
        kc::test::readFile("../tests/mock_responses/trades.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/trades" },
            utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::trade> Trades = Kite.trades();

    ASSERT_EQ(Trades.size(), 4);
    const kc::trade trade1 = Trades[0];
    EXPECT_DOUBLE_EQ(trade1.averagePrice, 420.64999999999998);
    EXPECT_EQ(trade1.exchange, "NSE");
    EXPECT_EQ(trade1.exchangeOrderID, "300000000000000");
    EXPECT_EQ(trade1.exchangeTimestamp, "2021-05-31 09:16:39");
    EXPECT_EQ(trade1.instrumentToken, 779521);
    EXPECT_EQ(trade1.orderID, "200000000000000");
    EXPECT_EQ(trade1.product, "CNC");
    EXPECT_DOUBLE_EQ(trade1.quantity, 1);
    EXPECT_EQ(trade1.tradeID, "10000000");
    EXPECT_EQ(trade1.tradingSymbol, "SBIN");
    EXPECT_EQ(trade1.transactionType, "BUY");
}

TEST(kiteTest, orderTradesTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/order_trades.json");
    const string ORDER_ID = "100000000000000";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/orders/{0}/trades" },
                          utils::http::Params {}, utils::FmtArgs { ORDER_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::trade> Trades = Kite.orderTrades(ORDER_ID);

    ASSERT_EQ(Trades.size(), 1);
    const kc::trade trade1 = Trades[0];
    EXPECT_DOUBLE_EQ(trade1.averagePrice, 4852);
    EXPECT_EQ(trade1.exchange, "MCX");
    EXPECT_EQ(trade1.exchangeOrderID, "300000000000000");
    EXPECT_EQ(trade1.exchangeTimestamp, "2021-05-31 16:00:36");
    EXPECT_EQ(trade1.instrumentToken, 58424839);
    EXPECT_EQ(trade1.orderID, "200000000000000");
    EXPECT_EQ(trade1.product, "NRML");
    EXPECT_DOUBLE_EQ(trade1.quantity, 1);
    EXPECT_EQ(trade1.tradeID, "10000000");
    EXPECT_EQ(trade1.tradingSymbol, "GOLDPETAL21JUNFUT");
    EXPECT_EQ(trade1.transactionType, "BUY");
}
