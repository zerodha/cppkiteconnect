#pragma once
#pragma clang diagnostic ignored "-Wundefined-inline"

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {
inline userProfile kite::profile() {
    return callApi<userProfile, utils::json::JsonObject>("user.profile");
};

inline allMargins kite::getMargins() {
    return callApi<allMargins, utils::json::JsonObject>("user.margins");
};

inline margins kite::getMargins(const string& segment) {
    return callApi<margins, utils::json::JsonObject>(
        "user.margins.segment", {}, { segment });
};
} // namespace kiteconnect
