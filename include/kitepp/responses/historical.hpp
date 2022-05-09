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
        datetime = val[0].GetString();
        open = getDouble(val[1]);
        high = getDouble(val[2]);
        low = getDouble(val[3]);
        close = getDouble(val[4]);
        volume = val[5].GetInt();
        if (val.Size() > 6) { OI = val[6].GetInt(); };
    };

    string datetime;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    int volume = 0;
    int OI = 0;
};
} // namespace kiteconnect
