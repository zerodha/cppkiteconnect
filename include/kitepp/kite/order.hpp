#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include <vector>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline string kite::placeOrder(const placeOrderParams& params) {
    // required parameters
    utils::http::Params bodyParams = {
        { "exchange", params.exchange },
        { "tradingsymbol", params.symbol },
        { "transaction_type", params.transactionType },
        { "quantity", std::to_string(params.quantity) },
        { "product", params.product },
        { "order_type", params.orderType },
    };
    // optional parameters
    utils::addParam(bodyParams, params.price, "price");
    utils::addParam(bodyParams, params.validity, "validity");
    utils::addParam(bodyParams, params.disclosedQuantity, "disclosed_quantity");
    utils::addParam(bodyParams, params.triggerPrice, "trigger_price");
    utils::addParam(bodyParams, params.squareOff, "squareoff");
    utils::addParam(bodyParams, params.stopLoss, "stoploss");
    utils::addParam(bodyParams, params.trailingStopLoss, "trailing_stoploss");
    utils::addParam(bodyParams, params.tag, "tag");

    return callApi<string, utils::json::JsonObject, true>(
        "order.place", bodyParams, { params.variety }, [](utils::json::JsonObject& data) {
            string orderId;
            rju::_getIfExists(data, orderId, "order_id");
            return orderId;
        });
};

inline string kite::modifyOrder(const modifyOrderParams& params) {
    utils::http::Params bodyParams = {};
    // optional parameters
    utils::addParam(bodyParams, params.parentOrderId, "parent_order_id");
    utils::addParam(bodyParams, params.quantity, "quantity");
    utils::addParam(bodyParams, params.price, "price");
    utils::addParam(bodyParams, params.orderType, "order_type");
    utils::addParam(bodyParams, params.triggerPrice, "trigger_price");
    utils::addParam(bodyParams, params.validity, "validity");
    utils::addParam(bodyParams, params.disclosedQuantity, "disclosed_quantity");

    return callApi<string, utils::json::JsonObject, true>(
        "order.modify", bodyParams, { params.variety, params.orderId }, [](utils::json::JsonObject& data) {
            string orderId;
            rju::_getIfExists(data, orderId, "order_id");
            return orderId;
        });
};

inline string kite::cancelOrder(const string& variety, const string& orderId, const string& parentOrderID) {
    string endpoint;
    utils::FmtArgs fmtArgs;
    if (variety == "bo") {
        endpoint = "order.cancel.bo";
        fmtArgs = { variety, orderId, parentOrderID };
    } else {
        endpoint = "order.cancel";
        fmtArgs = { variety, orderId };
    };
    return callApi<string, utils::json::JsonObject, true>(endpoint, {}, fmtArgs, [](utils::json::JsonObject& data) {
        string orderId;
        rju::_getIfExists(data, orderId, "order_id");
        return orderId;
    });
};

inline string kite::exitOrder(const string& variety, const string& orderId, const string& parentOrderId) {
    return cancelOrder(variety, orderId, parentOrderId);
};

inline std::vector<order> kite::orders() {
    return callApi<std::vector<order>, utils::json::JsonArray, true>(
        "orders", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<order> Orders;
            for (auto& i : data) { Orders.emplace_back(i.GetObject()); }
            return Orders;
        });
};

inline std::vector<order> kite::orderHistory(const string& orderId) {
    return callApi<std::vector<order>, utils::json::JsonArray, true>(
        "order.info", {}, { orderId }, [](utils::json::JsonArray& data) {
            std::vector<order> history;
            for (auto& i : data) { history.emplace_back(i.GetObject()); }
            return history;
        });
};

inline std::vector<trade> kite::trades() {
    return callApi<std::vector<trade>, utils::json::JsonArray, true>(
        "trades", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<trade> trades;
            for (auto& i : data) { trades.emplace_back(i.GetObject()); }
            return trades;
        });
};

inline std::vector<trade> kite::orderTrades(const string& orderId) {
    return callApi<std::vector<trade>, utils::json::JsonArray, true>(
        "order.trades", {}, { orderId }, [](utils::json::JsonArray& data) {
            std::vector<trade> trades;
            for (auto& i : data) { trades.emplace_back(i.GetObject()); }
            return trades;
        });
};
} // namespace kiteconnect