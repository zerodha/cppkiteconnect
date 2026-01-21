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

#include <cstdint>
#include <string>
#include <vector>

#include <glaze/glaze.hpp>

namespace kiteconnect {

using std::string;
namespace kc = kiteconnect;

/// Represents a single entry in market depth returned by `ticker`.
struct depthWS {
    int16_t orders = -1;
    int32_t quantity = -1;
    double price = -1;
};

/// Represents a single market data tick.
struct tick {
    int32_t instrumentToken = -1;
    int32_t timestamp = -1;
    int32_t lastTradeTime = -1;
    int32_t lastTradedQuantity = -1;
    int32_t totalBuyQuantity = -1;
    int32_t totalSellQuantity = -1;
    int32_t volumeTraded = -1;
    int32_t oi = -1;
    int32_t oiDayHigh = -1;
    int32_t oiDayLow = -1;
    string mode;
    double lastPrice = -1;
    double averageTradePrice = -1;
    double netChange = -1;
    bool isTradable;
    struct OHLC {
        double open = -1;
        double high = -1;
        double low = -1;
        double close = -1;
    } ohlc;
    struct m_depth {
        std::vector<depthWS> buy;
        std::vector<depthWS> sell;
    } marketDepth;
};

/// Represents a postback.
struct postback {
    postback() = default;

    int quantity = -1;
    int filledQuantity = -1;
    int unfilledQuantity = -1;
    double averagePrice = -1;
    double price = -1;
    double triggerPrice = -1;
    string orderId;
    string exchangeOrderId;
    string placedBy;
    string status;
    string statusMessage;
    string tradingSymbol;
    string exchange;
    string orderType;
    string transactionType;
    string validity;
    string product;
    string userId;
    string orderTimestamp;
    string exchangeTimestamp;
    string checksum;
};

} // namespace kiteconnect

/// glaze metadata for postback struct (maps JSON snake_case to C++ camelCase)
template <>
struct glz::meta<kiteconnect::postback> {
    using T = kiteconnect::postback;
    static constexpr auto value = object(
        "quantity", &T::quantity,
        "filled_quantity", &T::filledQuantity,
        "unfilled_quantity", &T::unfilledQuantity,
        "average_price", &T::averagePrice,
        "price", &T::price,
        "trigger_price", &T::triggerPrice,
        "order_id", &T::orderId,
        "exchange_order_id", &T::exchangeOrderId,
        "placed_by", &T::placedBy,
        "status", &T::status,
        "status_message", &T::statusMessage,
        "tradingsymbol", &T::tradingSymbol,
        "exchange", &T::exchange,
        "order_type", &T::orderType,
        "transaction_type", &T::transactionType,
        "validity", &T::validity,
        "product", &T::product,
        "user_id", &T::userId,
        "order_timestamp", &T::orderTimestamp,
        "exchange_timestamp", &T::exchangeTimestamp,
        "checksum", &T::checksum
    );
};
