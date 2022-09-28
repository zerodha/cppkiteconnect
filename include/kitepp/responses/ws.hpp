#pragma once

#include <cstdint>
#include <string>

#include "../config.hpp"
#include "../utils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace kc = kiteconnect;
namespace rj = rapidjson;
namespace utils = kc::internal::utils;

/// represents a single entry in market depth returned by kWS
struct depthWS {
    int16_t orders = -1;
    int32_t quantity = -1;
    double price = -1;
};

/// represents a single tick returned by kWS
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

/// represents postback sent via websockets
struct postback {
    postback() = default;
    explicit postback(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        orderId = utils::json::get<string>(val, "order_id");
        exchangeOrderId = utils::json::get<string>(val, "exchange_order_id");
        placedBy = utils::json::get<string>(val, "placed_by");
        status = utils::json::get<string>(val, "status");
        statusMessage = utils::json::get<string>(val, "status_message");
        tradingSymbol = utils::json::get<string>(val, "tradingsymbol");
        exchange = utils::json::get<string>(val, "exchange");
        orderType = utils::json::get<string>(val, "order_type");
        transactionType = utils::json::get<string>(val, "transaction_type");
        validity = utils::json::get<string>(val, "validity");
        product = utils::json::get<string>(val, "product");
        averagePrice = utils::json::get<double>(val, "average_price");
        price = utils::json::get<double>(val, "price");
        quantity = utils::json::get<int>(val, "quantity");
        filledQuantity = utils::json::get<int>(val, "filled_quantity");
        unfilledQuantity = utils::json::get<int>(val, "unfilled_quantity");
        triggerPrice = utils::json::get<double>(val, "trigger_price");
        userId = utils::json::get<string>(val, "user_id");
        orderTimestamp = utils::json::get<string>(val, "order_timestamp");
        exchangeTimestamp = utils::json::get<string>(val, "exchange_timestamp");
        checksum = utils::json::get<string>(val, "checksum");
    };

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