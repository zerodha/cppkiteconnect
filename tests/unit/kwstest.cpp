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
#include <iterator>
#include <vector>

#include <gtest/gtest.h>

#include "kitepp.hpp"

namespace kiteconnect {

namespace kc = kiteconnect;

TEST(kWSTest, binaryParsingTest) {
    kc::ticker kws("apikey123");
    std::ifstream dataFile("../../tests/mock_custom/websocket_ticks.bin");
    ASSERT_TRUE(dataFile);
    std::vector<char> data(std::istreambuf_iterator<char>(dataFile), {});

    std::vector<kc::tick> ticks = kws.parseBinaryMessage(data.data(), data.size());

    ASSERT_EQ(ticks.size(), 2);
    kc::tick tick1 = ticks[0];
    EXPECT_EQ(tick1.mode, "full");
    EXPECT_EQ(tick1.instrumentToken, 408065);
    EXPECT_EQ(tick1.isTradable, true);
    EXPECT_EQ(tick1.timestamp, 1612777255);
    EXPECT_EQ(tick1.lastTradeTime, 1612777255);
    EXPECT_DOUBLE_EQ(tick1.lastPrice, 1299.05);
    EXPECT_EQ(tick1.lastTradedQuantity, 2);
    EXPECT_EQ(tick1.totalBuyQuantity, 390117);
    EXPECT_EQ(tick1.totalSellQuantity, 410173);
    EXPECT_EQ(tick1.volumeTraded, 6065675);
    EXPECT_DOUBLE_EQ(tick1.averageTradePrice, 1290.29);
    EXPECT_EQ(tick1.oi, 0);
    EXPECT_EQ(tick1.oiDayHigh, 0);
    EXPECT_EQ(tick1.oiDayLow, 0);
    EXPECT_DOUBLE_EQ(tick1.netChange, 2.1185441396116693);

    EXPECT_DOUBLE_EQ(tick1.ohlc.open, 1285.5);
    EXPECT_DOUBLE_EQ(tick1.ohlc.high, 1305.90);
    EXPECT_DOUBLE_EQ(tick1.ohlc.low, 1275.5);
    EXPECT_DOUBLE_EQ(tick1.ohlc.close, 1272.1);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.buy[0].price, 1299);
    EXPECT_EQ(tick1.marketDepth.buy[0].quantity, 2098);
    EXPECT_EQ(tick1.marketDepth.buy[0].orders, 10);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.buy[1].price, 1298.90);
    EXPECT_EQ(tick1.marketDepth.buy[1].quantity, 6);
    EXPECT_EQ(tick1.marketDepth.buy[1].orders, 2);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.buy[2].price, 1298.8);
    EXPECT_EQ(tick1.marketDepth.buy[2].quantity, 135);
    EXPECT_EQ(tick1.marketDepth.buy[2].orders, 5);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.buy[3].price, 1298.75);
    EXPECT_EQ(tick1.marketDepth.buy[3].quantity, 1);
    EXPECT_EQ(tick1.marketDepth.buy[3].orders, 1);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.buy[4].price, 1298.7);
    EXPECT_EQ(tick1.marketDepth.buy[4].quantity, 55);
    EXPECT_EQ(tick1.marketDepth.buy[4].orders, 2);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.sell[0].price, 1299.05);
    EXPECT_EQ(tick1.marketDepth.sell[0].quantity, 335);
    EXPECT_EQ(tick1.marketDepth.sell[0].orders, 7);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.sell[1].price, 1299.1);
    EXPECT_EQ(tick1.marketDepth.sell[1].quantity, 1);
    EXPECT_EQ(tick1.marketDepth.sell[1].orders, 1);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.sell[2].price, 1299.4);
    EXPECT_EQ(tick1.marketDepth.sell[2].quantity, 45);
    EXPECT_EQ(tick1.marketDepth.sell[2].orders, 2);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.sell[3].price, 1299.45);
    EXPECT_EQ(tick1.marketDepth.sell[3].quantity, 120);
    EXPECT_EQ(tick1.marketDepth.sell[3].orders, 2);

    EXPECT_DOUBLE_EQ(tick1.marketDepth.sell[4].price, 1299.5);
    EXPECT_EQ(tick1.marketDepth.sell[4].quantity, 233);
    EXPECT_EQ(tick1.marketDepth.sell[4].orders, 3);

    kc::tick tick2 = ticks[1];
    EXPECT_EQ(tick2.mode, "full");
    EXPECT_EQ(tick2.instrumentToken, 2953217);
    EXPECT_EQ(tick2.isTradable, true);
    EXPECT_EQ(tick2.timestamp, 1612777254);
    EXPECT_EQ(tick2.lastTradeTime, 1612777254);
    EXPECT_DOUBLE_EQ(tick2.lastPrice, 3209.40);
    EXPECT_EQ(tick2.lastTradedQuantity, 148);
    EXPECT_EQ(tick2.totalBuyQuantity, 146646);
    EXPECT_EQ(tick2.totalSellQuantity, 145876);
    EXPECT_EQ(tick2.volumeTraded, 2261635);
    EXPECT_DOUBLE_EQ(tick2.averageTradePrice, 3188.32);
    EXPECT_EQ(tick2.oi, 0);
    EXPECT_EQ(tick2.oiDayHigh, 0);
    EXPECT_EQ(tick2.oiDayLow, 0);
    EXPECT_DOUBLE_EQ(tick2.netChange, 1.6292214886239578);

    EXPECT_DOUBLE_EQ(tick2.ohlc.open, 3189.5);
    EXPECT_DOUBLE_EQ(tick2.ohlc.high, 3226);
    EXPECT_DOUBLE_EQ(tick2.ohlc.low, 3155.15);
    EXPECT_DOUBLE_EQ(tick2.ohlc.close, 3157.95);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.buy[0].price, 3209.7);
    EXPECT_EQ(tick2.marketDepth.buy[0].quantity, 1);
    EXPECT_EQ(tick2.marketDepth.buy[0].orders, 1);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.buy[1].price, 3209.4);
    EXPECT_EQ(tick2.marketDepth.buy[1].quantity, 300);
    EXPECT_EQ(tick2.marketDepth.buy[1].orders, 1);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.buy[2].price, 3209.25);
    EXPECT_EQ(tick2.marketDepth.buy[2].quantity, 138);
    EXPECT_EQ(tick2.marketDepth.buy[2].orders, 1);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.buy[3].price, 3209.15);
    EXPECT_EQ(tick2.marketDepth.buy[3].quantity, 31);
    EXPECT_EQ(tick2.marketDepth.buy[3].orders, 1);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.buy[4].price, 3209.05);
    EXPECT_EQ(tick2.marketDepth.buy[4].quantity, 52);
    EXPECT_EQ(tick2.marketDepth.buy[4].orders, 1);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.sell[0].price, 3209.95);
    EXPECT_EQ(tick2.marketDepth.sell[0].quantity, 16);
    EXPECT_EQ(tick2.marketDepth.sell[0].orders, 2);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.sell[1].price, 3210.05);
    EXPECT_EQ(tick2.marketDepth.sell[1].quantity, 35);
    EXPECT_EQ(tick2.marketDepth.sell[1].orders, 1);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.sell[2].price, 3210.1);
    EXPECT_EQ(tick2.marketDepth.sell[2].quantity, 47);
    EXPECT_EQ(tick2.marketDepth.sell[2].orders, 2);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.sell[3].price, 3210.15);
    EXPECT_EQ(tick2.marketDepth.sell[3].quantity, 30);
    EXPECT_EQ(tick2.marketDepth.sell[3].orders, 3);

    EXPECT_DOUBLE_EQ(tick2.marketDepth.sell[4].price, 3210.2);
    EXPECT_EQ(tick2.marketDepth.sell[4].quantity, 670);
    EXPECT_EQ(tick2.marketDepth.sell[4].orders, 1);
};
} // namespace kiteconnect
