/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2023 Bhumit Attarde
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

#include <cstdint>
#include <string>

#include "../utils.hpp"
#include "rapidcsv/src/rapidcsv.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/// Parameters required by the `getOrderMargins` method.
struct orderMarginsParams {
    orderMarginsParams() = default;

    GENERATE_FLUENT_METHOD(orderMarginsParams, double, quantity, Quantity);
    GENERATE_FLUENT_METHOD(orderMarginsParams, double, price, Price);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, tradingsymbol, Tradingsymbol);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(
        orderMarginsParams, const string&, orderType, OrderType);

    double quantity = 0;
    double price = 0;
    double triggerPrice = 0;
    string exchange;
    string tradingsymbol;
    string transactionType;
    string variety;
    string product;
    string orderType;
};

/// Represents margins required for placing an order.
struct orderMargins {
    orderMargins() = default;
    explicit orderMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        type = utils::json::get<string>(val, "type");
        tradingSymbol = utils::json::get<string>(val, "tradingsymbol");
        exchange = utils::json::get<string>(val, "exchange");
        span = utils::json::get<double>(val, "span");
        exposure = utils::json::get<double>(val, "exposure");
        optionPremium = utils::json::get<double>(val, "option_premium");
        additional = utils::json::get<double>(val, "additional");
        bo = utils::json::get<double>(val, "bo");
        cash = utils::json::get<double>(val, "cash");
        var = utils::json::get<double>(val, "var");
        pnl = utils::json::get<utils::json::JsonObject, PNL>(val, "pnl");
        total = utils::json::get<double>(val, "total");
    };

    double span = -1;
    double exposure = -1;
    double optionPremium = -1;
    double additional = -1;
    double bo = -1;
    double cash = -1;
    double var = -1;
    double total = -1;
    string type;
    string tradingSymbol;
    string exchange;
    struct PNL {
        PNL() = default;
        explicit PNL(const rj::Value::Object& val) { parse(val); };

        void parse(const rj::Value::Object& val) {
            realised = utils::json::get<double>(val, "realised");
            unrealised = utils::json::get<double>(val, "unrealised");
        };

        double realised = -1;
        double unrealised = -1;
    } pnl;
};
} // namespace kiteconnect