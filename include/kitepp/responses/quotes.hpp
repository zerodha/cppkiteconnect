#pragma once

#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

/// contains OHLC information of a security
struct ohlc {
    ohlc() = default;
    explicit ohlc(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, open, "open");
        kc::rjutils::_getIfExists(val, high, "high");
        kc::rjutils::_getIfExists(val, low, "low");
        kc::rjutils::_getIfExists(val, close, "close");
    };

    double open = kc::DEFAULTDOUBLE;
    double high = kc::DEFAULTDOUBLE;
    double low = kc::DEFAULTDOUBLE;
    double close = kc::DEFAULTDOUBLE;
};

/// represents market depth
struct depth {
    depth() = default;
    explicit depth(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, price, "price");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, orders, "orders");
    };

    int quantity = kc::DEFAULTINT;
    int orders = 0;
    double price = kc::DEFAULTDOUBLE;
};

/// represents a full quote response
struct quote {
    quote() = default;
    explicit quote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, timestamp, "timestamp");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
        kc::rjutils::_getIfExists(val, lastQuantity, "last_quantity");
        kc::rjutils::_getIfExists(val, lastTradeTime, "last_trade_time");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, volume, "volume");
        kc::rjutils::_getIfExists(val, buyQuantity, "buy_quantity");
        kc::rjutils::_getIfExists(val, sellQuantity, "sell_quantity");

        rj::Value ohlcVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, ohlcVal, "ohlc", kc::rjutils::_RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());

        kc::rjutils::_getIfExists(val, netChange, "net_change");
        kc::rjutils::_getIfExists(val, OI, "oi");
        kc::rjutils::_getIfExists(val, OIDayHigh, "oi_day_high");
        kc::rjutils::_getIfExists(val, OIDayLow, "oi_day_low");
        kc::rjutils::_getIfExists(val, lowerCircuitLimit, "lower_circuit_limit");
        kc::rjutils::_getIfExists(val, upperCircuitLimit, "upper_circuit_limit");

        rj::Value tmpVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, tmpVal, "depth", kc::rjutils::_RJValueType::Object);
        auto depthVal = tmpVal.GetObject();

        rj::Value buyDepthVal(rj::kArrayType);
        kc::rjutils::_getIfExists(depthVal, buyDepthVal, "buy", kc::rjutils::_RJValueType::Array);
        for (auto& i : buyDepthVal.GetArray()) { marketDepth.buy.emplace_back(i.GetObject()); };

        rj::Value sellDepthVal(rj::kArrayType);
        kc::rjutils::_getIfExists(depthVal, sellDepthVal, "sell", kc::rjutils::_RJValueType::Array);
        for (auto& i : sellDepthVal.GetArray()) { marketDepth.sell.emplace_back(i.GetObject()); };
    };

    int instrumentToken = kc::DEFAULTINT;
    int lastQuantity = kc::DEFAULTINT;
    int volume = kc::DEFAULTINT;
    int buyQuantity = kc::DEFAULTINT;
    int sellQuantity = kc::DEFAULTINT;
    double lastPrice = kc::DEFAULTDOUBLE;
    double averagePrice = kc::DEFAULTDOUBLE;
    double netChange = kc::DEFAULTDOUBLE;
    double OI = kc::DEFAULTDOUBLE;
    double OIDayHigh = kc::DEFAULTDOUBLE;
    double OIDayLow = kc::DEFAULTDOUBLE;
    double lowerCircuitLimit = kc::DEFAULTDOUBLE;
    double upperCircuitLimit = kc::DEFAULTDOUBLE;
    string timestamp;
    string lastTradeTime;
    ohlc OHLC;
    struct mDepth {
        std::vector<depth> buy;
        std::vector<depth> sell;

    } marketDepth;
};

/// represents a ohlc quote response
struct OHLCQuote {
    OHLCQuote() = default;
    explicit OHLCQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");

        rj::Value ohlcVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, ohlcVal, "ohlc", kc::rjutils::_RJValueType::Object);
        OHLC.parse(ohlcVal.GetObject());
    };

    int instrumentToken = kc::DEFAULTINT;
    double lastPrice = kc::DEFAULTDOUBLE;
    ohlc OHLC;
};

/// represents a ltp quote response
struct LTPQuote {
    LTPQuote() = default;
    explicit LTPQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
    };

    int instrumentToken = kc::DEFAULTINT;
    double lastPrice = kc::DEFAULTDOUBLE;
};
} // namespace kiteconnect