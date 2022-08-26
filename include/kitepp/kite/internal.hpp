#pragma once

#include "../kite.hpp"
#include "../utils.hpp"

namespace kiteconnect {

// GMock requires mock methods to be virtual (hi-perf dep injection is not possible here ಥ﹏ಥ).
// Macro used to eliminate vptr overhead.
inline utils::http::response kite::sendReq(
    const utils::http::endpoint& endpoint, const utils::http::paramsT& body, const utils::fmtArgsT& fmtArgs) {
    return utils::http::request { endpoint.method, endpoint.Path(fmtArgs), _getAuthStr(), body, endpoint.contentType }
        .send(_httpClient);
};

template <class resT, utils::json::PARSE_AS resBodyT>
inline resT kite::callApi(const string& service, const utils::http::paramsT& body, const utils::fmtArgsT& fmtArgs) {
    //! move resBodyT to `endpoint` if non-hacky constexpr std maps come along
    utils::http::response res = sendReq(endpoints.at(service), body, fmtArgs);
    if (!res) { kiteconnect::_throwException(res.errorType, res.code, res.message); }
    return utils::json::parse<resT, resBodyT>(res.data);
}
} // namespace kiteconnect
