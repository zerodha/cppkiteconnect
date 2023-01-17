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