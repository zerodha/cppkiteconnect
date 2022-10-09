#pragma once

#include <optional>
#include <string>

#include "../utils.hpp"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/// represents parameters required for the `placeOrder` function
struct placeOrderParams {
    GENERATE_FLUENT_METHOD(placeOrderParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(
        placeOrderParams, int, disclosedQuantity, DisclosedQuantity);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, price, Price);
    GENERATE_FLUENT_METHOD(
        placeOrderParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, squareOff, SquareOff);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, stopLoss, StopLoss);
    GENERATE_FLUENT_METHOD(
        placeOrderParams, double, trailingStopLoss, TrailingStopLoss);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(
        placeOrderParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(
        placeOrderParams, const string&, orderType, OrderType);
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
    GENERATE_FLUENT_METHOD(
        modifyOrderParams, int, disclosedQuantity, DisclosedQuantity);
    GENERATE_FLUENT_METHOD(modifyOrderParams, double, price, Price);
    GENERATE_FLUENT_METHOD(
        modifyOrderParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(
        modifyOrderParams, const string&, orderType, OrderType);
    GENERATE_FLUENT_METHOD(
        modifyOrderParams, const string&, validity, Validity);
    GENERATE_FLUENT_METHOD(modifyOrderParams, const string&, orderId, OrderId);
    GENERATE_FLUENT_METHOD(
        modifyOrderParams, const string&, parentOrderId, ParentOrderId);

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
        accountID = utils::json::get<string>(val, "account_id");
        placedBy = utils::json::get<string>(val, "placed_by");
        orderID = utils::json::get<string>(val, "order_id");
        exchangeOrderID = utils::json::get<string>(val, "exchange_order_id");
        parentOrderID = utils::json::get<string>(val, "parent_order_id");
        status = utils::json::get<string>(val, "status");
        statusMessage = utils::json::get<string>(val, "status_message");
        orderTimestamp = utils::json::get<string>(val, "order_timestamp");
        exchangeUpdateTimestamp =
            utils::json::get<string>(val, "exchange_update_timestamp");
        exchangeTimestamp = utils::json::get<string>(val, "exchange_timestamp");
        rejectedBy = utils::json::get<string>(val, "rejected_by");
        variety = utils::json::get<string>(val, "variety");
        exchange = utils::json::get<string>(val, "exchange");
        tradingSymbol = utils::json::get<string>(val, "tradingsymbol");
        instrumentToken = utils::json::get<int>(val, "instrument_token");
        orderType = utils::json::get<string>(val, "order_type");
        transactionType = utils::json::get<string>(val, "transaction_type");
        validity = utils::json::get<string>(val, "validity");
        product = utils::json::get<string>(val, "product");
        quantity = utils::json::get<int>(val, "quantity");
        disclosedQuantity = utils::json::get<int>(val, "disclosed_quantity");
        price = utils::json::get<double>(val, "price");
        triggerPrice = utils::json::get<double>(val, "trigger_price");
        averagePrice = utils::json::get<double>(val, "average_price");
        filledQuantity = utils::json::get<int>(val, "filled_quantity");
        pendingQuantity = utils::json::get<int>(val, "pending_quantity");
        cancelledQuantity = utils::json::get<int>(val, "cancelled_quantity");
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
        averagePrice = utils::json::get<double>(val, "average_price");
        quantity = utils::json::get<double>(val, "quantity");
        tradeID = utils::json::get<string>(val, "trade_id");
        product = utils::json::get<string>(val, "product");
        fillTimestamp = utils::json::get<string>(val, "fill_timestamp");
        exchangeTimestamp = utils::json::get<string>(val, "exchange_timestamp");
        exchangeOrderID = utils::json::get<string>(val, "exchange_order_id");
        orderID = utils::json::get<string>(val, "order_id");
        transactionType = utils::json::get<string>(val, "transaction_type");
        tradingSymbol = utils::json::get<string>(val, "tradingsymbol");
        exchange = utils::json::get<string>(val, "exchange");
        instrumentToken = utils::json::get<int>(val, "instrument_token");
    };

    int instrumentToken = -1;
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