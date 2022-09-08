#pragma once

#include <optional>
#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "../utils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

/// represents parameters required for the `placeOrder` function
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

    int quantity;
    std::optional<int> disclosedQuantity;
    std::optional<int> price;
    std::optional<double> triggerPrice;
    std::optional<double> squareOff;
    std::optional<double> stopLoss;
    std::optional<double> trailingStopLoss;
    string variety;
    string exchange;
    string symbol;
    string transactionType;
    string product;
    string orderType;
    std::optional<string> validity;
    std::optional<string> tag;
};

/// represents parameters required for the `modifyOrder` function
struct modifyOrderParams {
    GENERATE_FLUENT_METHOD(modifyOrderParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(modifyOrderParams, int, disclosedQuantity, DisclosedQuantity);
    GENERATE_FLUENT_METHOD(modifyOrderParams, double, price, Price);
    GENERATE_FLUENT_METHOD(modifyOrderParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, orderType, OrderType);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, validity, Validity);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, orderId, OrderId);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, parentOrderId, ParentOrderId);

    std::optional<int> quantity;
    std::optional<int> disclosedQuantity;
    std::optional<double> price;
    std::optional<double> triggerPrice;
    string variety;
    string orderId;
    std::optional<string> parentOrderId;
    std::optional<string> orderType;
    std::optional<string> validity;
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

    int instrumentToken = -1;
    int quantity = -1;
    int disclosedQuantity = -1;
    int filledQuantity = -1;
    int pendingQuantity = -1;
    int cancelledQuantity = -1;
    double price = -1.0;
    double triggerPrice = -1.0;
    double averagePrice = -1.0;
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

    int InstrumentToken = -1;
    double averagePrice = -1.0;
    double quantity = -1.0;
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

} // namespace kiteconnect