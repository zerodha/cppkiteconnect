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
inline string kite::placeMfOrder(const placeMfOrderParams& params) {
    // required parameters
    utils::http::Params bodyParams = {
        { "tradingsymbol", params.symbol },
        { "transaction_type", params.transactionType },
    };
    // optional parameters
    utils::addParam(bodyParams, params.quantity, "quantity");
    utils::addParam(bodyParams, params.amount, "amount");
    utils::addParam(bodyParams, params.tag, "tag");

    return callApi<string, utils::json::JsonObject, true>(
        "mf.order.place", bodyParams, {}, [](utils::json::JsonObject& data) {
            return utils::json::get<string>(data, "order_id");
        });
};

inline string kite::cancelMfOrder(const string& orderId) {
    return callApi<string, utils::json::JsonObject, true>(
        "mf.order.cancel", {}, { orderId }, [](utils::json::JsonObject& data) {
            return utils::json::get<string>(data, "order_id");
        });
};

inline std::vector<mfOrder> kite::getMfOrders() {
    return callApi<std::vector<mfOrder>, utils::json::JsonArray, true>(
        "mf.orders", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<mfOrder> Orders;
            for (auto& i : data) { Orders.emplace_back(i.GetObject()); }
            return Orders;
        });
};

inline mfOrder kite::getMfOrder(const string& orderId) {
    return callApi<mfOrder, utils::json::JsonObject>(
        "mf.order.info", {}, { orderId });
};

inline std::vector<mfHolding> kite::getMfHoldings() {
    return callApi<std::vector<mfHolding>, utils::json::JsonArray, true>(
        "mf.holdings", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<mfHolding> holdings;
            for (auto& i : data) { holdings.emplace_back(i.GetObject()); }
            return holdings;
        });
};

inline placeMfSipResponse kite::placeMfSip(const placeMfSipParams& params) {
    // required parameters
    utils::http::Params bodyParams = {
        { "tradingsymbol", params.symbol },
        { "amount", std::to_string(params.amount) },
        { "instalments", std::to_string(params.installments) },
        { "frequency", params.frequency },
    };
    // optional parameters
    utils::addParam(bodyParams, params.initialAmount, "initial_amount");
    utils::addParam(bodyParams, params.installmentDay, "instalment_day");
    utils::addParam(bodyParams, params.tag, "tag");

    return callApi<placeMfSipResponse, utils::json::JsonObject>(
        "mf.sip.place", bodyParams);
};

inline string kite::modifyMfSip(const modifyMfSipParams& params) {
    utils::http::Params bodyParams = {};
    // optional parameters
    utils::addParam(bodyParams, params.amount, "amount");
    utils::addParam(bodyParams, params.status, "status");
    utils::addParam(bodyParams, params.installments, "instalments");
    utils::addParam(bodyParams, params.frequency, "frequency");
    utils::addParam(bodyParams, params.installmentDay, "instalment_day");

    return callApi<string, utils::json::JsonObject, true>("mf.sip.modify",
        bodyParams, { params.sipId }, [](utils::json::JsonObject& data) {
            return utils::json::get<string>(data, "order_id");
        });
};

inline string kite::cancelMfSip(const string& sipId) {
    return callApi<string, utils::json::JsonObject, true>(
        "mf.sip.cancel", {}, { sipId }, [](utils::json::JsonObject& data) {
            return utils::json::get<string>(data, "sip_id");
        });
};

inline std::vector<mfSip> kite::getSips() {
    return callApi<std::vector<mfSip>, utils::json::JsonArray, true>(
        "mf.sips", {}, {}, [](utils::json::JsonArray& data) {
            std::vector<mfSip> sips;
            for (auto& i : data) { sips.emplace_back(i.GetObject()); }
            return sips;
        });
};

inline mfSip kite::getSip(const string& sipId) {
    return callApi<mfSip, utils::json::JsonObject>(
        "mf.sip.info", {}, { sipId });
};

inline std::vector<mfInstrument> kite::getMfInstruments() {
    const auto response = sendReq(endpoints.at("mf.instruments"), {}, {});
    if (!response) { return {}; };

    std::vector<string> instruments = utils::parseInstruments(response.rawBody);
    return { instruments.begin(), instruments.end() };
};

} // namespace kiteconnect