/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2023 Bhumit Attarde
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

TEST(kiteTest, getOrderMarginsTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/order_margins.json");
    constexpr double QUANTITY = 1;
    constexpr double PRICE = 0;
    constexpr double TRIGGER_PRICE = 0;
    const string EXCHANGE = "NSE";
    const string TRADINGSYMBOL = "INFY";
    const string TRANSACTION_TYPE = "BUY";
    const string VARIETY = "regular";
    const string PRODUCT = "CNC";
    const string ORDER_TYPE = "MARKET";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::POST,
                    "/margins/orders", utils::http::CONTENT_TYPE::JSON },
            utils::http::Params { { "",
                R"([{"exchange":"NSE","tradingsymbol":"INFY","transaction_type":"BUY","variety":"regular","product":"CNC","order_type":"MARKET","quantity":1.0,"price":0.0,"trigger_price":0.0}])" } },
            utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::orderMargins> MARGINS =
        Kite.getOrderMargins({ kc::marginsParams()
                                   .Quantity(QUANTITY)
                                   .Price(PRICE)
                                   .TriggerPrice(TRIGGER_PRICE)
                                   .Exchange(EXCHANGE)
                                   .Tradingsymbol(TRADINGSYMBOL)
                                   .TransactionType(TRANSACTION_TYPE)
                                   .Variety(VARIETY)
                                   .Product(PRODUCT)
                                   .OrderType(ORDER_TYPE) });

    ASSERT_EQ(MARGINS.size(), 1);
    kc::orderMargins ordMargins1 = MARGINS[0];
    EXPECT_EQ(ordMargins1.type, "equity");
    EXPECT_EQ(ordMargins1.tradingSymbol, "INFY");
    EXPECT_EQ(ordMargins1.exchange, "NSE");
    EXPECT_DOUBLE_EQ(ordMargins1.span, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.exposure, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.optionPremium, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.additional, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.bo, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.cash, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.var, 1498);
    EXPECT_DOUBLE_EQ(ordMargins1.pnl.realised, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.pnl.unrealised, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.total, 1498);
    EXPECT_DOUBLE_EQ(ordMargins1.leverage, 1);

    EXPECT_DOUBLE_EQ(ordMargins1.charges.transactionTax, 1.498);
    EXPECT_EQ(ordMargins1.charges.transactionTaxType, "stt");
    EXPECT_DOUBLE_EQ(ordMargins1.charges.exchangeTurnoverCharge, 0.051681);
    EXPECT_DOUBLE_EQ(ordMargins1.charges.sebiTurnoverCharge, 0.001498);
    EXPECT_DOUBLE_EQ(ordMargins1.charges.brokerage, 0.01);
    EXPECT_DOUBLE_EQ(ordMargins1.charges.stampDuty, 0.22);

    EXPECT_DOUBLE_EQ(ordMargins1.charges.gst.igst, 0.011372219999999999);
    EXPECT_DOUBLE_EQ(ordMargins1.charges.gst.cgst, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.charges.gst.sgst, 0);
    EXPECT_DOUBLE_EQ(ordMargins1.charges.gst.total, 0.011372219999999999);
}

TEST(kiteTest, getBasketMarginsTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/basket_margins.json");
    constexpr double QUANTITY = 1;
    constexpr double PRICE = 0;
    constexpr double TRIGGER_PRICE = 0;
    const string EXCHANGE = "NSE";
    const string TRADINGSYMBOL = "INFY";
    const string TRANSACTION_TYPE = "BUY";
    const string VARIETY = "regular";
    const string PRODUCT = "CNC";
    const string ORDER_TYPE = "MARKET";
    const bool CONSIDER_POSITIONS = false;

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::POST,
                    "/margins/basket?consider_positions={0}",
                    utils::http::CONTENT_TYPE::JSON },
            utils::http::Params { { "",
                R"([{"exchange":"NSE","tradingsymbol":"INFY","transaction_type":"BUY","variety":"regular","product":"CNC","order_type":"MARKET","quantity":1.0,"price":0.0,"trigger_price":0.0}])" } },
            utils::FmtArgs { CONSIDER_POSITIONS ? "true" : "false" }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const kc::basketMargins MARGINS =
        Kite.getBasketMargins({ kc::marginsParams()
                                      .Quantity(QUANTITY)
                                      .Price(PRICE)
                                      .TriggerPrice(TRIGGER_PRICE)
                                      .Exchange(EXCHANGE)
                                      .Tradingsymbol(TRADINGSYMBOL)
                                      .TransactionType(TRANSACTION_TYPE)
                                      .Variety(VARIETY)
                                      .Product(PRODUCT)
                                      .OrderType(ORDER_TYPE) },
            CONSIDER_POSITIONS);

    ASSERT_EQ(MARGINS.initial.type, "");
    ASSERT_EQ(MARGINS.initial.tradingSymbol, "");
    ASSERT_EQ(MARGINS.initial.exchange, "");
    EXPECT_DOUBLE_EQ(MARGINS.initial.span, 132257.58);
    EXPECT_DOUBLE_EQ(MARGINS.initial.exposure, 56597.19);
    EXPECT_DOUBLE_EQ(MARGINS.initial.optionPremium, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.additional, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.bo, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.cash, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.var, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.pnl.realised, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.pnl.unrealised, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.leverage, 0);

    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.transactionTax, 0);
    ASSERT_EQ(MARGINS.initial.charges.transactionTaxType, "");
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.exchangeTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.sebiTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.brokerage, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.stampDuty, 0);

    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.gst.igst, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.gst.cgst, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.gst.sgst, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.gst.total, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.charges.total, 0);
    EXPECT_DOUBLE_EQ(MARGINS.initial.total, 188854.77);

    ASSERT_EQ(MARGINS.final.type, "");
    ASSERT_EQ(MARGINS.final.tradingSymbol, "");
    ASSERT_EQ(MARGINS.final.exchange, "");
    EXPECT_DOUBLE_EQ(MARGINS.final.span, 26000.519999999986);
    EXPECT_DOUBLE_EQ(MARGINS.final.exposure, 9452.689999999999);
    EXPECT_DOUBLE_EQ(MARGINS.final.optionPremium, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.additional, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.bo, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.cash, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.var, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.pnl.realised, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.pnl.unrealised, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.leverage, 0);

    EXPECT_DOUBLE_EQ(MARGINS.final.charges.transactionTax, 0);
    ASSERT_EQ(MARGINS.final.charges.transactionTaxType, "");
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.exchangeTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.sebiTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.brokerage, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.stampDuty, 0);

    EXPECT_DOUBLE_EQ(MARGINS.final.charges.gst.igst, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.gst.cgst, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.gst.sgst, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.charges.gst.total, 0);

    EXPECT_DOUBLE_EQ(MARGINS.final.charges.total, 0);
    EXPECT_DOUBLE_EQ(MARGINS.final.total, 35453.209999999985);

    ASSERT_EQ(MARGINS.orders.size(), 2);

    kc::orderMargins order1 = MARGINS.orders[0];
    EXPECT_EQ(order1.type, "equity");
    EXPECT_EQ(order1.tradingSymbol, "NIFTY23JANFUT");
    EXPECT_EQ(order1.exchange, "NFO");
    EXPECT_DOUBLE_EQ(order1.span, 132257.58);
    EXPECT_DOUBLE_EQ(order1.exposure, 28239.120000000003);
    EXPECT_DOUBLE_EQ(order1.optionPremium, 0);
    EXPECT_DOUBLE_EQ(order1.additional, 0);
    EXPECT_DOUBLE_EQ(order1.bo, 0);
    EXPECT_DOUBLE_EQ(order1.cash, 0);
    EXPECT_DOUBLE_EQ(order1.var, 0);
    EXPECT_DOUBLE_EQ(order1.pnl.realised, 0);
    EXPECT_DOUBLE_EQ(order1.pnl.unrealised, 0);
    EXPECT_DOUBLE_EQ(order1.leverage, 1);

    EXPECT_DOUBLE_EQ(order1.charges.transactionTax, 0);
    ASSERT_EQ(order1.charges.transactionTaxType, "");
    EXPECT_DOUBLE_EQ(order1.charges.exchangeTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(order1.charges.sebiTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(order1.charges.brokerage, 0);
    EXPECT_DOUBLE_EQ(order1.charges.stampDuty, 0);

    EXPECT_DOUBLE_EQ(order1.charges.gst.igst, 0);
    EXPECT_DOUBLE_EQ(order1.charges.gst.cgst, 0);
    EXPECT_DOUBLE_EQ(order1.charges.gst.sgst, 0);
    EXPECT_DOUBLE_EQ(order1.charges.gst.total, 0);

    EXPECT_DOUBLE_EQ(order1.charges.total, 0);
    EXPECT_DOUBLE_EQ(order1.total, 160496.69999999998);

    kc::orderMargins order2 = MARGINS.orders[1];
    EXPECT_EQ(order2.type, "equity");
    EXPECT_EQ(order2.tradingSymbol, "NIFTY23FEBFUT");
    EXPECT_EQ(order2.exchange, "NFO");
    EXPECT_DOUBLE_EQ(order2.span, 0);
    EXPECT_DOUBLE_EQ(order2.exposure, 28358.07);
    EXPECT_DOUBLE_EQ(order2.optionPremium, 0);
    EXPECT_DOUBLE_EQ(order2.additional, 0);
    EXPECT_DOUBLE_EQ(order2.bo, 0);
    EXPECT_DOUBLE_EQ(order2.cash, 0);
    EXPECT_DOUBLE_EQ(order2.var, 0);
    EXPECT_DOUBLE_EQ(order2.pnl.realised, 0);
    EXPECT_DOUBLE_EQ(order2.pnl.unrealised, 0);
    EXPECT_DOUBLE_EQ(order2.leverage, 1);

    EXPECT_DOUBLE_EQ(order2.charges.transactionTax, 0);
    ASSERT_EQ(order2.charges.transactionTaxType, "");
    EXPECT_DOUBLE_EQ(order2.charges.exchangeTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(order2.charges.sebiTurnoverCharge, 0);
    EXPECT_DOUBLE_EQ(order2.charges.brokerage, 0);
    EXPECT_DOUBLE_EQ(order2.charges.stampDuty, 0);

    EXPECT_DOUBLE_EQ(order2.charges.gst.igst, 0);
    EXPECT_DOUBLE_EQ(order2.charges.gst.cgst, 0);
    EXPECT_DOUBLE_EQ(order2.charges.gst.sgst, 0);
    EXPECT_DOUBLE_EQ(order2.charges.gst.total, 0);

    EXPECT_DOUBLE_EQ(order2.charges.total, 0);
    EXPECT_DOUBLE_EQ(order2.total, 28358.07);
}