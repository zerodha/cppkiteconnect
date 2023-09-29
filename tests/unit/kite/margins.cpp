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
        Kite.getOrderMargins({ kc::orderMarginsParams()
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