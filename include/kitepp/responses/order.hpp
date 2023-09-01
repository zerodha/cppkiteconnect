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

/// Parameters required for the `placeOrder` method.
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
    std::optional<double> price;
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

/// Parameters required for the `modifyOrder` method.
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

/// Represents information of an order.
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
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        instrumentToken = utils::json::get<uint32_t>(val, "instrument_token");
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

    uint32_t instrumentToken = 0;
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
    string tradingsymbol;
    string orderType;
    string transactionType;
    string validity;
    string product;
};

/// Represents information of a trade.
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
        instrumentToken = utils::json::get<uint32_t>(val, "instrument_token");
    };

    uint32_t instrumentToken = 0;
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