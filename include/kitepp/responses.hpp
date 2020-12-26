/**
 * @file responses.hpp
 * @author Bhumit Atatrde
 * @brief This file has all structs returned by kitepp
 *
 */

#pragma once

#include <iostream> //debugging
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rjhelper.hpp"

namespace kitepp {

using std::string;
namespace rj = rapidjson;
namespace rjh = kitepp::RJHelper;


// UserProfile represents a user's personal and financial profile.
struct userProfile {

    userProfile() = default;

    explicit userProfile(const rj::Value::Object& val) { parse(val); };

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

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, userName, "user_name");
        rjh::getIfExists(val, userShortName, "user_shortname");
        rjh::getIfExists(val, avatarURL, "avatar_url");
        rjh::getIfExists(val, userType, "user_type");
        rjh::getIfExists(val, email, "email");
        // x rjh::getIfExists(val, phone, "phone");
        rjh::getIfExists(val, broker, "broker");
        rjh::getIfExists(val, products, "products");
        rjh::getIfExists(val, orderTypes, "order_types");
        rjh::getIfExists(val, exchanges, "exchanges");
    };
};

struct userTokens {

    string userID;
    string accessToken;
    string refreshToken;

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, userID, "user_id");
        rjh::getIfExists(val, accessToken, "access_token");
        rjh::getIfExists(val, refreshToken, "refresh_token");
    };
};

struct userSession {

    userProfile profile;
    userTokens tokens;

    string apiKey;
    string publicToken;
    string loginTime;

    void parse(const rj::Value::Object& val) {

        profile.parse(val);
        tokens.parse(val);
        rjh::getIfExists(val, apiKey, "api_key");
        rjh::getIfExists(val, publicToken, "public_token");
        rjh::getIfExists(val, loginTime, "login_time");
    };
};

// AvailableMargins represents the available margins from the margins response for a single segment.
struct availableMargins {

    availableMargins() = default;

    explicit availableMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        double debugval = val["cash"].GetDouble();

        rjh::getIfExists(val, adHocMargin, "adhoc_margin");
        rjh::getIfExists(val, cash, "cash");
        rjh::getIfExists(val, collateral, "collateral");
        rjh::getIfExists(val, intradayPayin, "intraday_payin");
    };

    double adHocMargin = 0.0;
    double cash = 0.0;
    double collateral = 0.0;
    double intradayPayin = 0.0;
};

// UsedMargins represents the used margins from the margins response for a single segment.
struct usedMargins {

    usedMargins() = default;

    explicit usedMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        rjh::getIfExists(val, debits, "debits");
        rjh::getIfExists(val, exposure, "exposure");
        rjh::getIfExists(val, M2MRealised, "m2m_realised");
        rjh::getIfExists(val, M2MUnrealised, "m2m_unrealised");
        rjh::getIfExists(val, optionPremium, "option_premium");
        rjh::getIfExists(val, payout, "payout");
        rjh::getIfExists(val, span, "span");
        rjh::getIfExists(val, holdingSales, "holding_sales");
        rjh::getIfExists(val, turnover, "turnover");
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

// Margins represents the user margins for a segment.
struct margins {

    margins() = default;

    explicit margins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, enabled, "enabled");
        rjh::getIfExists(val, net, "net");

        rj::Value avlVal(rj::kObjectType);
        rjh::getIfExists(val, avlVal, "available", rjh::RJValueType::Object);
        rj::Value usedVal(rj::kObjectType);
        rjh::getIfExists(val, usedVal, "utilised", rjh::RJValueType::Object);

        available.parse(avlVal.GetObject());
        used.parse(usedVal.GetObject());
    };

    // string category;
    bool enabled = false;
    double net = 0.0;
    availableMargins available;
    usedMargins used;
};

// AllMargins contains both equity and commodity margins.
struct allMargins {

    allMargins() = default;

    explicit allMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rj::Value eqVal(rj::kObjectType);
        rjh::getIfExists(val, eqVal, "equity", rjh::RJValueType::Object);
        equity.parse(eqVal.GetObject());

        rj::Value cmVal(rj::kObjectType);
        rjh::getIfExists(val, cmVal, "commodity", rjh::RJValueType::Object);
        commodity.parse(cmVal.GetObject());
    };

    margins equity;
    margins commodity;
};


// Order represents a individual order response.
struct order {

    order() = default;

    explicit order(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, accountID, "account_id");
        rjh::getIfExists(val, placedBy, "placed_by");

        rjh::getIfExists(val, orderID, "order_id");
        rjh::getIfExists(val, exchangeOrderID, "exchange_order_id");
        rjh::getIfExists(val, parentOrderID, "parent_order_id");
        rjh::getIfExists(val, status, "status");
        rjh::getIfExists(val, statusMessage, "status_message");
        rjh::getIfExists(val, orderTimestamp, "order_timestamp");
        rjh::getIfExists(val, exchangeUpdateTimestamp, "exchange_update_timestamp");
        rjh::getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rjh::getIfExists(val, rejectedBy, "rejected_by");
        rjh::getIfExists(val, variety, "variety");

        rjh::getIfExists(val, exchange, "exchange");
        rjh::getIfExists(val, tradingSymbol, "tradingsymbol");
        rjh::getIfExists(val, instrumentToken, "instrument_token");

        rjh::getIfExists(val, orderType, "order_type");
        rjh::getIfExists(val, transactionType, "transaction_type");
        rjh::getIfExists(val, validity, "validity");
        rjh::getIfExists(val, product, "product");
        rjh::getIfExists(val, quantity, "quantity");
        rjh::getIfExists(val, disclosedQuantity, "disclosed_quantity");
        rjh::getIfExists(val, price, "price");
        rjh::getIfExists(val, triggerPrice, "trigger_price");

        rjh::getIfExists(val, averagePrice, "average_price");
        rjh::getIfExists(val, filledQuantity, "filled_quantity");
        rjh::getIfExists(val, pendingQuantity, "pending_quantity");
        rjh::getIfExists(val, cancelledQuantity, "cancelled_quantity");
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

struct trade {

    trade() = default;

    explicit trade(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, averagePrice, "average_price");
        rjh::getIfExists(val, quantity, "quantity");
        rjh::getIfExists(val, tradeID, "trade_id");
        rjh::getIfExists(val, product, "product");
        rjh::getIfExists(val, fillTimestamp, "fill_timestamp");
        rjh::getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rjh::getIfExists(val, exchangeOrderID, "exchange_order_id");
        rjh::getIfExists(val, orderID, "order_id");
        rjh::getIfExists(val, transactionType, "transaction_type");
        rjh::getIfExists(val, tradingSymbol, "tradingsymbol");
        rjh::getIfExists(val, exchange, "exchange");
        rjh::getIfExists(val, InstrumentToken, "instrument_token");
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

// GTTParams is the struct user needs to pass to placeGTT() to place a GTT
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

// GTTCondition represents the condition inside a GTT order.
struct GTTCondition {

    GTTCondition() = default;

    explicit GTTCondition(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, exchange, "exchange");
        rjh::getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::getIfExists(val, lastPrice, "last_price");
        rjh::getIfExists(val, triggerValues, "trigger_values");
    };

    string exchange;
    string tradingsymbol;
    double lastPrice = 0.0;
    std::vector<double> triggerValues;
};

// GTT represents a single GTT order.
struct GTT {

    GTT() = default;

    explicit GTT(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, ID, "id");
        rjh::getIfExists(val, userID, "user_id");
        rjh::getIfExists(val, type, "type");
        rjh::getIfExists(val, createdAt, "created_at");
        rjh::getIfExists(val, updatedAt, "updated_at");
        rjh::getIfExists(val, expiresAt, "expires_at");
        rjh::getIfExists(val, status, "status");

        rj::Value condnVal(rj::kObjectType);
        rjh::getIfExists(val, condnVal, "condition", rjh::RJValueType::Object);
        condition.parse(condnVal.GetObject());

        auto it = val.FindMember("orders");
        if (it == val.MemberEnd()) { throw libException("Expected value wasn't found (GTT)"); };
        if (!it->value.IsArray()) { throw libException("Expected value's type wasn't the one expected. Expected Array"); };

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

// Holding is an individual holdings response.
struct holding {

    holding() = default;

    explicit holding(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::getIfExists(val, exchange, "exchange");
        rjh::getIfExists(val, instrumentToken, "instrument_token");
        rjh::getIfExists(val, ISIN, "isin");
        rjh::getIfExists(val, product, "product");

        rjh::getIfExists(val, price, "price");
        rjh::getIfExists(val, quantity, "quantity");
        rjh::getIfExists(val, t1Quantity, "t1_quantity");
        rjh::getIfExists(val, realisedQuantity, "realised_quantity");
        rjh::getIfExists(val, collateralQuantity, "collateral_quantity");
        rjh::getIfExists(val, collateralType, "collateral_type");

        rjh::getIfExists(val, averagePrice, "average_price");
        rjh::getIfExists(val, lastPrice, "last_price");
        rjh::getIfExists(val, closePrice, "close_price");
        rjh::getIfExists(val, PnL, "pnl");
        rjh::getIfExists(val, dayChange, "day_change");
        rjh::getIfExists(val, dayChangePercentage, "day_change_percentage");
    };


    string tradingsymbol;
    string exchange;
    int instrumentToken;
    string ISIN;
    string product;

    double price;
    int quantity;
    int t1Quantity;
    int realisedQuantity;
    int collateralQuantity;
    string collateralType;

    double averagePrice;
    double lastPrice;
    double closePrice;
    double PnL;
    double dayChange;
    double dayChangePercentage;
};


// Position represents an individual position response.
struct position {


    position() = default;

    explicit position(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::getIfExists(val, exchange, "exchange");
        rjh::getIfExists(val, instrumentToken, "instrument_token");
        rjh::getIfExists(val, product, "product");

        rjh::getIfExists(val, quantity, "quantity");
        rjh::getIfExists(val, overnightQuantity, "overnight_quantity");
        rjh::getIfExists(val, multiplier, "multiplier");

        rjh::getIfExists(val, averagePrice, "average_price");
        rjh::getIfExists(val, closePrice, "close_price");
        rjh::getIfExists(val, lastPrice, "last_price");
        rjh::getIfExists(val, value, "value");
        rjh::getIfExists(val, PnL, "pnl");
        rjh::getIfExists(val, M2M, "m2m");
        rjh::getIfExists(val, unrealised, "unrealised");
        rjh::getIfExists(val, realised, "realised");

        rjh::getIfExists(val, buyQuantity, "buy_quantity");
        rjh::getIfExists(val, buyPrice, "buy_price");
        rjh::getIfExists(val, buyValue, "buy_value");
        rjh::getIfExists(val, buyM2MValue, "buy_m2m");

        rjh::getIfExists(val, sellQuantity, "sell_quantity");
        rjh::getIfExists(val, sellPrice, "sell_price");
        rjh::getIfExists(val, sellValue, "sell_value");
        rjh::getIfExists(val, sellM2MValue, "sell_m2m");

        rjh::getIfExists(val, dayBuyQuantity, "day_buy_quantity");
        rjh::getIfExists(val, dayBuyPrice, "day_buy_price");
        rjh::getIfExists(val, dayBuyValue, "day_buy_value");

        rjh::getIfExists(val, daySellQuantity, "day_sell_quantity");
        rjh::getIfExists(val, daySellPrice, "day_sell_price");
        rjh::getIfExists(val, daySellValue, "averaday_sell_valuege_price");
    };


    string tradingsymbol;
    string exchange;
    int instrumentToken;
    string product;

    int quantity;
    int overnightQuantity;
    double multiplier;

    double averagePrice;
    double closePrice;
    double lastPrice;
    double value;
    double PnL;
    double M2M;
    double unrealised;
    double realised;

    int buyQuantity;
    double buyPrice;
    double buyValue;
    double buyM2MValue;

    int sellQuantity;
    double sellPrice;
    double sellValue;
    double sellM2MValue;

    int dayBuyQuantity;
    double dayBuyPrice;
    double dayBuyValue;

    int daySellQuantity;
    double daySellPrice;
    double daySellValue;
};

struct positions {

    positions() = default;

    explicit positions(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rj::Value netVal(rj::kArrayType);
        rjh::getIfExists(val, netVal, "net", rjh::RJValueType::Array);
        for (auto& i : netVal.GetArray()) { net.emplace_back(i.GetObject()); };

        rj::Value dayVal(rj::kArrayType);
        rjh::getIfExists(val, dayVal, "day", rjh::RJValueType::Array);
        for (auto& i : netVal.GetArray()) { day.emplace_back(i.GetObject()); };
    };


    std::vector<position> net;
    std::vector<position> day;
};

} // namespace kitepp
