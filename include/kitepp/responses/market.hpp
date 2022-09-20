#pragma once

#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "../utils.hpp"
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
struct ohlcQuote {
    ohlcQuote() = default;
    explicit ohlcQuote(const rj::Value::Object& val) { parse(val); };

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
struct ltpQuote {
    ltpQuote() = default;
    explicit ltpQuote(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
    };

    int instrumentToken = kc::DEFAULTINT;
    double lastPrice = kc::DEFAULTDOUBLE;
};

/// represents parameters required for the `getHistoricalData` function
struct historicalDataParams {
    GENERATE_FLUENT_METHOD(historicalDataParams, int, instrumentToken, InstrumentToken);
    GENERATE_FLUENT_METHOD(historicalDataParams, bool, continuous, Continuous);
    GENERATE_FLUENT_METHOD(historicalDataParams, bool, oi, Oi);
    GENERATE_FLUENT_METHOD(historicalDataParams, const string&, from, From);
    GENERATE_FLUENT_METHOD(historicalDataParams, const string&, to, To);
    GENERATE_FLUENT_METHOD(historicalDataParams, const string&, interval, Interval);

    int instrumentToken = kc::DEFAULTINT;
    bool continuous = false;
    bool oi = false;
    string from;
    string to;
    string interval;
};

/// represents historical data call reponse
struct historicalData {
    historicalData() = default;
    explicit historicalData(const rj::Value::Array& val) { parse(val); };

    void parse(const rj::Value::Array& val) {
        /// in case returned number doesn't have decimal point.
        /// Directly calling GetDouble() will cause error if number
        /// doesn't have decimal.
        static auto getDouble = [](rj::Value& val) -> double {
            if (val.IsDouble()) { return val.GetDouble(); };
            if (val.IsInt()) { return val.GetInt(); };

            throw libException("type wasn't the one expected (expected double) (historicalData)");
        };
        datetime = val[DATETIME_IDX].GetString();
        open = getDouble(val[OPEN_IDX]);
        high = getDouble(val[HIGH_IDX]);
        low = getDouble(val[LOW_IDX]);
        close = getDouble(val[CLOSE_IDX]);
        volume = val[VOLUME_IDX].GetInt();
        if (val.Size() > OI_IDX) { OI = val[OI_IDX].GetInt(); };
    };

    int volume = kc::DEFAULTINT;
    int OI = kc::DEFAULTINT;
    double open = kc::DEFAULTDOUBLE;
    double high = kc::DEFAULTDOUBLE;
    double low = kc::DEFAULTDOUBLE;
    double close = kc::DEFAULTDOUBLE;
    string datetime;
    static constexpr uint8_t DATETIME_IDX = 0;
    static constexpr uint8_t OPEN_IDX = 1;
    static constexpr uint8_t HIGH_IDX = 2;
    static constexpr uint8_t LOW_IDX = 3;
    static constexpr uint8_t CLOSE_IDX = 4;
    static constexpr uint8_t VOLUME_IDX = 5;
    static constexpr uint8_t OI_IDX = 6;
};

/// represents a position from the margin calculation API
struct orderMarginsParams {
    orderMarginsParams() = default;

    GENERATE_FLUENT_METHOD(orderMarginsParams, double, quantity, Quantity);
    GENERATE_FLUENT_METHOD(orderMarginsParams, double, price, Price);
    GENERATE_FLUENT_METHOD(orderMarginsParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, tradingsymbol, Tradingsymbol);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, orderType, OrderType);

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

/// represents response from the margin calculation API
struct orderMargins {
    orderMargins() = default;
    explicit orderMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, type, "type");
        kc::rjutils::_getIfExists(val, tradingSymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, span, "span");
        kc::rjutils::_getIfExists(val, exposure, "exposure");
        kc::rjutils::_getIfExists(val, optionPremium, "option_premium");
        kc::rjutils::_getIfExists(val, additional, "additional");
        kc::rjutils::_getIfExists(val, bo, "bo");
        kc::rjutils::_getIfExists(val, cash, "cash");
        kc::rjutils::_getIfExists(val, var, "var");

        rj::Value pnlVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, pnlVal, "pnl", kc::rjutils::_RJValueType::Object);
        pnl.parse(pnlVal.GetObject());

        kc::rjutils::_getIfExists(val, total, "total");
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
            kc::rjutils::_getIfExists(val, realised, "realised");
            kc::rjutils::_getIfExists(val, unrealised, "unrealised");
        };

        double realised = -1;
        double unrealised = -1;
    } pnl;
};
/// represents an individual instrument response
struct instrument {
    instrument() = default;
    explicit instrument(const string& val) { parse(val); };

    void parse(const string& val) {
        std::vector<string> tokens = kc::_split(val, ',');

        static const auto toInt = [](const string& str) -> int { return (str.empty()) ? 0 : std::stoi(str); };
        static const auto toDouble = [](const string& str) -> double { return (str.empty()) ? 0.0 : std::stod(str); };

        instrumentToken = toInt(tokens[INSTRUMENT_TOKEN_IDX]);
        exchangeToken = toInt(tokens[EXCHANGE_TOKEN_IDX]);
        tradingsymbol = tokens[TRADINGSYMBOL_IDX];
        name = tokens[NAME_IDX];
        lastPrice = toDouble(tokens[LAST_PRICE_IDX]);
        expiry = tokens[EXPIRY_IDX];
        strikePrice = toDouble(tokens[STRIKE_PRICE_IDX]);
        tickSize = toDouble(tokens[TICK_SIZE_IDX]);
        lotSize = toDouble(tokens[LOT_SIZE_IDX]);
        instrumentType = tokens[INSTRUMENT_TYPE_IDX];
        segment = tokens[SEGMENT_IDX];
        exchange = tokens[EXCHANGE_IDX];
    };

    int instrumentToken = kc::DEFAULTINT;
    int exchangeToken = kc::DEFAULTINT;
    double lastPrice = kc::DEFAULTDOUBLE;
    double strikePrice = kc::DEFAULTDOUBLE;
    double tickSize = kc::DEFAULTDOUBLE;
    double lotSize = kc::DEFAULTDOUBLE;
    string tradingsymbol;
    string name;
    string expiry;
    string instrumentType;
    string segment;
    string exchange;

    static constexpr uint8_t INSTRUMENT_TOKEN_IDX = 0;
    static constexpr uint8_t EXCHANGE_TOKEN_IDX = 1;
    static constexpr uint8_t TRADINGSYMBOL_IDX = 2;
    static constexpr uint8_t NAME_IDX = 3;
    static constexpr uint8_t LAST_PRICE_IDX = 4;
    static constexpr uint8_t EXPIRY_IDX = 5;
    static constexpr uint8_t STRIKE_PRICE_IDX = 6;
    static constexpr uint8_t TICK_SIZE_IDX = 7;
    static constexpr uint8_t LOT_SIZE_IDX = 8;
    static constexpr uint8_t INSTRUMENT_TYPE_IDX = 9;
    static constexpr uint8_t SEGMENT_IDX = 10;
    static constexpr uint8_t EXCHANGE_IDX = 11;
};

} // namespace kiteconnect