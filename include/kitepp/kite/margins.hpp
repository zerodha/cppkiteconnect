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
#include "kitepp/responses/margins.hpp"
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <vector>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline std::vector<orderMargins> kite::getOrderMargins(
    const std::vector<marginsParams>& params) {
    utils::json::json<utils::json::JsonArray> ordersJson;
    ordersJson.array<marginsParams>(params, [&](const marginsParams& param,
                                                rj::Value& buffer) {
        ordersJson.field("exchange", param.exchange, &buffer);
        ordersJson.field("tradingsymbol", param.tradingsymbol, &buffer);
        ordersJson.field("transaction_type", param.transactionType, &buffer);
        ordersJson.field("variety", param.variety, &buffer);
        ordersJson.field("product", param.product, &buffer);
        ordersJson.field("order_type", param.orderType, &buffer);
        ordersJson.field("quantity", param.quantity, &buffer);
        ordersJson.field("price", param.price, &buffer);
        ordersJson.field("trigger_price", param.triggerPrice, &buffer);
    });

    return callApi<std::vector<orderMargins>, utils::json::JsonArray, true>(
        "margins.orders", { { "", ordersJson.serialize() } }, {},
        [](utils::json::JsonArray& data) {
            std::vector<orderMargins> margins;
            for (auto& i : data) { margins.emplace_back(i.GetObject()); }
            return margins;
        });
};

inline basketMargins kite::getBasketMargins(
    const std::vector<marginsParams>& params, bool considerPositions) {
    utils::json::json<utils::json::JsonArray> ordersJson;
    ordersJson.array<marginsParams>(params, [&](const marginsParams& param,
                                                rj::Value& buffer) {
        ordersJson.field("exchange", param.exchange, &buffer);
        ordersJson.field("tradingsymbol", param.tradingsymbol, &buffer);
        ordersJson.field("transaction_type", param.transactionType, &buffer);
        ordersJson.field("variety", param.variety, &buffer);
        ordersJson.field("product", param.product, &buffer);
        ordersJson.field("order_type", param.orderType, &buffer);
        ordersJson.field("quantity", param.quantity, &buffer);
        ordersJson.field("price", param.price, &buffer);
        ordersJson.field("trigger_price", param.triggerPrice, &buffer);
    });

    return callApi<basketMargins, utils::json::JsonObject>("margins.basket",
        { { "", ordersJson.serialize() } },
        { considerPositions ? "true" : "false" });
};
} // namespace kiteconnect