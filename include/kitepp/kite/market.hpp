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
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <string>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline std::unordered_map<string, quote> kite::getQuote(
    const std::vector<string>& symbols) {
    return callApi<std::unordered_map<string, quote>, utils::json::JsonObject,
        true>("market.quote", {}, { encodeSymbolsList(symbols) },
        [](utils::json::JsonObject& data) {
            std::unordered_map<string, quote> Quotes;
            for (auto& i : data) {
                Quotes.emplace(i.name.GetString(), i.value.GetObject());
            };
            return Quotes;
        });
};

inline std::unordered_map<string, ohlcQuote> kite::getOhlc(
    const std::vector<string>& symbols) {
    return callApi<std::unordered_map<string, ohlcQuote>,
        utils::json::JsonObject, true>("market.quote.ohlc", {},
        { encodeSymbolsList(symbols) }, [](utils::json::JsonObject& data) {
            std::unordered_map<string, ohlcQuote> Quotes;
            for (auto& i : data) {
                Quotes.emplace(i.name.GetString(), i.value.GetObject());
            };
            return Quotes;
        });
};

inline std::unordered_map<string, ltpQuote> kite::getLtp(
    const std::vector<string>& symbols) {
    return callApi<std::unordered_map<string, ltpQuote>,
        utils::json::JsonObject, true>("market.quote.ltp", {},
        { encodeSymbolsList(symbols) }, [](utils::json::JsonObject& data) {
            std::unordered_map<string, ltpQuote> Quotes;
            for (auto& i : data) {
                Quotes.emplace(i.name.GetString(), i.value.GetObject());
            };
            return Quotes;
        });
};

inline std::vector<historicalData> kite::getHistoricalData(
    const historicalDataParams& params) {
    static const auto toString = [](bool val) { return val ? "1" : "0"; };
    return callApi<std::vector<historicalData>, utils::json::JsonObject, true>(
        "market.historical", {},
        { std::to_string(params.instrumentToken), params.interval, params.from,
            params.to, toString(params.continuous), toString(params.oi) },
        [](utils::json::JsonObject& data) {
            std::vector<historicalData> candles;
            for (auto& i : data["candles"].GetArray()) {
                candles.emplace_back(i.GetArray());
            }
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