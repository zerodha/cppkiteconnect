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

/**
 * @file responses.hpp
 * @brief This file has all the structs returned by kitepp
 */

#pragma once

#include <iostream> //debugging
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rjutils.hpp"
#include "utils.hpp"

#include "responses/responses.hpp"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace rju = kc::rjutils;

/// ohlc strcut
struct ohlc {

    ohlc() = default;

    explicit ohlc(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, open, "open");
        rju::_getIfExists(val, high, "high");
        rju::_getIfExists(val, low, "low");
        rju::_getIfExists(val, close, "close");
    };

    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
};

/// represents market depth
struct depth {

    depth() = default;

    explicit depth(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, price, "price");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, orders, "orders");
    };

    double price = 0.0;
    int quantity = 0;
    int orders = 0;
};

/// represents full quote respone
struct quote {

    quote() = default;

    explicit quote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, instrumentToken, "instrument_token");
        rju::_getIfExists(val, timestamp, "timestamp");
        rju::_getIfExists(val, lastPrice, "last_price");
        rju::_getIfExists(val, lastQuantity, "last_quantity");
        rju::_getIfExists(val, lastTradeTime, "last_trade_time");
        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, volume, "volume");
        rju::_getIfExists(val, buyQuantity, "buy_quantity");
        rju::_getIfExists(val, sellQuantity, "sell_quantity");

        rj::Value ohlcVal(rj::kObjectType);
        rju::_getIfExists(val, ohlcVal, "ohlc", rju::_RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());

        rju::_getIfExists(val, netChange, "net_change");
        rju::_getIfExists(val, OI, "oi");
        rju::_getIfExists(val, OIDayHigh, "oi_day_high");
        rju::_getIfExists(val, OIDayLow, "oi_day_low");
        rju::_getIfExists(val, lowerCircuitLimit, "lower_circuit_limit");
        rju::_getIfExists(val, upperCircuitLimit, "upper_circuit_limit");

        rj::Value tmpVal(rj::kObjectType);
        rju::_getIfExists(val, tmpVal, "depth", rju::_RJValueType::Object);
        auto depthVal = tmpVal.GetObject();

        rj::Value buyDepthVal(rj::kArrayType);
        rju::_getIfExists(depthVal, buyDepthVal, "buy", rju::_RJValueType::Array);
        for (auto& i : buyDepthVal.GetArray()) { marketDepth.buy.emplace_back(i.GetObject()); };

        rj::Value sellDepthVal(rj::kArrayType);
        rju::_getIfExists(depthVal, sellDepthVal, "sell", rju::_RJValueType::Array);
        for (auto& i : sellDepthVal.GetArray()) { marketDepth.sell.emplace_back(i.GetObject()); };
    };

    int instrumentToken = 0;
    string timestamp;
    double lastPrice = 0.0;
    int lastQuantity = 0;
    string lastTradeTime;
    double averagePrice = 0.0;
    int volume = 0;
    int buyQuantity = 0;
    int sellQuantity = 0;
    ohlc OHLC;

    double netChange = 0.0;
    double OI = 0.0;
    double OIDayHigh = 0.0;
    double OIDayLow = 0.0;
    double lowerCircuitLimit = 0.0;
    double upperCircuitLimit = 0.0;

    struct mDepth {

        std::vector<depth> buy;
        std::vector<depth> sell;

    } marketDepth;
};

/// represents ohlc quote respone
struct OHLCQuote {

    OHLCQuote() = default;

    explicit OHLCQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, instrumentToken, "instrument_token");
        rju::_getIfExists(val, lastPrice, "last_price");

        rj::Value ohlcVal(rj::kObjectType);
        rju::_getIfExists(val, ohlcVal, "ohlc", rju::_RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());
    };

    int instrumentToken = 0;
    double lastPrice = 0.0;
    ohlc OHLC;
};

/// represents ltp quote respone
struct LTPQuote {

    LTPQuote() = default;

    explicit LTPQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, instrumentToken, "instrument_token");
        rju::_getIfExists(val, lastPrice, "last_price");
    };

    int instrumentToken = 0;
    double lastPrice = 0.0;
};

/// orderMarginsParams represents a position in the Margin Calculator API
struct orderMarginsParams {

    orderMarginsParams() = default;

    string exchange;
    string tradingsymbol;
    string transactionType;
    string variety;
    string product;
    string orderType;
    double quantity = 0.0;
    double price = 0.0;
    double triggerPrice = 0.0;
};

/// ordersMargins represents response from the Margin Calculator API.
struct orderMargins {

    orderMargins() = default;

    explicit orderMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, type, "type");
        rju::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, SPAN, "span");
        rju::_getIfExists(val, exposure, "exposure");
        rju::_getIfExists(val, optionPremium, "option_premium");
        rju::_getIfExists(val, additional, "additional");
        rju::_getIfExists(val, BO, "bo");
        rju::_getIfExists(val, cash, "cash");
        rju::_getIfExists(val, VAR, "var");

        rj::Value pnlVal(rj::kObjectType);
        rju::_getIfExists(val, pnlVal, "pnl", rju::_RJValueType::Object);
        pnl.parse(pnlVal.GetObject());

        rju::_getIfExists(val, total, "total");
    };

    string type;
    string tradingSymbol;
    string exchange;

    double SPAN = 0.0;
    double exposure = 0.0;
    double optionPremium = 0.0;
    double additional = 0.0;
    double BO = 0.0;
    double cash = 0.0;
    double VAR = 0.0;
    struct PNL {

        PNL() = default;

        explicit PNL(const rj::Value::Object& val) { parse(val); };

        void parse(const rj::Value::Object& val) {

            rju::_getIfExists(val, realised, "realised");
            rju::_getIfExists(val, unrealised, "unrealised");
        };

        double realised = 0.0;
        double unrealised = 0.0;
    } pnl;
    double total = 0.0;
};

/// instrument represents individual instrument response.
struct instrument {

    instrument() = default;

    explicit instrument(const string& val) { parse(val); };

    void parse(const string& val) {

        std::vector<string> tokens = kc::_split(val, ',');

        static const auto toInt = [](const string& str) -> int { return (str.empty()) ? 0 : std::stoi(str); };
        static const auto toDouble = [](const string& str) -> double { return (str.empty()) ? 0.0 : std::stod(str); };

        instrumentToken = toInt(tokens[0]);
        exchangeToken = toInt(tokens[1]);
        tradingsymbol = tokens[2];
        name = tokens[3];
        lastPrice = toDouble(tokens[4]);
        expiry = tokens[5];
        strikePrice = toDouble(tokens[6]);
        tickSize = toDouble(tokens[7]);
        lotSize = toDouble(tokens[8]);
        instrumentType = tokens[9];
        segment = tokens[10];
        exchange = tokens[11];
    };

    int instrumentToken = 0;
    int exchangeToken = 0;
    string tradingsymbol;
    string name;
    double lastPrice = 0.0;
    string expiry;
    double strikePrice = 0.0;
    double tickSize = 0.0;
    double lotSize = 0.0;
    string instrumentType;
    string segment;
    string exchange;
};

/// MFInstrument represents individual mfinstrument response.
struct MFInstrument {

    MFInstrument() = default;

    explicit MFInstrument(const string& val) { parse(val); };

    void parse(const string& val) {

        std::vector<string> tokens = kc::_split(val, ',');

        static const auto toDouble = [](const string& str) -> double { return (str.empty()) ? 0.0 : std::stod(str); };

        tradingsymbol = tokens[0];
        AMC = tokens[1];
        name = tokens[2];
        purchaseAllowed = static_cast<bool>(std::stoi(tokens[3]));
        redemtpionAllowed = static_cast<bool>(std::stoi(tokens[4]));
        minimumPurchaseAmount = toDouble(tokens[5]);
        purchaseAmountMultiplier = toDouble(tokens[6]);
        minimumAdditionalPurchaseAmount = toDouble(tokens[7]);
        minimumRedemptionQuantity = toDouble(tokens[8]);
        redemptionQuantityMultiplier = toDouble(tokens[9]);
        dividendType = tokens[10];
        schemeType = tokens[11];
        plan = tokens[12];
        settlementType = tokens[13];
        lastPrice = toDouble(tokens[14]);
        lastPriceDate = tokens[15];
    };

    string tradingsymbol;
    string AMC;
    string name;
    bool purchaseAllowed;
    bool redemtpionAllowed;
    double minimumPurchaseAmount = 0.0;
    double purchaseAmountMultiplier = 0.0;
    double minimumAdditionalPurchaseAmount = 0.0;
    double minimumRedemptionQuantity = 0.0;
    double redemptionQuantityMultiplier = 0.0;
    string dividendType;
    string schemeType;
    string plan;
    string settlementType;
    double lastPrice = 0.0;
    string lastPriceDate;
};

// kiteWS responses

/// Reoresents a single entry in market depth returned by kWS
struct depthWS {

    double price = 0.0;
    int32_t quantity = 0;
    int16_t orders = 0;
};

/// Represents a single tick returned by kWS
struct tick {

    string mode;
    int32_t instrumentToken = 0;
    bool isTradable;
    // xbool isIndex;

    int32_t timestamp = 0;
    int32_t lastTradeTime = 0;
    double lastPrice = 0.0;
    int32_t lastTradedQuantity = 0;
    int32_t totalBuyQuantity = 0;
    int32_t totalSellQuantity = 0;
    int32_t volumeTraded = 0;
    double averageTradePrice = 0.0;
    int32_t OI = 0;
    int32_t OIDayHigh = 0;
    int32_t OIDayLow = 0;
    double netChange = 0.0;

    // OHLC  OHLC
    struct ohlc {
        double open = 0.0;
        double high = 0.0;
        double low = 0.0;
        double close = 0.0;
    } OHLC;

    // Depth Depth
    struct m_depth {
        std::vector<depthWS> buy;
        std::vector<depthWS> sell;
    } marketDepth;
};

/// Represents postback sent via websockets
struct postback {

    postback() = default;

    explicit postback(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, orderID, "order_id");
        rju::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rju::_getIfExists(val, placedBy, "placed_by");
        rju::_getIfExists(val, status, "status");
        rju::_getIfExists(val, statusMessage, "status_message");

        rju::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, orderType, "order_type");
        rju::_getIfExists(val, transactionType, "transaction_type");
        rju::_getIfExists(val, validity, "validity");
        rju::_getIfExists(val, product, "product");

        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, price, "price");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, filledQuantity, "filled_quantity");
        rju::_getIfExists(val, unfilledQuantity, "unfilled_quantity");
        rju::_getIfExists(val, triggerPrice, "trigger_price");
        rju::_getIfExists(val, userID, "user_id");
        rju::_getIfExists(val, orderTimestamp, "order_timestamp");
        rju::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rju::_getIfExists(val, checksum, "checksum");
    };

    string orderID;
    string exchangeOrderID;
    string placedBy;
    string status;
    string statusMessage;

    string tradingSymbol;
    string exchange;
    string orderType;
    string transactionType;
    string validity;
    string product;

    double averagePrice = 0.0;
    double price = 0.0;
    int quantity = 0;
    int filledQuantity = 0;
    int unfilledQuantity = 0;
    double triggerPrice = 0.0;
    string userID;
    string orderTimestamp;
    string exchangeTimestamp;
    string checksum;
};

} // namespace kiteconnect
