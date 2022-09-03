#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline userSession kite::generateSession(const string& requestToken, const string& apiSecret) {
    return callApi<userSession, utils::json::JsonObject>(
        "api.token", {
                         { "api_key", _apiKey },
                         { "request_token", requestToken },
                         { "checksum", picosha2::hash256_hex_string(_apiKey + requestToken + apiSecret) },
                     });
};

inline bool kite::invalidateSession() {
    utils::http::response res = sendReq(endpoints.at("api.token.invalidate"), {}, { _apiKey, _accessToken });
    return static_cast<bool>(res);
};
} // namespace kiteconnect
