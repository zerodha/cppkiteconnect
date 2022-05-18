#pragma once

#include <string>

#include "../config.hpp"
#include "../kiteppexceptions.hpp"
#include "../utils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

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
} // namespace kiteconnect
