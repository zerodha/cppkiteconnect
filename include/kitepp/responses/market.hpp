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
#include <string>

#include "../utils.hpp"
#include "rapidcsv/src/rapidcsv.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/// Represents OHLC information of an instrument.
struct ohlc {
    ohlc() = default;
    explicit ohlc(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        open = utils::json::get<double>(val, "open");
        high = utils::json::get<double>(val, "high");
        low = utils::json::get<double>(val, "low");
        close = utils::json::get<double>(val, "close");
    };

    double open = -1;
    double high = -1;
    double low = -1;
    double close = -1;
};

/// Represents market depth of an instrument.
struct depth {
    depth() = default;
    explicit depth(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        price = utils::json::get<double>(val, "price");
        quantity = utils::json::get<int>(val, "quantity");
        orders = utils::json::get<int>(val, "orders");
    };

    int quantity = -1;
    int orders = 0;
    double price = -1;
};

/// Represents quote informating of an instrument.
struct quote {
    quote() = default;
    explicit quote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        instrumentToken = utils::json::get<uint32_t>(val, "instrument_token");
        timestamp = utils::json::get<string>(val, "timestamp");
        lastPrice = utils::json::get<double>(val, "last_price");
        lastQuantity = utils::json::get<int>(val, "last_quantity");
        lastTradeTime = utils::json::get<string>(val, "last_trade_time");
        averagePrice = utils::json::get<double>(val, "average_price");
        volume = utils::json::get<int64_t>(val, "volume");
        buyQuantity = utils::json::get<int>(val, "buy_quantity");
        sellQuantity = utils::json::get<int>(val, "sell_quantity");
        OHLC = utils::json::get<utils::json::JsonObject, ohlc>(val, "ohlc");
        netChange = utils::json::get<double>(val, "net_change");
        OI = utils::json::get<double>(val, "oi");
        OIDayHigh = utils::json::get<double>(val, "oi_day_high");
        OIDayLow = utils::json::get<double>(val, "oi_day_low");
        lowerCircuitLimit =
            utils::json::get<double>(val, "lower_circuit_limit");
        upperCircuitLimit =
            utils::json::get<double>(val, "upper_circuit_limit");
        marketDepth =
            utils::json::get<utils::json::JsonObject, mDepth>(val, "depth");
    };

    uint32_t instrumentToken = 0;
    int64_t volume = -1;
    int buyQuantity = -1;
    int sellQuantity = -1;
    int lastQuantity = -1;
    double lastPrice = -1;
    double averagePrice = -1;
    double netChange = -1;
    double OI = -1;
    double OIDayHigh = -1;
    double OIDayLow = -1;
    double lowerCircuitLimit = -1;
    double upperCircuitLimit = -1;
    string timestamp;
    string lastTradeTime;
    ohlc OHLC;
    struct mDepth {
        mDepth() = default;
        explicit mDepth(const rj::Value::Object& val) { parse(val); };

        void parse(const rj::Value::Object& val) {
            rj::Value buyDepthVal(rj::kArrayType);
            utils::json::get<utils::json::JsonArray>(val, buyDepthVal, "buy");
            for (auto& i : buyDepthVal.GetArray()) {
                buy.emplace_back(i.GetObject());
            };

            rj::Value sellDepthVal(rj::kArrayType);
            utils::json::get<utils::json::JsonArray>(val, sellDepthVal, "sell");
            for (auto& i : sellDepthVal.GetArray()) {
                sell.emplace_back(i.GetObject());
            };
        }

        std::vector<depth> buy;
        std::vector<depth> sell;
    } marketDepth;
};

/// Represents a ohlc quote information of an instrument.
struct ohlcQuote {
    ohlcQuote() = default;
    explicit ohlcQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        instrumentToken = utils::json::get<uint32_t>(val, "instrument_token");
        lastPrice = utils::json::get<double>(val, "last_price");
        OHLC = utils::json::get<utils::json::JsonObject, ohlc>(val, "ohlc");
    };

    uint32_t instrumentToken = 0;
    double lastPrice = -1;
    ohlc OHLC;
};

/// Represents a LTP quote information of an instrument.
struct ltpQuote {
    ltpQuote() = default;
    explicit ltpQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        instrumentToken = utils::json::get<uint32_t>(val, "instrument_token");
        lastPrice = utils::json::get<double>(val, "last_price");
    };

    uint32_t instrumentToken = 0;
    double lastPrice = -1;
};

/// represents parameters required for the `getHistoricalData` function
struct historicalDataParams {
    GENERATE_FLUENT_METHOD(
        historicalDataParams, uint32_t, instrumentToken, InstrumentToken);
    GENERATE_FLUENT_METHOD(historicalDataParams, bool, continuous, Continuous);
    GENERATE_FLUENT_METHOD(historicalDataParams, bool, oi, Oi);
    GENERATE_FLUENT_METHOD(historicalDataParams, const string&, from, From);
    GENERATE_FLUENT_METHOD(historicalDataParams, const string&, to, To);
    GENERATE_FLUENT_METHOD(
        historicalDataParams, const string&, interval, Interval);

    uint32_t instrumentToken = 0;
    bool continuous = false;
    bool oi = false;
    string from;
    string to;
    string interval;
};

/// Represents historical data of an instrument.
struct historicalData {
    historicalData() = default;
    explicit historicalData(const rj::Value::Array& val) { parse(val); };

    void parse(const rj::Value::Array& val) {
        // if the sent value doesn't have a floating point (this time),
        // GetDouble() will throw error
        static auto getDouble = [](rj::Value& val) -> double {
            if (val.IsDouble()) { return val.GetDouble(); };
            if (val.IsInt()) { return val.GetInt(); };
            throw libException("type isn't double");
        };
        datetime = val[DATETIME_IDX].GetString();
        open = getDouble(val[OPEN_IDX]);
        high = getDouble(val[HIGH_IDX]);
        low = getDouble(val[LOW_IDX]);
        close = getDouble(val[CLOSE_IDX]);
        volume = val[VOLUME_IDX].GetInt64();
        if (val.Size() > OI_IDX) { OI = val[OI_IDX].GetInt64(); };
    };

    int64_t volume = -1;
    int64_t OI = -1;
    double open = -1;
    double high = -1;
    double low = -1;
    double close = -1;
    string datetime;
    static constexpr uint8_t DATETIME_IDX = 0;
    static constexpr uint8_t OPEN_IDX = 1;
    static constexpr uint8_t HIGH_IDX = 2;
    static constexpr uint8_t LOW_IDX = 3;
    static constexpr uint8_t CLOSE_IDX = 4;
    static constexpr uint8_t VOLUME_IDX = 5;
    static constexpr uint8_t OI_IDX = 6;
};

/// Parameters required by the `getOrderMargins` method.
struct orderMarginsParams {
    orderMarginsParams() = default;

    GENERATE_FLUENT_METHOD(orderMarginsParams, double, quantity, Quantity);
    GENERATE_FLUENT_METHOD(orderMarginsParams, double, price, Price);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, tradingsymbol, Tradingsymbol);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, orderType, OrderType);

    double quantity = 0;
    double price = 0;
    double triggerPrice = 0;
    string exchange;
    string tradingsymbol;
    string transactionType;
    string variety;
    string product;
    string orderType;
};

/// Represents margins required for placing an order.
struct orderMargins {
    orderMargins() = default;
    explicit orderMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        type = utils::json::get<string>(val, "type");
        tradingSymbol = utils::json::get<string>(val, "tradingsymbol");
        exchange = utils::json::get<string>(val, "exchange");
        span = utils::json::get<double>(val, "span");
        exposure = utils::json::get<double>(val, "exposure");
        optionPremium = utils::json::get<double>(val, "option_premium");
        additional = utils::json::get<double>(val, "additional");
        bo = utils::json::get<double>(val, "bo");
        cash = utils::json::get<double>(val, "cash");
        var = utils::json::get<double>(val, "var");
        pnl = utils::json::get<utils::json::JsonObject, PNL>(val, "pnl");
        total = utils::json::get<double>(val, "total");
    };

    double span = -1;
    double exposure = -1;
    double optionPremium = -1;
    double additional = -1;
    double bo = -1;
    double cash = -1;
    double var = -1;
    double total = -1;
    string type;
    string tradingSymbol;
    string exchange;
    struct PNL {
        PNL() = default;
        explicit PNL(const rj::Value::Object& val) { parse(val); };

        void parse(const rj::Value::Object& val) {
            realised = utils::json::get<double>(val, "realised");
            unrealised = utils::json::get<double>(val, "unrealised");
        };

        double realised = -1;
        double unrealised = -1;
    } pnl;
};
/// Represents information of an instrument.
struct instrument {
    instrument() = default;
    explicit instrument(const std::vector<string>& row) { parse(row); };

    void parse(const std::vector<string>& tokens) {
        static const auto toInt = [](const string& str) -> int {
            return (str.empty()) ? 0 : std::stoi(str);
        };
        static const auto toUint32 = [](const string& str) -> uint32_t {
            return (str.empty()) ? 0 : std::stoul(str);
        };
        static const auto toDouble = [](const string& str) -> double {
            return (str.empty()) ? 0.0 : std::stod(str);
        };

        instrumentToken = toUint32(tokens[INSTRUMENT_TOKEN_IDX]);
        exchangeToken = toInt(tokens[EXCHANGE_TOKEN_IDX]);
        tradingsymbol = tokens[TRADINGSYMBOL_IDX];
        name = tokens[NAME_IDX];
        lastPrice = toDouble(tokens[LAST_PRICE_IDX]);
        expiry = tokens[EXPIRY_IDX];
        strikePrice = toDouble(tokens[STRIKE_PRICE_IDX]);
        tickSize = toDouble(tokens[TICK_SIZE_IDX]);
        lotSize = toDouble(tokens[LOT_SIZE_IDX]);
        instrumentType = tokens[INSTRUMENT_TYPE_IDX];
        segment = tokens[SEGMENT_IDX];
        exchange = tokens[EXCHANGE_IDX];
    };

    uint32_t instrumentToken = 0;
    int exchangeToken = -1;
    double lastPrice = -1;
    double strikePrice = -1;
    double tickSize = -1;
    double lotSize = -1;
    string tradingsymbol;
    string name;
    string expiry;
    string instrumentType;
    string segment;
    string exchange;

    static constexpr uint8_t INSTRUMENT_TOKEN_IDX = 0;
    static constexpr uint8_t EXCHANGE_TOKEN_IDX = 1;
    static constexpr uint8_t TRADINGSYMBOL_IDX = 2;
    static constexpr uint8_t NAME_IDX = 3;
    static constexpr uint8_t LAST_PRICE_IDX = 4;
    static constexpr uint8_t EXPIRY_IDX = 5;
    static constexpr uint8_t STRIKE_PRICE_IDX = 6;
    static constexpr uint8_t TICK_SIZE_IDX = 7;
    static constexpr uint8_t LOT_SIZE_IDX = 8;
    static constexpr uint8_t INSTRUMENT_TYPE_IDX = 9;
    static constexpr uint8_t SEGMENT_IDX = 10;
    static constexpr uint8_t EXCHANGE_IDX = 11;
};

} // namespace kiteconnect