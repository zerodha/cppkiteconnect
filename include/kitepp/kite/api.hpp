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

#include "picosha2.h"

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {

inline kite::kite(string apikey): key(std::move(apikey)), client(root.c_str()) {
    client.set_default_headers({ { "X-Kite-Version", version } });
};

inline void kite::setApiKey(const string& arg) { key = arg; };

inline string kite::getApiKey() const { return key; };

inline string kite::loginURL() const {
    return FMT(loginUrlFmt, "api_key"_a = key);
};

inline void kite::setAccessToken(const string& arg) {
    token = arg;
    authorization = FMT("token {0}:{1}", key, token);
};

inline string kite::getAccessToken() const { return token; };

inline userSession kite::generateSession(
    const string& requestToken, const string& apiSecret) {
    return callApi<userSession, utils::json::JsonObject>("api.token",
        {
            { "api_key", key },
            { "request_token", requestToken },
            { "checksum",
                picosha2::hash256_hex_string(key + requestToken + apiSecret) },
        });
};

inline bool kite::invalidateSession() {
    utils::http::response res =
        sendReq(endpoints.at("api.token.invalidate"), {}, { key, token });
    return static_cast<bool>(res);
};
} // namespace kiteconnect
