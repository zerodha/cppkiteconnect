#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <vector>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline std::vector<holding> kite::holdings() {
    return callApi<std::vector<holding>, utils::json::JsonArray, true>(
        "portfolio.holdings", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<holding> Holdings;
            for (auto& i : data) { Holdings.emplace_back(i.GetObject()); }
            return Holdings;
        });
};

inline positions kite::getPositions() {
    return callApi<positions, utils::json::JsonObject>("portfolio.positions");
};

inline bool kite::convertPosition(const convertPositionParams& params) {
    utils::http::Params bodyParams = {
        { "exchange", params.exchange },
        { "tradingsymbol", params.symbol },
        { "transaction_type", params.transactionType },
        { "position_type", params.positionType },
        { "quantity", std::to_string(params.quantity) },
        { "old_product", params.oldProduct },
        { "new_product", params.newProduct },
    };
    return callApi<bool, bool>("portfolio.positions.convert", bodyParams);
};
}; // namespace kiteconnect