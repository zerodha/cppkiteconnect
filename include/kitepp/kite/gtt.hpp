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
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <string>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
namespace internal {
using std::string;

template <class T>
string getConditionJson(const T& params) {
    utils::json::json<utils::json::JsonObject> conditionJson;
    conditionJson.field("exchange", params.exchange);
    conditionJson.field("tradingsymbol", params.symbol);
    conditionJson.field("trigger_values", params.triggerValues);
    conditionJson.field("last_price", params.lastPrice);
    return conditionJson.serialize();
}

template <class T>
string getOrdersJson(const T& params) {
    utils::json::json<utils::json::JsonArray> ordersJson;
    ordersJson.array<gttParams>(
        params.gttParamsList, [&](const gttParams& param, rj::Value& buffer) {
            ordersJson.field("exchange", params.exchange, &buffer);
            ordersJson.field("tradingsymbol", params.symbol, &buffer);
            ordersJson.field(
                "transaction_type", param.transactionType, &buffer);
            ordersJson.field("quantity", param.quantity, &buffer);
            ordersJson.field("order_type", param.orderType, &buffer);
            ordersJson.field("product", param.product, &buffer);
            ordersJson.field("price", param.price, &buffer);
        });
    return ordersJson.serialize();
}
}; // namespace internal

inline int kite::placeGtt(const placeGttParams& params) {
    utils::http::Params reqParams = {
        { "type", params.triggerType },
        { "condition", internal::getConditionJson(params) },
        { "orders", internal::getOrdersJson(params) },
    };

    return callApi<int, utils::json::JsonObject, true>(
        "gtt.place", reqParams, {}, [](utils::json::JsonObject& data) {
            return utils::json::get<int>(data, "trigger_id");
        });
};

inline std::vector<GTT> kite::triggers() {
    return callApi<std::vector<GTT>, utils::json::JsonArray, true>(
        "gtt", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<GTT> Triggers;
            for (auto& i : data) { Triggers.emplace_back(i.GetObject()); }
            return Triggers;
        });
};

inline GTT kite::getGtt(int triggerId) {
    return callApi<GTT, utils::json::JsonObject>(
        "gtt.info", {}, { std::to_string(triggerId) });
};

inline int kite::modifyGtt(const kc::modifyGttParams& params) {
    utils::http::Params reqParams = {
        { "type", params.triggerType },
        { "condition", internal::getConditionJson(params) },
        { "orders", internal::getOrdersJson(params) },
    };

    return callApi<int, utils::json::JsonObject, true>("gtt.modify", reqParams,
        { std::to_string(params.triggerId) },
        [](utils::json::JsonObject& data) {
            return utils::json::get<int>(data, "trigger_id");
        });
};

inline int kite::deleteGtt(int triggerId) {
    return callApi<int, utils::json::JsonObject, true>("gtt.delete", {},
        { std::to_string(triggerId) }, [](utils::json::JsonObject& data) {
            return utils::json::get<int>(data, "trigger_id");
        });
};

} // namespace kiteconnect