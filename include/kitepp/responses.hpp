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
        rjh::getIfExists(val, avlVal, "available");
        rj::Value usedVal(rj::kObjectType);
        rjh::getIfExists(val, usedVal, "utilised");

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
        rjh::getIfExists(val, eqVal, "equity");
        rj::Value cmVal(rj::kObjectType);
        rjh::getIfExists(val, cmVal, "commodity");

        equity.parse(eqVal.GetObject());
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

    trade(const rj::Value::Object& val) { parse(val); };

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

} // namespace kitepp
