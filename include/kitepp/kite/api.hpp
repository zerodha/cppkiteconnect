#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {

inline void kite::setAPIKey(const string& arg) { _apiKey = arg; };

inline string kite::getAPIKey() const { return _apiKey; };

inline string kite::loginURL() const { return FMT(_loginURLFmt, "api_key"_a = _apiKey); };

inline void kite::setAccessToken(const string& arg) { _accessToken = arg; };

inline string kite::getAccessToken() const { return _accessToken; };

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
