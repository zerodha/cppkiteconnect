/*
 *   Copyright (c) 2020 Bhumit Attarde

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file responses.hpp
 * @brief This file has all the structs returned by kitepp
 */

#pragma once

#include <iostream> //debugging
#include <string>
#include <vector>

#include "helperfunctions.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rjhelper.hpp"

namespace kitepp {

using std::string;
namespace rj = rapidjson;
namespace rjh = kitepp::RJHelper;

/// userProfile represents a user's personal and financial profile.
struct userProfile {

    userProfile() = default;

    explicit userProfile(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::_getIfExists(val, userName, "user_name");
        rjh::_getIfExists(val, userShortName, "user_shortname");
        rjh::_getIfExists(val, avatarURL, "avatar_url");
        rjh::_getIfExists(val, userType, "user_type");
        rjh::_getIfExists(val, email, "email");
        // x rjh::_getIfExists(val, phone, "phone");
        rjh::_getIfExists(val, broker, "broker");
        rjh::_getIfExists(val, products, "products");
        rjh::_getIfExists(val, orderTypes, "order_types");
        rjh::_getIfExists(val, exchanges, "exchanges");
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

        rjh::_getIfExists(val, userID, "user_id");
        rjh::_getIfExists(val, accessToken, "access_token");
        rjh::_getIfExists(val, refreshToken, "refresh_token");
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
        rjh::_getIfExists(val, apiKey, "api_key");
        rjh::_getIfExists(val, publicToken, "public_token");
        rjh::_getIfExists(val, loginTime, "login_time");
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

        rjh::_getIfExists(val, adHocMargin, "adhoc_margin");
        rjh::_getIfExists(val, cash, "cash");
        rjh::_getIfExists(val, collateral, "collateral");
        rjh::_getIfExists(val, intradayPayin, "intraday_payin");
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
        rjh::_getIfExists(val, debits, "debits");
        rjh::_getIfExists(val, exposure, "exposure");
        rjh::_getIfExists(val, M2MRealised, "m2m_realised");
        rjh::_getIfExists(val, M2MUnrealised, "m2m_unrealised");
        rjh::_getIfExists(val, optionPremium, "option_premium");
        rjh::_getIfExists(val, payout, "payout");
        rjh::_getIfExists(val, span, "span");
        rjh::_getIfExists(val, holdingSales, "holding_sales");
        rjh::_getIfExists(val, turnover, "turnover");
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

        rjh::_getIfExists(val, enabled, "enabled");
        rjh::_getIfExists(val, net, "net");

        rj::Value avlVal(rj::kObjectType);
        rjh::_getIfExists(val, avlVal, "available", rjh::_RJValueType::Object);
        rj::Value usedVal(rj::kObjectType);
        rjh::_getIfExists(val, usedVal, "utilised", rjh::_RJValueType::Object);

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
        rjh::_getIfExists(val, eqVal, "equity", rjh::_RJValueType::Object);
        equity.parse(eqVal.GetObject());

        rj::Value cmVal(rj::kObjectType);
        rjh::_getIfExists(val, cmVal, "commodity", rjh::_RJValueType::Object);
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

        rjh::_getIfExists(val, accountID, "account_id");
        rjh::_getIfExists(val, placedBy, "placed_by");

        rjh::_getIfExists(val, orderID, "order_id");
        rjh::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rjh::_getIfExists(val, parentOrderID, "parent_order_id");
        rjh::_getIfExists(val, status, "status");
        rjh::_getIfExists(val, statusMessage, "status_message");
        rjh::_getIfExists(val, orderTimestamp, "order_timestamp");
        rjh::_getIfExists(val, exchangeUpdateTimestamp, "exchange_update_timestamp");
        rjh::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rjh::_getIfExists(val, rejectedBy, "rejected_by");
        rjh::_getIfExists(val, variety, "variety");

        rjh::_getIfExists(val, exchange, "exchange");
        rjh::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rjh::_getIfExists(val, instrumentToken, "instrument_token");

        rjh::_getIfExists(val, orderType, "order_type");
        rjh::_getIfExists(val, transactionType, "transaction_type");
        rjh::_getIfExists(val, validity, "validity");
        rjh::_getIfExists(val, product, "product");
        rjh::_getIfExists(val, quantity, "quantity");
        rjh::_getIfExists(val, disclosedQuantity, "disclosed_quantity");
        rjh::_getIfExists(val, price, "price");
        rjh::_getIfExists(val, triggerPrice, "trigger_price");

        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, filledQuantity, "filled_quantity");
        rjh::_getIfExists(val, pendingQuantity, "pending_quantity");
        rjh::_getIfExists(val, cancelledQuantity, "cancelled_quantity");
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

        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, quantity, "quantity");
        rjh::_getIfExists(val, tradeID, "trade_id");
        rjh::_getIfExists(val, product, "product");
        rjh::_getIfExists(val, fillTimestamp, "fill_timestamp");
        rjh::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rjh::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rjh::_getIfExists(val, orderID, "order_id");
        rjh::_getIfExists(val, transactionType, "transaction_type");
        rjh::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rjh::_getIfExists(val, exchange, "exchange");
        rjh::_getIfExists(val, InstrumentToken, "instrument_token");
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

        rjh::_getIfExists(val, exchange, "exchange");
        rjh::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::_getIfExists(val, lastPrice, "last_price");
        rjh::_getIfExists(val, triggerValues, "trigger_values");
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

        rjh::_getIfExists(val, ID, "id");
        rjh::_getIfExists(val, userID, "user_id");
        rjh::_getIfExists(val, type, "type");
        rjh::_getIfExists(val, createdAt, "created_at");
        rjh::_getIfExists(val, updatedAt, "updated_at");
        rjh::_getIfExists(val, expiresAt, "expires_at");
        rjh::_getIfExists(val, status, "status");

        rj::Value condnVal(rj::kObjectType);
        rjh::_getIfExists(val, condnVal, "condition", rjh::_RJValueType::Object);
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

        rjh::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::_getIfExists(val, exchange, "exchange");
        rjh::_getIfExists(val, instrumentToken, "instrument_token");
        rjh::_getIfExists(val, ISIN, "isin");
        rjh::_getIfExists(val, product, "product");

        rjh::_getIfExists(val, price, "price");
        rjh::_getIfExists(val, quantity, "quantity");
        rjh::_getIfExists(val, t1Quantity, "t1_quantity");
        rjh::_getIfExists(val, realisedQuantity, "realised_quantity");
        rjh::_getIfExists(val, collateralQuantity, "collateral_quantity");
        rjh::_getIfExists(val, collateralType, "collateral_type");

        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, lastPrice, "last_price");
        rjh::_getIfExists(val, closePrice, "close_price");
        rjh::_getIfExists(val, PnL, "pnl");
        rjh::_getIfExists(val, dayChange, "day_change");
        rjh::_getIfExists(val, dayChangePercentage, "day_change_percentage");
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

        rjh::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::_getIfExists(val, exchange, "exchange");
        rjh::_getIfExists(val, instrumentToken, "instrument_token");
        rjh::_getIfExists(val, product, "product");

        rjh::_getIfExists(val, quantity, "quantity");
        rjh::_getIfExists(val, overnightQuantity, "overnight_quantity");
        rjh::_getIfExists(val, multiplier, "multiplier");

        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, closePrice, "close_price");
        rjh::_getIfExists(val, lastPrice, "last_price");
        rjh::_getIfExists(val, value, "value");
        rjh::_getIfExists(val, PnL, "pnl");
        rjh::_getIfExists(val, M2M, "m2m");
        rjh::_getIfExists(val, unrealised, "unrealised");
        rjh::_getIfExists(val, realised, "realised");

        rjh::_getIfExists(val, buyQuantity, "buy_quantity");
        rjh::_getIfExists(val, buyPrice, "buy_price");
        rjh::_getIfExists(val, buyValue, "buy_value");
        rjh::_getIfExists(val, buyM2MValue, "buy_m2m");

        rjh::_getIfExists(val, sellQuantity, "sell_quantity");
        rjh::_getIfExists(val, sellPrice, "sell_price");
        rjh::_getIfExists(val, sellValue, "sell_value");
        rjh::_getIfExists(val, sellM2MValue, "sell_m2m");

        rjh::_getIfExists(val, dayBuyQuantity, "day_buy_quantity");
        rjh::_getIfExists(val, dayBuyPrice, "day_buy_price");
        rjh::_getIfExists(val, dayBuyValue, "day_buy_value");

        rjh::_getIfExists(val, daySellQuantity, "day_sell_quantity");
        rjh::_getIfExists(val, daySellPrice, "day_sell_price");
        rjh::_getIfExists(val, daySellValue, "day_sell_value");
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
        rjh::_getIfExists(val, netVal, "net", rjh::_RJValueType::Array);
        for (auto& i : netVal.GetArray()) { net.emplace_back(i.GetObject()); };

        rj::Value dayVal(rj::kArrayType);
        rjh::_getIfExists(val, dayVal, "day", rjh::_RJValueType::Array);
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

        rjh::_getIfExists(val, open, "open");
        rjh::_getIfExists(val, high, "high");
        rjh::_getIfExists(val, low, "low");
        rjh::_getIfExists(val, close, "close");
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

        rjh::_getIfExists(val, price, "price");
        rjh::_getIfExists(val, quantity, "quantity");
        rjh::_getIfExists(val, orders, "orders");
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

        rjh::_getIfExists(val, instrumentToken, "instrument_token");
        rjh::_getIfExists(val, timestamp, "timestamp");
        rjh::_getIfExists(val, lastPrice, "last_price");
        rjh::_getIfExists(val, lastQuantity, "last_quantity");
        rjh::_getIfExists(val, lastTradeTime, "last_trade_time");
        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, volume, "volume");
        rjh::_getIfExists(val, buyQuantity, "buy_quantity");
        rjh::_getIfExists(val, sellQuantity, "sell_quantity");

        rj::Value ohlcVal(rj::kObjectType);
        rjh::_getIfExists(val, ohlcVal, "ohlc", rjh::_RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());

        rjh::_getIfExists(val, netChange, "net_change");
        rjh::_getIfExists(val, OI, "oi");
        rjh::_getIfExists(val, OIDayHigh, "oi_day_high");
        rjh::_getIfExists(val, OIDayLow, "oi_day_low");
        rjh::_getIfExists(val, lowerCircuitLimit, "lower_circuit_limit");
        rjh::_getIfExists(val, upperCircuitLimit, "upper_circuit_limit");

        rj::Value tmpVal(rj::kObjectType);
        rjh::_getIfExists(val, tmpVal, "depth", rjh::_RJValueType::Object);
        auto depthVal = tmpVal.GetObject();

        rj::Value buyDepthVal(rj::kArrayType);
        rjh::_getIfExists(depthVal, buyDepthVal, "buy", rjh::_RJValueType::Array);
        for (auto& i : buyDepthVal.GetArray()) { marketDepth.buy.emplace_back(i.GetObject()); };

        rj::Value sellDepthVal(rj::kArrayType);
        rjh::_getIfExists(depthVal, sellDepthVal, "sell", rjh::_RJValueType::Array);
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

        rjh::_getIfExists(val, instrumentToken, "instrument_token");
        rjh::_getIfExists(val, lastPrice, "last_price");

        rj::Value ohlcVal(rj::kObjectType);
        rjh::_getIfExists(val, ohlcVal, "ohlc", rjh::_RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());
    };

    int instrumentToken = 0;
    double lastPrice = 0.0;
    ohlc OHLC;
};

// represents ltp quote respone
struct LTPQuote {

    LTPQuote() = default;

    explicit LTPQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::_getIfExists(val, instrumentToken, "instrument_token");
        rjh::_getIfExists(val, lastPrice, "last_price");
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

        rjh::_getIfExists(val, orderID, "order_id");
        rjh::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rjh::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::_getIfExists(val, status, "status");
        rjh::_getIfExists(val, statusMessage, "status_message");
        rjh::_getIfExists(val, folio, "folio");
        rjh::_getIfExists(val, fund, "fund");
        rjh::_getIfExists(val, orderTimestamp, "order_timestamp");
        rjh::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rjh::_getIfExists(val, settlementID, "settlement_id");

        rjh::_getIfExists(val, transactionType, "transaction_type");
        rjh::_getIfExists(val, variety, "variety");
        rjh::_getIfExists(val, purchaseType, "purchase_type");
        rjh::_getIfExists(val, quantity, "quantity");
        rjh::_getIfExists(val, amount, "amount");
        rjh::_getIfExists(val, lastPrice, "last_price");
        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, placedBy, "placed_by");
        rjh::_getIfExists(val, tag, "tag");
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

        rjh::_getIfExists(val, folio, "folio");
        rjh::_getIfExists(val, fund, "fund");
        rjh::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::_getIfExists(val, averagePrice, "average_price");
        rjh::_getIfExists(val, lastPrice, "last_price");
        rjh::_getIfExists(val, lastPriceDate, "last_price_date");
        rjh::_getIfExists(val, Pnl, "pnl");
        rjh::_getIfExists(val, quantity, "quantity");
    };

    string folio;
    string fund;
    string tradingsymbol;
    double averagePrice = 0.0;
    double lastPrice = 0.0;
    string lastPriceDate;
    double Pnl = 0.0;
    int quantity = 0;
};

/// MFSIP represents a individual mutualfund SIP response.
struct MFSIP {

    MFSIP() = default;

    explicit MFSIP(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::_getIfExists(val, ID, "sip_id");
        rjh::_getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::_getIfExists(val, fundName, "fund");
        rjh::_getIfExists(val, dividendType, "dividend_type");
        rjh::_getIfExists(val, transactionType, "transaction_type");

        rjh::_getIfExists(val, status, "status");
        rjh::_getIfExists(val, SIPType, "sip_type");
        rjh::_getIfExists(val, created, "created");
        rjh::_getIfExists(val, frequency, "frequency");
        rjh::_getIfExists(val, instalmentAmount, "instalment_amount");
        rjh::_getIfExists(val, instalments, "instalments");
        rjh::_getIfExists(val, lastInstalment, "last_instalment");
        rjh::_getIfExists(val, pendingInstalments, "pending_instalments");
        rjh::_getIfExists(val, instalmentDay, "instalment_day");
        rjh::_getIfExists(val, completedInstalments, "completed_instalments");
        rjh::_getIfExists(val, nextInstalment, "next_instalment");
        rjh::_getIfExists(val, triggerPrice, "trigger_price");
        rjh::_getIfExists(val, tag, "tag");
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

        rjh::_getIfExists(val, type, "type");
        rjh::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rjh::_getIfExists(val, exchange, "exchange");
        rjh::_getIfExists(val, SPAN, "span");
        rjh::_getIfExists(val, exposure, "exposure");
        rjh::_getIfExists(val, optionPremium, "option_premium");
        rjh::_getIfExists(val, additional, "additional");
        rjh::_getIfExists(val, BO, "bo");
        rjh::_getIfExists(val, cash, "cash");
        rjh::_getIfExists(val, VAR, "var");

        rj::Value pnlVal(rj::kObjectType);
        rjh::_getIfExists(val, pnlVal, "pnl", rjh::_RJValueType::Object);
        pnl.parse(pnlVal.GetObject());

        rjh::_getIfExists(val, total, "total");
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

            rjh::_getIfExists(val, realised, "realised");
            rjh::_getIfExists(val, unrealised, "unrealised");
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

        std::vector<string> tokens = kitepp::_split(val, ',');

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

        std::vector<string> tokens = kitepp::_split(val, ',');

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

/// Reoresents a single entry in market depth returned by kWS
struct depthWS {

    double price;
    int32_t quantity;
    int32_t orders;
};

/// Represents a single tick returned by kWS
struct tick {

    string mode;
    int32_t InstrumentToken;
    bool isTradable;
    bool isIndex;

    int32_t timestamp;
    int32_t lastTradeTime;
    double lastPrice;
    int32_t lastTradedQuantity;
    int32_t totalBuyQuantity;
    int32_t totalSellQuantity;
    int32_t volumeTraded;
    int32_t totalBuy;
    int32_t totalSell;
    double averageTradePrice;
    int32_t OI;
    int32_t OIDayHigh;
    int32_t OIDayLow;
    double netChange;

    // OHLC  OHLC
    struct ohlc {
        // xint32_t InstrumentToken;
        double open;
        double high;
        double low;
        double close;
    } OHLC;

    // Depth Depth
    struct m_depth {
        std::vector<depthWS> buy;
        std::vector<depthWS> sell;
    } marketDepth;
};

} // namespace kitepp
