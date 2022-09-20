#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <string>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline std::unordered_map<string, quote> kite::getQuote(const std::vector<string>& symbols) {
    return callApi<std::unordered_map<string, quote>, utils::json::JsonObject, true>(
        "market.quote", {}, { encodeSymbolsList(symbols) }, [](utils::json::JsonObject& data) {
            std::unordered_map<string, quote> Quotes;
            for (auto& i : data) { Quotes.emplace(i.name.GetString(), i.value.GetObject()); };
            return Quotes;
        });
};

inline std::unordered_map<string, ohlcQuote> kite::getOhlc(const std::vector<string>& symbols) {
    return callApi<std::unordered_map<string, ohlcQuote>, utils::json::JsonObject, true>(
        "market.quote.ohlc", {}, { encodeSymbolsList(symbols) }, [](utils::json::JsonObject& data) {
            std::unordered_map<string, ohlcQuote> Quotes;
            for (auto& i : data) { Quotes.emplace(i.name.GetString(), i.value.GetObject()); };
            return Quotes;
        });
};

inline std::unordered_map<string, ltpQuote> kite::getLtp(const std::vector<string>& symbols) {
    return callApi<std::unordered_map<string, ltpQuote>, utils::json::JsonObject, true>(
        "market.quote.ltp", {}, { encodeSymbolsList(symbols) }, [](utils::json::JsonObject& data) {
            std::unordered_map<string, ltpQuote> Quotes;
            for (auto& i : data) { Quotes.emplace(i.name.GetString(), i.value.GetObject()); };
            return Quotes;
        });
};

inline std::vector<historicalData> kite::getHistoricalData(const historicalDataParams& params) {
    static const auto toString = [](bool val) { return val ? "1" : "0"; };
    return callApi<std::vector<historicalData>, utils::json::JsonObject, true>("market.historical", {},
        { std::to_string(params.instrumentToken), params.interval, params.from, params.to, toString(params.continuous),
            toString(params.oi) },
        [](utils::json::JsonObject& data) {
            std::vector<historicalData> candles;
            for (auto& i : data["candles"].GetArray()) { candles.emplace_back(i.GetArray()); }
            return candles;
        });
};

inline std::vector<instrument> kite::getInstruments(const string& exchange) {
    utils::FmtArgs fmtArgs = {};
    utils::http::endpoint endpoint;
    if (exchange.empty()) {
        endpoint = endpoints.at("market.instruments.all");
    } else {
        endpoint = endpoints.at("market.instruments");
        fmtArgs.emplace_back(exchange);
    }
    const auto response = sendReq(endpoint, {}, fmtArgs);
    if (!response) { return {}; };

    std::vector<string> instruments = utils::parseInstruments(response.rawBody);
    return { instruments.begin(), instruments.end() };
};

}; // namespace kiteconnect