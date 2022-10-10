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

#include <functional>
#include <type_traits>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {

inline string kite::getAuth() const { return authorization; }

inline string kite::encodeSymbolsList(const std::vector<string>& symbols) {
    string str;
    for (const auto& symbol : symbols) { str.append(FMT("i={0}&", symbol)); };
    if (!str.empty()) { str.pop_back(); };
    return str;
}

// GMock requires mock methods to be virtual (hi-perf dep injection is not
// possible here ಥ﹏ಥ). Macro used to eliminate vptr overhead.
inline utils::http::response kite::sendReq(
    const utils::http::endpoint& endpoint, const utils::http::Params& body,
    const utils::FmtArgs& fmtArgs) {
    if (endpoint.contentType == utils::http::CONTENT_TYPE::JSON) {
        return utils::http::request { endpoint.method, endpoint.Path(fmtArgs),
            getAuth(), body, endpoint.contentType, endpoint.responseType,
            body.begin()->second }
            .send(client);
    }
    return utils::http::request { endpoint.method, endpoint.Path(fmtArgs),
        getAuth(), body, endpoint.contentType }
        .send(client);
};

template <class Res, class Data, bool UseCustomParser>
inline Res kite::callApi(const string& service, const utils::http::Params& body,
    const utils::FmtArgs& fmtArgs,
    utils::json::CustomParser<Res, Data, UseCustomParser> customParser) {
    utils::http::response res = sendReq(endpoints.at(service), body, fmtArgs);
    if (!res) {
        kiteconnect::internal::throwException(
            res.errorType, res.code, res.message);
    }
    return utils::json::parse<Res, Data, UseCustomParser>(
        res.data, customParser);
}
} // namespace kiteconnect
