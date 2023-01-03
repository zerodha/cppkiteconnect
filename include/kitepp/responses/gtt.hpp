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

#include <string>

#include "../utils.hpp"
#include "order.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/// Represents parameters of a single GTT.
struct gttParams {
    GENERATE_FLUENT_METHOD(gttParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(gttParams, double, price, Price);
    GENERATE_FLUENT_METHOD(
        gttParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(gttParams, const string&, orderType, OrderType);
    GENERATE_FLUENT_METHOD(gttParams, const string&, product, Product);

    int quantity = -1;
    double price = -1;
    string transactionType;
    string orderType;
    string product;
};

/// Represents 'condition' for a single gtt.
struct GTTCondition {
    GTTCondition() = default;
    explicit GTTCondition(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        exchange = utils::json::get<string>(val, "exchange");
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        lastPrice = utils::json::get<double>(val, "last_price");
        triggerValues =
            utils::json::get<std::vector<double>>(val, "trigger_values");
    };

    double lastPrice = -1;
    string exchange;
    string tradingsymbol;
    std::vector<double> triggerValues;
};

/// Parameters required for the `placeGtt` method.
struct placeGttParams {
    GENERATE_FLUENT_METHOD(placeGttParams, double, lastPrice, LastPrice);
    GENERATE_FLUENT_METHOD(
        placeGttParams, const string&, triggerType, TriggerType);
    GENERATE_FLUENT_METHOD(placeGttParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(placeGttParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(placeGttParams, const std::vector<double>&,
        triggerValues, TriggerValues);
    GENERATE_FLUENT_METHOD(placeGttParams, const std::vector<gttParams>&,
        gttParamsList, GttParamsList);

    double lastPrice = -1;
    string triggerType;
    string symbol;
    string exchange;
    std::vector<double> triggerValues;
    std::vector<gttParams> gttParamsList;
};

/// Parameters required for the `modifyGtt` method.
struct modifyGttParams {
    GENERATE_FLUENT_METHOD(modifyGttParams, int, triggerId, TriggerId);
    GENERATE_FLUENT_METHOD(modifyGttParams, double, lastPrice, LastPrice);
    GENERATE_FLUENT_METHOD(
        modifyGttParams, const string&, triggerType, TriggerType);
    GENERATE_FLUENT_METHOD(modifyGttParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(modifyGttParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(modifyGttParams, const std::vector<double>&,
        triggerValues, TriggerValues);
    GENERATE_FLUENT_METHOD(modifyGttParams, const std::vector<gttParams>&,
        gttParamsList, GttParamsList);

    int triggerId;
    double lastPrice;
    string triggerType;
    string symbol;
    string exchange;
    std::vector<double> triggerValues;
    std::vector<gttParams> gttParamsList;
};

/// `GTT` represents a single GTT order.
struct GTT {
    GTT() = default;
    explicit GTT(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        ID = utils::json::get<int>(val, "id");
        userID = utils::json::get<string>(val, "user_id");
        type = utils::json::get<string>(val, "type");
        createdAt = utils::json::get<string>(val, "created_at");
        updatedAt = utils::json::get<string>(val, "updated_at");
        expiresAt = utils::json::get<string>(val, "expires_at");
        status = utils::json::get<string>(val, "status");
        condition = utils::json::get<utils::json::JsonObject, GTTCondition>(
            val, "condition");

        rj::Value ordersBuffer(rj::kArrayType);
        utils::json::get<utils::json::JsonArray>(val, ordersBuffer, "orders");
        for (auto& v : ordersBuffer.GetArray()) {
            orders.emplace_back(v.GetObject());
        };
    };

    int ID = -1;
    string userID;
    string type;
    string createdAt;
    string updatedAt;
    string expiresAt;
    string status;
    GTTCondition condition;
    std::vector<order> orders;
};
} // namespace kiteconnect