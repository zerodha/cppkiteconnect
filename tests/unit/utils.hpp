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

class mockKite : public kc::kite {
  public:
    mockKite(): kite(kc::test::API_KEY) {};
    MOCK_METHOD(utils::http::response, sendReq,
        (const utils::http::endpoint& endpoint, const utils::http::Params& body,
            const utils::FmtArgs& fmtArgs),
        (override));
};

inline string readFile(const string& path) {
    std::ifstream jsonFile(path);
    if (!jsonFile.good()) {
        string fileName;
        try {
            size_t lastOfSlash = path.find_last_of('/');
            if (lastOfSlash == std::string::npos) {
                throw std::runtime_error("");
            };
            fileName = path.substr(lastOfSlash + 1, path.size() - 1);
        } catch (const std::exception& ex) {
            throw std::runtime_error("invalid test data file path");
        }
        throw std::runtime_error(FMT("missing test data file ({0})", fileName));
    }

    std::stringstream buffer;
    buffer << jsonFile.rdbuf();
    return buffer.str();
}
} // namespace kiteconnect::test