#pragma once

#include <string>

#include <gmock/gmock.h>

#include "./kitepp.hpp"

namespace kiteconnect::test {

using std::string;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

// NOLINTBEGIN(cert-err58-cpp)
inline const string API_KEY = "Uz7Mdn29ZGya31a";
inline const string REQUEST_TOKEN = "qKLeSUycwFEvWGw";
inline const string API_SECRET = "kqykYBBhGEwPziq";
inline const string ACCESS_TOKEN = "rqykYBfhGEsPziq";
// NOLINTEND(cert-err58-cpp)

class mockKite2 : public kc::kite {
  public:
    mockKite2(): kite(kc::test::API_KEY) {};
    MOCK_METHOD(utils::http::response, sendReq,
        (const utils::http::endpoint& endpoint, const utils::http::Params& body, const utils::FmtArgs& fmtArgs),
        (override));
};

inline string readFile(const string& path) {
    std::ifstream jsonFile(path);
    if (!jsonFile.good()) {
        string fileName;
        try {
            size_t lastOfSlash = path.find_last_of('/');
            if (lastOfSlash == std::string::npos) { throw std::runtime_error(""); };
            fileName = path.substr(lastOfSlash + 1, path.size() - 1);
        } catch (const std::exception& ex) { throw std::runtime_error("invalid test data file path"); }
        throw std::runtime_error(FMT("missing test data file ({0})", fileName));
    }

    std::stringstream buffer;
    buffer << jsonFile.rdbuf();
    return buffer.str();
}
} // namespace kiteconnect::test