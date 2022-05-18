#pragma once

#include <cstdint>
#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "../utils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace kc = kiteconnect;
namespace rj = rapidjson;

/// represents a single entry in market depth returned by kWS
struct depthWS {
    int16_t orders = kc::DEFAULTINT;
    int32_t quantity = kc::DEFAULTINT;
    double price = kc::DEFAULTDOUBLE;
};

/// represents a single tick returned by kWS
struct tick {
    int32_t instrumentToken = kc::DEFAULTINT;
    int32_t timestamp = kc::DEFAULTINT;
    int32_t lastTradeTime = kc::DEFAULTINT;
    int32_t lastTradedQuantity = kc::DEFAULTINT;
    int32_t totalBuyQuantity = kc::DEFAULTINT;
    int32_t totalSellQuantity = kc::DEFAULTINT;
    int32_t volumeTraded = kc::DEFAULTINT;
    int32_t oi = kc::DEFAULTINT;
    int32_t oiDayHigh = kc::DEFAULTINT;
    int32_t oiDayLow = kc::DEFAULTINT;
    string mode;
    double lastPrice = kc::DEFAULTDOUBLE;
    double averageTradePrice = kc::DEFAULTDOUBLE;
    double netChange = kc::DEFAULTDOUBLE;
    bool isTradable;
    struct OHLC {
        double open = kc::DEFAULTDOUBLE;
        double high = kc::DEFAULTDOUBLE;
        double low = kc::DEFAULTDOUBLE;
        double close = kc::DEFAULTDOUBLE;
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
        kc::rjutils::_getIfExists(val, orderId, "order_id");
        kc::rjutils::_getIfExists(val, exchangeOrderId, "exchange_order_id");
        kc::rjutils::_getIfExists(val, placedBy, "placed_by");
        kc::rjutils::_getIfExists(val, status, "status");
        kc::rjutils::_getIfExists(val, statusMessage, "status_message");
        kc::rjutils::_getIfExists(val, tradingSymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, orderType, "order_type");
        kc::rjutils::_getIfExists(val, transactionType, "transaction_type");
        kc::rjutils::_getIfExists(val, validity, "validity");
        kc::rjutils::_getIfExists(val, product, "product");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, price, "price");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, filledQuantity, "filled_quantity");
        kc::rjutils::_getIfExists(val, unfilledQuantity, "unfilled_quantity");
        kc::rjutils::_getIfExists(val, triggerPrice, "trigger_price");
        kc::rjutils::_getIfExists(val, userId, "user_id");
        kc::rjutils::_getIfExists(val, orderTimestamp, "order_timestamp");
        kc::rjutils::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        kc::rjutils::_getIfExists(val, checksum, "checksum");
    };

    int quantity = kc::DEFAULTINT;
    int filledQuantity = kc::DEFAULTINT;
    int unfilledQuantity = kc::DEFAULTINT;
    double averagePrice = kc::DEFAULTDOUBLE;
    double price = kc::DEFAULTDOUBLE;
    double triggerPrice = kc::DEFAULTDOUBLE;
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