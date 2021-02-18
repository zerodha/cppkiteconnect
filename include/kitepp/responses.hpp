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

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace rju = kc::rjutils;

/// userProfile represents a user's personal and financial profile.
struct userProfile {

    userProfile() = default;

    explicit userProfile(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, userName, "user_name");
        rju::_getIfExists(val, userShortName, "user_shortname");
        rju::_getIfExists(val, avatarURL, "avatar_url");
        rju::_getIfExists(val, userType, "user_type");
        rju::_getIfExists(val, email, "email");
        // x rju::_getIfExists(val, phone, "phone");
        rju::_getIfExists(val, broker, "broker");
        rju::_getIfExists(val, products, "products");
        rju::_getIfExists(val, orderTypes, "order_types");
        rju::_getIfExists(val, exchanges, "exchanges");
    };

    string userName;
    string userShortName;
    string avatarURL;
    string userType;
    string email;
    string phone;
    string broker;
    std::vector<string> products;
    std::vector<string> orderTypes;
    std::vector<string> exchanges;
};

/// tokens received after successfull authentication
struct userTokens {

    userTokens() = default;

    explicit userTokens(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, userID, "user_id");
        rju::_getIfExists(val, accessToken, "access_token");
        rju::_getIfExists(val, refreshToken, "refresh_token");
    };

    string userID;
    string accessToken;
    string refreshToken;
};

/// userSession represents the response after a successful authentication.
struct userSession {

    userSession() = default;

    explicit userSession(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        profile.parse(val);
        tokens.parse(val);
        rju::_getIfExists(val, apiKey, "api_key");
        rju::_getIfExists(val, publicToken, "public_token");
        rju::_getIfExists(val, loginTime, "login_time");
    };

    userProfile profile;
    userTokens tokens;

    string apiKey;
    string publicToken;
    string loginTime;
};

/// availableMargins represents the available margins from the margins response for a single segment.
struct availableMargins {

    availableMargins() = default;

    explicit availableMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        double debugval = val["cash"].GetDouble();

        rju::_getIfExists(val, adHocMargin, "adhoc_margin");
        rju::_getIfExists(val, cash, "cash");
        rju::_getIfExists(val, collateral, "collateral");
        rju::_getIfExists(val, intradayPayin, "intraday_payin");
    };

    double adHocMargin = 0.0;
    double cash = 0.0;
    double collateral = 0.0;
    double intradayPayin = 0.0;
};

/// usedMargins represents the used margins from the margins response for a single segment.
struct usedMargins {

    usedMargins() = default;

    explicit usedMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        rju::_getIfExists(val, debits, "debits");
        rju::_getIfExists(val, exposure, "exposure");
        rju::_getIfExists(val, M2MRealised, "m2m_realised");
        rju::_getIfExists(val, M2MUnrealised, "m2m_unrealised");
        rju::_getIfExists(val, optionPremium, "option_premium");
        rju::_getIfExists(val, payout, "payout");
        rju::_getIfExists(val, span, "span");
        rju::_getIfExists(val, holdingSales, "holding_sales");
        rju::_getIfExists(val, turnover, "turnover");
    };

    double debits = 0.0;
    double exposure = 0.0;
    double M2MRealised = 0.0;
    double M2MUnrealised = 0.0;
    double optionPremium = 0.0;
    double payout = 0.0;
    double span = 0.0;
    double holdingSales = 0.0;
    double turnover = 0.0;
};

/// margins represents the user margins for a segment.
struct margins {

    margins() = default;

    explicit margins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, enabled, "enabled");
        rju::_getIfExists(val, net, "net");

        rj::Value avlVal(rj::kObjectType);
        rju::_getIfExists(val, avlVal, "available", rju::_RJValueType::Object);
        rj::Value usedVal(rj::kObjectType);
        rju::_getIfExists(val, usedVal, "utilised", rju::_RJValueType::Object);

        available.parse(avlVal.GetObject());
        used.parse(usedVal.GetObject());
    };

    bool enabled = false;
    double net = 0.0;
    availableMargins available;
    usedMargins used;
};

/// allMargins contains both equity and commodity margins.
struct allMargins {

    allMargins() = default;

    explicit allMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rj::Value eqVal(rj::kObjectType);
        rju::_getIfExists(val, eqVal, "equity", rju::_RJValueType::Object);
        equity.parse(eqVal.GetObject());

        rj::Value cmVal(rj::kObjectType);
        rju::_getIfExists(val, cmVal, "commodity", rju::_RJValueType::Object);
        commodity.parse(cmVal.GetObject());
    };

    margins equity;
    margins commodity;
};

/// order represents a individual order response.
struct order {

    order() = default;

    explicit order(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, accountID, "account_id");
        rju::_getIfExists(val, placedBy, "placed_by");

        rju::_getIfExists(val, orderID, "order_id");
        rju::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rju::_getIfExists(val, parentOrderID, "parent_order_id");
        rju::_getIfExists(val, status, "status");
        rju::_getIfExists(val, statusMessage, "status_message");
        rju::_getIfExists(val, orderTimestamp, "order_timestamp");
        rju::_getIfExists(val, exchangeUpdateTimestamp, "exchange_update_timestamp");
        rju::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rju::_getIfExists(val, rejectedBy, "rejected_by");
        rju::_getIfExists(val, variety, "variety");

        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rju::_getIfExists(val, instrumentToken, "instrument_token");

        rju::_getIfExists(val, orderType, "order_type");
        rju::_getIfExists(val, transactionType, "transaction_type");
        rju::_getIfExists(val, validity, "validity");
        rju::_getIfExists(val, product, "product");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, disclosedQuantity, "disclosed_quantity");
        rju::_getIfExists(val, price, "price");
        rju::_getIfExists(val, triggerPrice, "trigger_price");

        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, filledQuantity, "filled_quantity");
        rju::_getIfExists(val, pendingQuantity, "pending_quantity");
        rju::_getIfExists(val, cancelledQuantity, "cancelled_quantity");
    };

    string accountID;
    string placedBy;

    string orderID;
    string exchangeOrderID;
    string parentOrderID;
    string status;
    string statusMessage;
    string orderTimestamp;
    string exchangeUpdateTimestamp;
    string exchangeTimestamp;
    // xMeta;
    string rejectedBy;
    string variety;

    string exchange;
    string tradingSymbol;
    // xstring instrumentToken;
    int instrumentToken;

    string orderType;
    string transactionType;
    string validity;
    string product;
    int quantity = 0;
    int disclosedQuantity = 0;
    double price = 0.0;
    double triggerPrice = 0.0;

    double averagePrice = 0.0;
    int filledQuantity = 0;
    int pendingQuantity = 0;
    int cancelledQuantity = 0;
};

/// trade represents a individual order response.
struct trade {

    trade() = default;

    explicit trade(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, tradeID, "trade_id");
        rju::_getIfExists(val, product, "product");
        rju::_getIfExists(val, fillTimestamp, "fill_timestamp");
        rju::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rju::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rju::_getIfExists(val, orderID, "order_id");
        rju::_getIfExists(val, transactionType, "transaction_type");
        rju::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, InstrumentToken, "instrument_token");
    };

    double averagePrice;
    double quantity;
    string tradeID;
    string product;
    string fillTimestamp;
    string exchangeTimestamp;
    string exchangeOrderID;
    string orderID;
    string transactionType;
    string tradingSymbol;
    string exchange;
    int InstrumentToken;
};

/// GTTParams is the struct user needs to pass to placeGTT() to place a GTT
struct GTTParams {

    GTTParams() = default;

    GTTParams(string txntype, int quant, string ordtype, string prod, double pr)
        : transactionType(txntype), quantity(quant), orderType(ordtype), product(prod), price(pr) {};

    string transactionType;
    int quantity = 0;
    string orderType;
    string product;
    double price = 0.0;
};

/// GTTCondition represents the condition inside a GTT order.
struct GTTCondition {

    GTTCondition() = default;

    explicit GTTCondition(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rju::_getIfExists(val, lastPrice, "last_price");
        rju::_getIfExists(val, triggerValues, "trigger_values");
    };

    string exchange;
    string tradingsymbol;
    double lastPrice = 0.0;
    std::vector<double> triggerValues;
};

/// GTT represents a single GTT order.
struct GTT {

    GTT() = default;

    explicit GTT(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, ID, "id");
        rju::_getIfExists(val, userID, "user_id");
        rju::_getIfExists(val, type, "type");
        rju::_getIfExists(val, createdAt, "created_at");
        rju::_getIfExists(val, updatedAt, "updated_at");
        rju::_getIfExists(val, expiresAt, "expires_at");
        rju::_getIfExists(val, status, "status");

        rj::Value condnVal(rj::kObjectType);
        rju::_getIfExists(val, condnVal, "condition", rju::_RJValueType::Object);
        condition.parse(condnVal.GetObject());

        auto it = val.FindMember("orders");
        if (it == val.MemberEnd()) { throw libException("Expected value wasn't found (GTT)"); };
        if (!it->value.IsArray()) {
            throw libException("Expected value's type wasn't the one expected. Expected Array");
        };

        for (auto& v : it->value.GetArray()) { orders.emplace_back(v.GetObject()); };
    };

    int ID = 0;
    string userID;
    string type;
    string createdAt;
    string updatedAt;
    string expiresAt;
    string status;
    GTTCondition condition;
    std::vector<order> orders;
}; // namespace kitepp

/// holding is an individual holdings response.
struct holding {

    holding() = default;

    explicit holding(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, instrumentToken, "instrument_token");
        rju::_getIfExists(val, ISIN, "isin");
        rju::_getIfExists(val, product, "product");

        rju::_getIfExists(val, price, "price");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, t1Quantity, "t1_quantity");
        rju::_getIfExists(val, realisedQuantity, "realised_quantity");
        rju::_getIfExists(val, collateralQuantity, "collateral_quantity");
        rju::_getIfExists(val, collateralType, "collateral_type");

        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, lastPrice, "last_price");
        rju::_getIfExists(val, closePrice, "close_price");
        rju::_getIfExists(val, PnL, "pnl");
        rju::_getIfExists(val, dayChange, "day_change");
        rju::_getIfExists(val, dayChangePercentage, "day_change_percentage");
    };

    string tradingsymbol;
    string exchange;
    int instrumentToken = 0;
    string ISIN;
    string product;

    double price = 0.0;
    int quantity = 0;
    int t1Quantity = 0;
    int realisedQuantity = 0;
    int collateralQuantity = 0;
    string collateralType;

    double averagePrice = 0.0;
    double lastPrice = 0.0;
    double closePrice = 0.0;
    double PnL = 0.0;
    double dayChange = 0.0;
    double dayChangePercentage = 0.0;
};

/// position represents an individual position response.
struct position {

    position() = default;

    explicit position(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, instrumentToken, "instrument_token");
        rju::_getIfExists(val, product, "product");

        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, overnightQuantity, "overnight_quantity");
        rju::_getIfExists(val, multiplier, "multiplier");

        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, closePrice, "close_price");
        rju::_getIfExists(val, lastPrice, "last_price");
        rju::_getIfExists(val, value, "value");
        rju::_getIfExists(val, PnL, "pnl");
        rju::_getIfExists(val, M2M, "m2m");
        rju::_getIfExists(val, unrealised, "unrealised");
        rju::_getIfExists(val, realised, "realised");

        rju::_getIfExists(val, buyQuantity, "buy_quantity");
        rju::_getIfExists(val, buyPrice, "buy_price");
        rju::_getIfExists(val, buyValue, "buy_value");
        rju::_getIfExists(val, buyM2MValue, "buy_m2m");

        rju::_getIfExists(val, sellQuantity, "sell_quantity");
        rju::_getIfExists(val, sellPrice, "sell_price");
        rju::_getIfExists(val, sellValue, "sell_value");
        rju::_getIfExists(val, sellM2MValue, "sell_m2m");

        rju::_getIfExists(val, dayBuyQuantity, "day_buy_quantity");
        rju::_getIfExists(val, dayBuyPrice, "day_buy_price");
        rju::_getIfExists(val, dayBuyValue, "day_buy_value");

        rju::_getIfExists(val, daySellQuantity, "day_sell_quantity");
        rju::_getIfExists(val, daySellPrice, "day_sell_price");
        rju::_getIfExists(val, daySellValue, "day_sell_value");
    };

    string tradingsymbol;
    string exchange;
    int instrumentToken = 0;
    string product;

    int quantity = 0;
    int overnightQuantity = 0;
    double multiplier = 0.0;

    double averagePrice = 0.0;
    double closePrice = 0.0;
    double lastPrice = 0.0;
    double value = 0.0;
    double PnL = 0.0;
    double M2M = 0.0;
    double unrealised = 0.0;
    double realised = 0.0;

    int buyQuantity = 0;
    double buyPrice = 0.0;
    double buyValue = 0.0;
    double buyM2MValue = 0.0;

    int sellQuantity = 0;
    double sellPrice = 0.0;
    double sellValue = 0.0;
    double sellM2MValue = 0.0;

    int dayBuyQuantity = 0;
    double dayBuyPrice = 0.0;
    double dayBuyValue = 0.0;

    int daySellQuantity = 0;
    double daySellPrice = 0.0;
    double daySellValue = 0.0;
};

/// positions represents all positions response.
struct positions {

    positions() = default;

    explicit positions(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rj::Value netVal(rj::kArrayType);
        rju::_getIfExists(val, netVal, "net", rju::_RJValueType::Array);
        for (auto& i : netVal.GetArray()) { net.emplace_back(i.GetObject()); };

        rj::Value dayVal(rj::kArrayType);
        rju::_getIfExists(val, dayVal, "day", rju::_RJValueType::Array);
        for (auto& i : dayVal.GetArray()) { day.emplace_back(i.GetObject()); };
    };

    std::vector<position> net;
    std::vector<position> day;
};

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

/// represents historical data call reponse
struct historicalData {

    historicalData() = default;

    explicit historicalData(const rj::Value::Array& val) { parse(val); };

    void parse(const rj::Value::Array& val) {

        /// in case returned number doesn't have decimal point. Directly calling GetDouble() will cause error if number
        /// doesn't have decimal
        static auto getDouble = [](rj::Value& val) -> double {
            if (val.IsDouble()) { return val.GetDouble(); };
            if (val.IsInt()) { return val.GetInt(); }; //! may lead to data loss

            throw libException("type wasn't the one expected (expected double) (historicalData)");
        };

        datetime = val[0].GetString();
        open = getDouble(val[1]);
        high = getDouble(val[2]);
        low = getDouble(val[3]);
        close = getDouble(val[4]);
        volume = val[5].GetInt();
        if (val.Size() > 6) { OI = val[6].GetInt(); };
    };

    string datetime;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    int volume = 0;
    int OI = 0;
};

/// MFOrder represents a individual mutualfund order response.
struct MFOrder {

    MFOrder() = default;

    explicit MFOrder(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, orderID, "order_id");
        rju::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rju::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rju::_getIfExists(val, status, "status");
        rju::_getIfExists(val, statusMessage, "status_message");
        rju::_getIfExists(val, folio, "folio");
        rju::_getIfExists(val, fund, "fund");
        rju::_getIfExists(val, orderTimestamp, "order_timestamp");
        rju::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rju::_getIfExists(val, settlementID, "settlement_id");

        rju::_getIfExists(val, transactionType, "transaction_type");
        rju::_getIfExists(val, variety, "variety");
        rju::_getIfExists(val, purchaseType, "purchase_type");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, amount, "amount");
        rju::_getIfExists(val, lastPrice, "last_price");
        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, placedBy, "placed_by");
        rju::_getIfExists(val, tag, "tag");
    };

    string orderID;
    string exchangeOrderID;
    string tradingsymbol;
    string status;
    string statusMessage;
    string folio;
    string fund;
    string orderTimestamp;
    string exchangeTimestamp;
    string settlementID;

    string transactionType;
    string variety;
    string purchaseType;
    int quantity = 0;
    double amount = 0.0;
    double lastPrice = 0.0;
    double averagePrice = 0.0;
    string placedBy;
    string tag;
};

/// MFHolding represents a individual mutualfund holding.
struct MFHolding {

    MFHolding() = default;

    explicit MFHolding(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, folio, "folio");
        rju::_getIfExists(val, fund, "fund");
        rju::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, lastPrice, "last_price");
        rju::_getIfExists(val, lastPriceDate, "last_price_date");
        rju::_getIfExists(val, Pnl, "pnl");
        rju::_getIfExists(val, quantity, "quantity");
    };

    string folio;
    string fund;
    string tradingsymbol;
    double averagePrice = 0.0;
    double lastPrice = 0.0;
    string lastPriceDate;
    double Pnl = 0.0;
    double quantity = 0;
};

/// MFSIP represents a individual mutualfund SIP response.
struct MFSIP {

    MFSIP() = default;

    explicit MFSIP(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, ID, "sip_id");
        rju::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rju::_getIfExists(val, fundName, "fund");
        rju::_getIfExists(val, dividendType, "dividend_type");
        rju::_getIfExists(val, transactionType, "transaction_type");

        rju::_getIfExists(val, status, "status");
        rju::_getIfExists(val, SIPType, "sip_type");
        rju::_getIfExists(val, created, "created");
        rju::_getIfExists(val, frequency, "frequency");
        rju::_getIfExists(val, instalmentAmount, "instalment_amount");
        rju::_getIfExists(val, instalments, "instalments");
        rju::_getIfExists(val, lastInstalment, "last_instalment");
        rju::_getIfExists(val, pendingInstalments, "pending_instalments");
        rju::_getIfExists(val, instalmentDay, "instalment_day");
        rju::_getIfExists(val, completedInstalments, "completed_instalments");
        rju::_getIfExists(val, nextInstalment, "next_instalment");
        rju::_getIfExists(val, triggerPrice, "trigger_price");
        rju::_getIfExists(val, tag, "tag");
    };

    string ID;
    string tradingsymbol;
    string fundName;
    string dividendType;
    string transactionType;

    string status;
    string SIPType;
    string created;
    string frequency;
    double instalmentAmount = 0.0;
    int instalments = 0;
    string lastInstalment;
    int pendingInstalments = 0;
    int instalmentDay = 0;
    int completedInstalments = 0;
    string nextInstalment;
    double triggerPrice = 0.0;
    string tag;
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
