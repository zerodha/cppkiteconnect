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

TEST(kiteTest, getQuoteTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/quote.json");
    const std::vector<string> SYMBOLS = { "NSE:INFY", "NSE:TCS", "NSE:M&M" };
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(
            utils::http::endpoint { utils::http::METHOD::GET, "/quote?{0}" },
            utils::http::Params {},
            utils::FmtArgs { "i=NSE:INFY&i=NSE:TCS&i=NSE:M%26M" }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::unordered_map<string, kc::quote> QUOTES = Kite.getQuote(SYMBOLS);

    kc::quote Quote = QUOTES.at("NSE:INFY");
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
    const string JSON = kc::test::readFile("../tests/mock_responses/ohlc.json");
    const std::vector<string> SYMBOLS = { "NSE:INFY", "NSE:TCS", "NSE:M&M" };
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(
        Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                          "/quote/ohlc?{0}" },
                  utils::http::Params {},
                  utils::FmtArgs { "i=NSE:INFY&i=NSE:TCS&i=NSE:M%26M" }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::unordered_map<string, kc::ohlcQuote> QUOTES =
        Kite.getOhlc(SYMBOLS);

    kc::ohlcQuote Quote = QUOTES.at("NSE:INFY");
    EXPECT_EQ(Quote.instrumentToken, 408065);
    EXPECT_DOUBLE_EQ(Quote.lastPrice, 1075);
    EXPECT_DOUBLE_EQ(Quote.OHLC.open, 1085.8);
    EXPECT_DOUBLE_EQ(Quote.OHLC.high, 1085.9);
    EXPECT_DOUBLE_EQ(Quote.OHLC.low, 1070.9);
    EXPECT_DOUBLE_EQ(Quote.OHLC.close, 1075.8);
}

TEST(kiteTest, getLTPTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/ltp.json");
    const std::vector<string> SYMBOLS = { "NSE:INFY", "NSE:TCS", "NSE:M&M" };
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(
        Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                          "/quote/ltp?{0}" },
                  utils::http::Params {},
                  utils::FmtArgs { "i=NSE:INFY&i=NSE:TCS&i=NSE:M%26M" }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::unordered_map<string, kc::ltpQuote> QUOTES =
        Kite.getLtp(SYMBOLS);

    kc::ltpQuote Quote = QUOTES.at("NSE:INFY");
    EXPECT_EQ(Quote.instrumentToken, 408065);
    EXPECT_DOUBLE_EQ(Quote.lastPrice, 1074.35);
}

TEST(kiteTest, getHistoricalDataTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/historical_minute.json");
    const std::vector<string> SYMBOLS = { "NSE:INFY", "NSE:TCS" };
    constexpr int INSTRUMENT_TOKEN = 5633;
    const string INTERVAL = "minute";
    const string FROM = "2017-12-15+09:15:00";
    const string TO = "2017-12-15+09:20:00";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/instruments/historical/{0}/"
                                  "{1}?from={2}&to={3}&continuous={4}&oi={5}" },
                          utils::http::Params {},
                          utils::FmtArgs { std::to_string(INSTRUMENT_TOKEN),
                              INTERVAL, FROM, TO, "0", "0" }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::historicalData> DATA =
        Kite.getHistoricalData(kc::historicalDataParams()
                                   .InstrumentToken(INSTRUMENT_TOKEN)
                                   .Interval(INTERVAL)
                                   .From(FROM)
                                   .To(TO));

    ASSERT_EQ(DATA.size(), 6);
    kc::historicalData data1 = DATA[0];
    EXPECT_EQ(data1.datetime, "2017-12-15T09:15:00+0530");
    EXPECT_DOUBLE_EQ(data1.open, 1704.5);
    EXPECT_DOUBLE_EQ(data1.high, 1705);
    EXPECT_DOUBLE_EQ(data1.low, 1699.25);
    EXPECT_DOUBLE_EQ(data1.close, 1702.8);
    EXPECT_EQ(data1.volume, 2499);
    EXPECT_DOUBLE_EQ(data1.OI, -1);

    kc::historicalData data2 = DATA[1];
    EXPECT_EQ(data2.datetime, "2017-12-15T09:16:00+0530");
    EXPECT_DOUBLE_EQ(data2.open, 1702);
    EXPECT_DOUBLE_EQ(data2.high, 1702);
    EXPECT_DOUBLE_EQ(data2.low, 1698.15);
    EXPECT_DOUBLE_EQ(data2.close, 1698.15);
    EXPECT_EQ(data2.volume, 1271);
    EXPECT_DOUBLE_EQ(data2.OI, -1);

    kc::historicalData data3 = DATA[2];
    EXPECT_EQ(data3.datetime, "2017-12-15T09:17:00+0530");
    EXPECT_DOUBLE_EQ(data3.open, 1698.15);
    EXPECT_DOUBLE_EQ(data3.high, 1700.25);
    EXPECT_DOUBLE_EQ(data3.low, 1698);
    EXPECT_DOUBLE_EQ(data3.close, 1699.25);
    EXPECT_EQ(data3.volume, 831);
    EXPECT_DOUBLE_EQ(data3.OI, -1);
}

TEST(kiteTest, getInstrumentsTest) {
    const string CSV =
        kc::test::readFile("../tests/mock_responses/instruments_all.csv");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(
        Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                          "/instruments", utils::http::CONTENT_TYPE::NON_JSON },
                  utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(Return(
            ByMove(utils::http::response(utils::http::code::OK, CSV, false))));

    std::vector<kc::instrument> INSTRUMENTS = Kite.getInstruments();

    ASSERT_EQ(INSTRUMENTS.size(), 99);
    kc::instrument instrument1 = INSTRUMENTS[0];
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

    kc::instrument instrument2 = INSTRUMENTS[1];
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

TEST(kiteTest, getInstrumentsExchangeTest) {
    const string CSV =
        kc::test::readFile("../tests/mock_responses/instruments_all.csv");
    const string EXCHANGE = "NSE";
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                    "/instruments/{0}", utils::http::CONTENT_TYPE::NON_JSON },
            utils::http::Params {}, utils::FmtArgs { EXCHANGE }))
        .Times(1)
        .WillOnce(Return(
            ByMove(utils::http::response(utils::http::code::OK, CSV, false))));

    std::vector<kc::instrument> INSTRUMENTS = Kite.getInstruments(EXCHANGE);

    ASSERT_EQ(INSTRUMENTS.size(), 99);
    kc::instrument instrument1 = INSTRUMENTS[0];
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

    kc::instrument instrument2 = INSTRUMENTS[1];
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
