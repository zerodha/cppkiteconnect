/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

/**
 * @file responses.hpp
 * @brief This file has all the structs returned by kitepp
 */

#pragma once

#include <iostream> //debugging
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rjutils.hpp"
#include "utils.hpp"

#include "responses/responses.hpp"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace rju = kc::rjutils;

// kiteWS responses

/// Reoresents a single entry in market depth returned by kWS
struct depthWS {

    double price = 0.0;
    int32_t quantity = 0;
    int16_t orders = 0;
};

/// Represents a single tick returned by kWS
struct tick {

    string mode;
    int32_t instrumentToken = 0;
    bool isTradable;
    // xbool isIndex;

    int32_t timestamp = 0;
    int32_t lastTradeTime = 0;
    double lastPrice = 0.0;
    int32_t lastTradedQuantity = 0;
    int32_t totalBuyQuantity = 0;
    int32_t totalSellQuantity = 0;
    int32_t volumeTraded = 0;
    double averageTradePrice = 0.0;
    int32_t OI = 0;
    int32_t OIDayHigh = 0;
    int32_t OIDayLow = 0;
    double netChange = 0.0;

    // OHLC  OHLC
    struct ohlc {
        double open = 0.0;
        double high = 0.0;
        double low = 0.0;
        double close = 0.0;
    } OHLC;

    // Depth Depth
    struct m_depth {
        std::vector<depthWS> buy;
        std::vector<depthWS> sell;
    } marketDepth;
};

/// Represents postback sent via websockets
struct postback {

    postback() = default;

    explicit postback(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {

        rju::_getIfExists(val, orderID, "order_id");
        rju::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        rju::_getIfExists(val, placedBy, "placed_by");
        rju::_getIfExists(val, status, "status");
        rju::_getIfExists(val, statusMessage, "status_message");

        rju::_getIfExists(val, tradingSymbol, "tradingsymbol");
        rju::_getIfExists(val, exchange, "exchange");
        rju::_getIfExists(val, orderType, "order_type");
        rju::_getIfExists(val, transactionType, "transaction_type");
        rju::_getIfExists(val, validity, "validity");
        rju::_getIfExists(val, product, "product");

        rju::_getIfExists(val, averagePrice, "average_price");
        rju::_getIfExists(val, price, "price");
        rju::_getIfExists(val, quantity, "quantity");
        rju::_getIfExists(val, filledQuantity, "filled_quantity");
        rju::_getIfExists(val, unfilledQuantity, "unfilled_quantity");
        rju::_getIfExists(val, triggerPrice, "trigger_price");
        rju::_getIfExists(val, userID, "user_id");
        rju::_getIfExists(val, orderTimestamp, "order_timestamp");
        rju::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        rju::_getIfExists(val, checksum, "checksum");
    };

    string orderID;
    string exchangeOrderID;
    string placedBy;
    string status;
    string statusMessage;

    string tradingSymbol;
    string exchange;
    string orderType;
    string transactionType;
    string validity;
    string product;

    double averagePrice = 0.0;
    double price = 0.0;
    int quantity = 0;
    int filledQuantity = 0;
    int unfilledQuantity = 0;
    double triggerPrice = 0.0;
    string userID;
    string orderTimestamp;
    string exchangeTimestamp;
    string checksum;
};

} // namespace kiteconnect
