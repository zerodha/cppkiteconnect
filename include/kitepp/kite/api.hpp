#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include "PicoSHA2/picosha2.h"

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {

inline kite::kite(string apikey): key(std::move(apikey)), client(root.c_str()) {
    client.set_default_headers({ { "X-Kite-Version", version } });
};

inline void kite::setApiKey(const string& arg) { key = arg; };

inline string kite::getApiKey() const { return key; };

inline string kite::loginURL() const { return FMT(loginUrlFmt, "api_key"_a = key); };

inline void kite::setAccessToken(const string& arg) {
    token = arg;
    authorization = FMT("token {0}:{1}", key, token);
};

inline string kite::getAccessToken() const { return token; };

inline userSession kite::generateSession(const string& requestToken, const string& apiSecret) {
    return callApi<userSession, utils::json::JsonObject>(
        "api.token", {
                         { "api_key", key },
                         { "request_token", requestToken },
                         { "checksum", picosha2::hash256_hex_string(key + requestToken + apiSecret) },
                     });
};

inline bool kite::invalidateSession() {
    utils::http::response res = sendReq(endpoints.at("api.token.invalidate"), {}, { key, token });
    return static_cast<bool>(res);
};
} // namespace kiteconnect
