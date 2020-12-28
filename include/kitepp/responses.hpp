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

// TODO move these structs inside margins
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

struct ohlc {

    ohlc() = default;

    explicit ohlc(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, open, "open");
        rjh::getIfExists(val, high, "high");
        rjh::getIfExists(val, low, "low");
        rjh::getIfExists(val, close, "close");
    };

    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
};

struct depth {

    depth() = default;

    explicit depth(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, price, "price");
        rjh::getIfExists(val, quantity, "quantity");
        rjh::getIfExists(val, orders, "orders");
    };

    double price = 0.0;
    int quantity = 0;
    int orders = 0;
};

struct quote {

    quote() = default;

    explicit quote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, instrumentToken, "instrument_token");
        rjh::getIfExists(val, timestamp, "timestamp");
        rjh::getIfExists(val, lastPrice, "last_price");
        rjh::getIfExists(val, lastQuantity, "last_quantity");
        rjh::getIfExists(val, lastTradeTime, "last_trade_time");
        rjh::getIfExists(val, averagePrice, "average_price");
        rjh::getIfExists(val, volume, "volume");
        rjh::getIfExists(val, buyQuantity, "buy_quantity");
        rjh::getIfExists(val, sellQuantity, "sell_quantity");

        rj::Value ohlcVal(rj::kObjectType);
        rjh::getIfExists(val, ohlcVal, "ohlc", rjh::RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());

        rjh::getIfExists(val, netChange, "net_change");
        rjh::getIfExists(val, OI, "oi");
        rjh::getIfExists(val, OIDayHigh, "oi_day_high");
        rjh::getIfExists(val, OIDayLow, "oi_day_low");
        rjh::getIfExists(val, lowerCircuitLimit, "lower_circuit_limit");
        rjh::getIfExists(val, upperCircuitLimit, "upper_circuit_limit");

        rj::Value tmpVal(rj::kObjectType);
        rjh::getIfExists(val, tmpVal, "depth", rjh::RJValueType::Object);
        auto depthVal = tmpVal.GetObject();

        rj::Value buyDepthVal(rj::kArrayType);
        rjh::getIfExists(depthVal, buyDepthVal, "buy", rjh::RJValueType::Array);
        for (auto& i : buyDepthVal.GetArray()) { marketDepth.buy.emplace_back(i.GetObject()); };

        rj::Value sellDepthVal(rj::kArrayType);
        rjh::getIfExists(depthVal, sellDepthVal, "sell", rjh::RJValueType::Array);
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

struct OHLCQuote {

    OHLCQuote() = default;

    explicit OHLCQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, instrumentToken, "instrument_token");
        rjh::getIfExists(val, lastPrice, "last_price");

        rj::Value ohlcVal(rj::kObjectType);
        rjh::getIfExists(val, ohlcVal, "ohlc", rjh::RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());
    };

    int instrumentToken = 0;
    double lastPrice = 0.0;
    ohlc OHLC;
};

struct LTPQuote {

    LTPQuote() = default;

    explicit LTPQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, instrumentToken, "instrument_token");
        rjh::getIfExists(val, lastPrice, "last_price");
    };

    int instrumentToken = 0;
    double lastPrice = 0.0;
};

struct historicalData {

    historicalData() = default;

    explicit historicalData(const rj::Value::Array& val) { parse(val); };

    void parse(const rj::Value::Array& val) {

        // in case returned number doesn't have decimal point. Directly calling GetDouble() will cause error if number doesn't have decimal
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

// MFOrder represents a individual mutualfund order response.
struct MFOrder {

    MFOrder() = default;

    explicit MFOrder(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rjh::getIfExists(val, orderID, "order_id");
        rjh::getIfExists(val, exchangeOrderID, "exchange_order_id");
        rjh::getIfExists(val, tradingsymbol, "tradingsymbol");
        rjh::getIfExists(val, status, "status");
        rjh::getIfExists(val, statusMessage, "status_message");
        rjh::getIfExists(val, folio, "folio");
        rjh::getIfExists(val, fund, "fund");
        rjh::getIfExists(val, orderTimestamp, "order_timestamp");
        rjh::getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rjh::getIfExists(val, settlementID, "settlement_id");

        rjh::getIfExists(val, transactionType, "transaction_type");
        rjh::getIfExists(val, variety, "variety");
        rjh::getIfExists(val, purchaseType, "purchase_type");
        rjh::getIfExists(val, quantity, "quantity");
        rjh::getIfExists(val, amount, "amount");
        rjh::getIfExists(val, lastPrice, "last_price");
        rjh::getIfExists(val, averagePrice, "average_price");
        rjh::getIfExists(val, placedBy, "placed_by");
        rjh::getIfExists(val, tag, "tag");
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


} // namespace kitepp
