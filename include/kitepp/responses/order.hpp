#pragma once

#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GENERATE_FLUENT_METHOD(returnType, fieldType, fieldName, methodName)                                           \
    returnType& methodName(fieldType arg) {                                                                            \
        (fieldName) = arg;                                                                                             \
        return *this;                                                                                                  \
    };

struct placeOrderParams {
    GENERATE_FLUENT_METHOD(placeOrderParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(placeOrderParams, int, disclosedQuantity, DisclosedQuantity);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, price, Price);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, squareOff, SquareOff);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, stopLoss, StopLoss);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, trailingStopLoss, TrailingStopLoss);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, orderType, OrderType);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, validity, Validity);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, tag, Tag);

    int quantity = kc::DEFAULTINT;
    int disclosedQuantity = kc::DEFAULTINT;
    double price = kc::DEFAULTDOUBLE;
    double triggerPrice = kc::DEFAULTDOUBLE;
    double squareOff = kc::DEFAULTDOUBLE;
    double stopLoss = kc::DEFAULTDOUBLE;
    double trailingStopLoss = kc::DEFAULTDOUBLE;
    string variety;
    string exchange;
    string symbol;
    string transactionType;
    string product;
    string orderType;
    string validity;
    string tag;
};

/// order represents an individual order response
struct order {
    order() = default;
    explicit order(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, accountID, "account_id");
        kc::rjutils::_getIfExists(val, placedBy, "placed_by");
        kc::rjutils::_getIfExists(val, orderID, "order_id");
        kc::rjutils::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        kc::rjutils::_getIfExists(val, parentOrderID, "parent_order_id");
        kc::rjutils::_getIfExists(val, status, "status");
        kc::rjutils::_getIfExists(val, statusMessage, "status_message");
        kc::rjutils::_getIfExists(val, orderTimestamp, "order_timestamp");
        kc::rjutils::_getIfExists(val, exchangeUpdateTimestamp, "exchange_update_timestamp");
        kc::rjutils::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        kc::rjutils::_getIfExists(val, rejectedBy, "rejected_by");
        kc::rjutils::_getIfExists(val, variety, "variety");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, tradingSymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, orderType, "order_type");
        kc::rjutils::_getIfExists(val, transactionType, "transaction_type");
        kc::rjutils::_getIfExists(val, validity, "validity");
        kc::rjutils::_getIfExists(val, product, "product");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, disclosedQuantity, "disclosed_quantity");
        kc::rjutils::_getIfExists(val, price, "price");
        kc::rjutils::_getIfExists(val, triggerPrice, "trigger_price");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, filledQuantity, "filled_quantity");
        kc::rjutils::_getIfExists(val, pendingQuantity, "pending_quantity");
        kc::rjutils::_getIfExists(val, cancelledQuantity, "cancelled_quantity");
    };

    int instrumentToken = kc::DEFAULTINT;
    int quantity = kc::DEFAULTINT;
    int disclosedQuantity = kc::DEFAULTINT;
    int filledQuantity = kc::DEFAULTINT;
    int pendingQuantity = kc::DEFAULTINT;
    int cancelledQuantity = kc::DEFAULTINT;
    double price = kc::DEFAULTDOUBLE;
    double triggerPrice = kc::DEFAULTDOUBLE;
    double averagePrice = kc::DEFAULTDOUBLE;
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
    string rejectedBy;
    string variety;
    string exchange;
    string tradingSymbol;
    string orderType;
    string transactionType;
    string validity;
    string product;
};

/// trade represents an individual trade response
struct trade {
    trade() = default;
    explicit trade(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, tradeID, "trade_id");
        kc::rjutils::_getIfExists(val, product, "product");
        kc::rjutils::_getIfExists(val, fillTimestamp, "fill_timestamp");
        kc::rjutils::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        kc::rjutils::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        kc::rjutils::_getIfExists(val, orderID, "order_id");
        kc::rjutils::_getIfExists(val, transactionType, "transaction_type");
        kc::rjutils::_getIfExists(val, tradingSymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, InstrumentToken, "instrument_token");
    };

    int InstrumentToken = kc::DEFAULTINT;
    double averagePrice = kc::DEFAULTDOUBLE;
    double quantity = kc::DEFAULTDOUBLE;
    string tradeID;
    string product;
    string fillTimestamp;
    string exchangeTimestamp;
    string exchangeOrderID;
    string orderID;
    string transactionType;
    string tradingSymbol;
    string exchange;
};

#undef GENERATE_FLUENT_METHOD

} // namespace kiteconnect