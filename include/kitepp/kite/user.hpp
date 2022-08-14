#pragma once

#include "../kite.hpp"

namespace kiteconnect {

inline userSession kite::generateSession(const string& requestToken, const string& apiSecret) {
    return callApi<userSession, utils::json::PARSE_AS::OBJECT>(
        "api.token", {
                         { "api_key", _apiKey },
                         { "request_token", requestToken },
                         { "checksum", picosha2::hash256_hex_string(_apiKey + requestToken + apiSecret) },
                     });
};

inline userProfile kite::profile() { return callApi<userProfile, utils::json::PARSE_AS::OBJECT>("user.profile"); };

} // namespace kiteconnect
