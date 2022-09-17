#pragma once

#include <functional>
#include <type_traits>

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {

// GMock requires mock methods to be virtual (hi-perf dep injection is not possible here ಥ﹏ಥ).
// Macro used to eliminate vptr overhead.
inline utils::http::response kite::sendReq(
    const utils::http::endpoint& endpoint, const utils::http::Params& body, const utils::FmtArgs& fmtArgs) {
    if (endpoint.contentType == utils::http::CONTENT_TYPE::JSON) {
        return utils::http::request { endpoint.method, endpoint.Path(fmtArgs), _getAuthStr(), body,
            endpoint.contentType, body.begin()->second }
            .send(_httpClient);
    }
    return utils::http::request { endpoint.method, endpoint.Path(fmtArgs), _getAuthStr(), body, endpoint.contentType }
        .send(_httpClient);
};

template <class Res, class Data, bool UseCustomParser>
inline Res kite::callApi(const string& service, const utils::http::Params& body, const utils::FmtArgs& fmtArgs,
    utils::json::CustomParser<Res, Data, UseCustomParser> customParser) {
    utils::http::response res = sendReq(endpoints.at(service), body, fmtArgs);
    if (!res) { kiteconnect::_throwException(res.errorType, res.code, res.message); }
    return utils::json::parse<Res, Data, UseCustomParser>(res.data, customParser);
}
} // namespace kiteconnect
