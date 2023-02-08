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

TEST(kiteTest, placeMFOrderTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_order_response.json");
    constexpr int QUANTITY = 10;
    const string SYMBOL = "INF174K01LS2";
    const string TRANSACTION_TYPE = "BUY";
    constexpr double AMOUNT = 1000;
    const string EXPECTED_ORDER_ID = "3bb085d1-5038-450e-a807-6543fef6c9ae";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::POST,
                                  "/mf/orders" },
                          utils::http::Params {
                              { "tradingsymbol", SYMBOL },
                              { "transaction_type", TRANSACTION_TYPE },
                              { "quantity", std::to_string(QUANTITY) },
                              { "amount", std::to_string(AMOUNT) },
                          },
                          utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    // clang-format off
    const string ORDER_ID = Kite.placeMfOrder(kc::placeMfOrderParams()
                                        .Quantity(QUANTITY)
                                        .Amount(AMOUNT)
                                        .Symbol(SYMBOL)
                                        .TransactionType(TRANSACTION_TYPE)
                                    );
    // clang-format on

    // Expected values
    EXPECT_EQ(ORDER_ID, EXPECTED_ORDER_ID);
}

TEST(kiteTest, cancelMFOrderTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_order_response.json");
    const string ORDER_ID = "123457";
    const string EXPECTED_ORDER_ID = "3bb085d1-5038-450e-a807-6543fef6c9ae";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::DEL,
                                  "/mf/orders/{0}" },
                          utils::http::Params {}, utils::FmtArgs { ORDER_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const string RECEIVED_ORDER_ID = Kite.cancelMfOrder(ORDER_ID);

    EXPECT_EQ(RECEIVED_ORDER_ID, EXPECTED_ORDER_ID);
}

TEST(kiteTest, getMFOrdersTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_orders.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/mf/orders" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::mfOrder> orders = Kite.getMfOrders();
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
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_orders_info.json");
    const string ORDER_ID = "2b6ad4b7-c84e-4c76-b459-f3a8994184f1";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/mf/orders/{0}" },
                          utils::http::Params {}, utils::FmtArgs { ORDER_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const kc::mfOrder ORDER = Kite.getMfOrder(ORDER_ID);

    EXPECT_EQ(ORDER.orderID, "2b6ad4b7-c84e-4c76-b459-f3a8994184f1");
    EXPECT_EQ(ORDER.exchangeOrderID, "");
    EXPECT_EQ(ORDER.tradingsymbol, "INF761K01EE1");
    EXPECT_EQ(ORDER.status, "OPEN");
    EXPECT_EQ(ORDER.statusMessage, "Insufficient fund. 1/5");
    EXPECT_EQ(ORDER.folio, "");
    EXPECT_EQ(ORDER.fund, "BOI AXA Arbitrage Fund - Direct Plan");
    EXPECT_EQ(ORDER.orderTimestamp, "2021-06-29 12:20:28");
    EXPECT_EQ(ORDER.exchangeTimestamp, "");
    EXPECT_EQ(ORDER.settlementID, "");
    EXPECT_EQ(ORDER.transactionType, "BUY");
    EXPECT_EQ(ORDER.variety, "regular");
    EXPECT_EQ(ORDER.purchaseType, "FRESH");
    EXPECT_EQ(ORDER.quantity, 0);
    EXPECT_DOUBLE_EQ(ORDER.amount, 5000);
    EXPECT_DOUBLE_EQ(ORDER.lastPrice, 10.432399999999999);
    EXPECT_DOUBLE_EQ(ORDER.averagePrice, 0);
    EXPECT_EQ(ORDER.placedBy, "ZV8062");
    EXPECT_EQ(ORDER.tag, "");
}

TEST(kiteTest, getMFHoldingsTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_holdings.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/mf/holdings" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::mfHolding> HOLDINGS = Kite.getMfHoldings();

    ASSERT_EQ(HOLDINGS.size(), 3);
    kc::mfHolding holding1 = HOLDINGS[0];
    EXPECT_EQ(holding1.folio, "3108290884");
    EXPECT_EQ(holding1.fund, "INVESCO INDIA TAX PLAN - DIRECT PLAN");
    EXPECT_EQ(holding1.tradingsymbol, "INF205K01NT8");
    EXPECT_DOUBLE_EQ(holding1.averagePrice, 78.43);
    EXPECT_DOUBLE_EQ(holding1.lastPrice, 84.86);
    EXPECT_DOUBLE_EQ(holding1.Pnl, 0);
    EXPECT_EQ(holding1.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding1.quantity, 382.488);

    kc::mfHolding holding2 = HOLDINGS[1];
    EXPECT_EQ(holding2.folio, "5102495241");
    EXPECT_EQ(holding2.fund, "Indiabulls Liquid Fund - Direct Plan");
    EXPECT_EQ(holding2.tradingsymbol, "INF666M01451");
    EXPECT_DOUBLE_EQ(holding2.averagePrice, 1874.101138);
    EXPECT_DOUBLE_EQ(holding2.lastPrice, 2081.4984);
    EXPECT_DOUBLE_EQ(holding2.Pnl, 0);
    EXPECT_EQ(holding2.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding2.quantity, 1.334);

    kc::mfHolding holding3 = HOLDINGS[2];
    EXPECT_EQ(holding3.folio, "9104386836");
    EXPECT_EQ(holding3.fund, "BOI AXA TAX ADVANTAGE FUND - DIRECT PLAN");
    EXPECT_EQ(holding3.tradingsymbol, "INF761K01884");
    EXPECT_DOUBLE_EQ(holding3.averagePrice, 116.7);
    EXPECT_DOUBLE_EQ(holding3.lastPrice, 101.13);
    EXPECT_DOUBLE_EQ(holding3.Pnl, 0);
    EXPECT_EQ(holding3.lastPriceDate, "");
    EXPECT_DOUBLE_EQ(holding3.quantity, 257.057);
}

TEST(kiteTest, placeMFSIPTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_sip_place.json");
    const string SYMBOL = "INF174K01LS2";
    constexpr double AMOUNT = 1000;
    const string FREQUENCY = "monthly";
    constexpr int INSTALLMENTS = 100;
    constexpr int INSTALLMENT_DAY = 12;
    const string TAG = "randomtag";
    const string SIP_ID = "986124545877922";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::POST, "/mf/sips" },
            utils::http::Params {
                { "tradingsymbol", SYMBOL },
                { "amount", std::to_string(AMOUNT) },
                { "instalments", std::to_string(INSTALLMENTS) },
                { "frequency", FREQUENCY },
                { "instalment_day", std::to_string(INSTALLMENT_DAY) },
                { "tag", TAG },
            },
            utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const auto SIP = Kite.placeMfSip(kc::placeMfSipParams()
                                         .Symbol(SYMBOL)
                                         .Amount(AMOUNT)
                                         .Frequency(FREQUENCY)
                                         .Installments(INSTALLMENTS)
                                         .InstallmentDay(INSTALLMENT_DAY)
                                         .Tag(TAG));

    EXPECT_EQ(SIP.sipId, SIP_ID);
}

TEST(kiteTest, modifyMFSIPTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_order_response.json");
    constexpr double AMOUNT = 900;
    const string SIP_ID = "123457";
    const string FREQUENCY = "monthly";
    const string EXPECTED_SIP_ID = "3bb085d1-5038-450e-a807-6543fef6c9ae";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::PUT,
                                  "/mf/sips/{0}" },
                          utils::http::Params {
                              { "amount", std::to_string(AMOUNT) },
                              { "frequency", FREQUENCY },
                          },
                          utils::FmtArgs { SIP_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const string RECEIVED_SIP_ID = Kite.modifyMfSip(
        kc::modifyMfSipParams().SipId(SIP_ID).Frequency(FREQUENCY).Amount(
            AMOUNT));

    EXPECT_EQ(RECEIVED_SIP_ID, EXPECTED_SIP_ID);
}

TEST(kiteTest, cancelMFSIPTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_sip_cancel.json");
    const string SIP_ID = "986124545877922";
    const string EXPECTED_SIP_ID = "986124545877922";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::DEL,
                                  "/mf/sips/{0}" },
                          utils::http::Params {}, utils::FmtArgs { SIP_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    string SIPID = Kite.cancelMfSip(SIP_ID);

    EXPECT_EQ(SIPID, EXPECTED_SIP_ID);
}

TEST(kiteTest, getSIPsTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_sips.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/mf/sips" },
            utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::mfSip> SIPS = Kite.getSips();
    ASSERT_EQ(SIPS.size(), 5);
    kc::mfSip sip1 = SIPS[0];
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
    const string JSON =
        kc::test::readFile("../tests/mock_responses/mf_sip_info.json");
    const string SIP_ID = "181635213661372";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/mf/sips/{0}" },
                          utils::http::Params {}, utils::FmtArgs { SIP_ID }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const kc::mfSip sip = Kite.getSip(SIP_ID);
    EXPECT_EQ(sip.ID, "181635213661372");
    EXPECT_EQ(sip.tradingsymbol, "INF209KB1ZH2");
    EXPECT_EQ(
        sip.fundName, "Aditya Birla Sun Life Overnight Fund - Direct Plan");
    EXPECT_EQ(sip.dividendType, "growth");
    EXPECT_EQ(sip.transactionType, "BUY");
    EXPECT_EQ(sip.status, "ACTIVE");
    EXPECT_EQ(sip.created, "2022-02-15 13:39:44");
    EXPECT_EQ(sip.frequency, "weekly");
    EXPECT_DOUBLE_EQ(sip.instalmentAmount, 5000);
    EXPECT_EQ(sip.instalments, -1);
    EXPECT_EQ(sip.lastInstalment, "2022-02-15 13:39:44");
    EXPECT_EQ(sip.pendingInstalments, -1);
    EXPECT_EQ(sip.instalmentDay, 0);
    EXPECT_EQ(sip.tag, "coiniossip");
}

TEST(kiteTest, getMFInstrumentsTest) {
    const string CSV =
        kc::test::readFile("../tests/mock_responses/mf_instruments.csv");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                    "/mf/instruments", utils::http::CONTENT_TYPE::NON_JSON },
            utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(Return(
            ByMove(utils::http::response(utils::http::code::OK, CSV, false))));

    const std::vector<kc::mfInstrument> INSTRUMENTS = Kite.getMfInstruments();

    ASSERT_EQ(INSTRUMENTS.size(), 99);
    kc::mfInstrument instrument1 = INSTRUMENTS[0];
    EXPECT_EQ(instrument1.tradingsymbol, "INF209K01157");
    EXPECT_EQ(instrument1.amc, "BirlaSunLifeMutualFund_MF");
    EXPECT_EQ(instrument1.name, "Aditya Birla Sun Life Advantage Fund");
    EXPECT_EQ(instrument1.purchaseAllowed, true);
    EXPECT_EQ(instrument1.redemptionAllowed, true);
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

    kc::mfInstrument instrument2 = INSTRUMENTS[1];
    EXPECT_EQ(instrument2.tradingsymbol, "INF209K01165");
    EXPECT_EQ(instrument2.amc, "BirlaSunLifeMutualFund_MF");
    EXPECT_EQ(instrument2.name, "Aditya Birla Sun Life Advantage Fund");
    EXPECT_EQ(instrument2.purchaseAllowed, true);
    EXPECT_EQ(instrument2.redemptionAllowed, true);
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
