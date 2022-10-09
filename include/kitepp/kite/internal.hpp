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
        kiteconnect::throwException(res.errorType, res.code, res.message);
    }
    return utils::json::parse<Res, Data, UseCustomParser>(
        res.data, customParser);
}
} // namespace kiteconnect
