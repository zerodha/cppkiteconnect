#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <string>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
namespace internal {
using std::string;

template <class T>
string getConditionJson(const T& params) {
    utils::json::json<utils::json::JsonObject> conditionJson;
    conditionJson.field("exchange", params.exchange);
    conditionJson.field("tradingsymbol", params.symbol);
    conditionJson.field("trigger_values", params.triggerValues);
    conditionJson.field("last_price", params.lastPrice);
    return conditionJson.serialize();
}

template <class T>
string getOrdersJson(const T& params) {
    utils::json::json<utils::json::JsonArray> ordersJson;
    ordersJson.array<gttParams>(params.gttParamsList, [&](const gttParams& param, rj::Value& buffer) {
        ordersJson.field("exchange", params.exchange, &buffer);
        ordersJson.field("tradingsymbol", params.symbol, &buffer);
        ordersJson.field("transaction_type", param.transactionType, &buffer);
        ordersJson.field("quantity", param.quantity, &buffer);
        ordersJson.field("order_type", param.orderType, &buffer);
        ordersJson.field("product", param.product, &buffer);
        ordersJson.field("price", param.price, &buffer);
    });
    return ordersJson.serialize();
}
}; // namespace internal

inline int kite::placeGtt(const placeGttParams& params) {
    utils::http::Params reqParams = {
        { "type", params.triggerType },
        { "condition", internal::getConditionJson(params) },
        { "orders", internal::getOrdersJson(params) },
    };

    return callApi<int, utils::json::JsonObject, true>("gtt.place", reqParams, {}, [](utils::json::JsonObject& data) {
        int triggerId = -1;
        rju::_getIfExists(data, triggerId, "trigger_id");
        return triggerId;
    });
};

inline std::vector<GTT> kite::triggers() {
    return callApi<std::vector<GTT>, utils::json::JsonArray, true>("gtt", {}, {}, [](utils::json::JsonArray& data) {
        std::vector<GTT> Triggers;
        for (auto& i : data) { Triggers.emplace_back(i.GetObject()); }
        return Triggers;
    });
};

inline GTT kite::getGtt(int triggerId) {
    return callApi<GTT, utils::json::JsonObject>("gtt.info", {}, { std::to_string(triggerId) });
};

inline int kite::modifyGtt(const kc::modifyGttParams& params) {
    utils::http::Params reqParams = {
        { "type", params.triggerType },
        { "condition", internal::getConditionJson(params) },
        { "orders", internal::getOrdersJson(params) },
    };

    return callApi<int, utils::json::JsonObject, true>(
        "gtt.modify", reqParams, { std::to_string(params.triggerId) }, [](utils::json::JsonObject& data) {
            int triggerId = -1;
            rju::_getIfExists(data, triggerId, "trigger_id");
            return triggerId;
        });
};

inline int kite::deleteGtt(int triggerId) {
    return callApi<int, utils::json::JsonObject, true>(
        "gtt.delete", {}, { std::to_string(triggerId) }, [](utils::json::JsonObject& data) {
            int triggerId = -1;
            rju::_getIfExists(data, triggerId, "trigger_id");
            return triggerId;
        });
};

} // namespace kiteconnect