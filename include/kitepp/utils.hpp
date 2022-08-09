/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config.hpp"
#include "cpp-httplib/httplib.h"
#include "kiteppexceptions.hpp"
#include "rjutils.hpp"

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GENERATE_FLUENT_METHOD(returnType, fieldType, fieldName, methodName)                                           \
    returnType& methodName(fieldType arg) {                                                                            \
        (fieldName) = arg;                                                                                             \
        return *this;                                                                                                  \
    };

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

inline std::vector<string> _split(const std::string& text, char sep) {

    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.emplace_back(text.substr(start, end - start));

        start = end + 1;
    };

    tokens.emplace_back(text.substr(start));

    return tokens;
};

// ! std::isnan should be used but doing so breaks the compilation on Windows
// ! utilizes a IEEE 754 property that states NaN != NaN, can fail because of compiler optimizations
// NOLINTNEXTLINE(clang-diagnostic-tautological-compare, misc-redundant-expression)
inline bool isValidArg(int value) { return value == value; };
// NOLINTNEXTLINE(clang-diagnostic-tautological-compare, misc-redundant-expression)
inline bool isValidArg(double value) { return value == value; };
inline bool isValidArg(const string& value) { return !value.empty(); };

} // namespace kiteconnect

namespace kiteconnect::internal::utils {
namespace json {
enum class PARSE_AS : uint8_t
{
    OBJECT,
    ARRAY
};

template <class resT, json::PARSE_AS parseAs>
resT parse(rj::Document& doc) {
    if constexpr (parseAs == json::PARSE_AS::OBJECT) {
        if (!doc.IsObject()) { throw libException("invalid object"); };
        return resT(doc["data"].GetObject());
    } else if constexpr (parseAs == json::PARSE_AS::ARRAY) {
        if (!doc.IsArray()) { throw libException("invalid array"); };
        return resT(doc["data"].GetArray());
    }
}
} // namespace json

namespace http {
using paramsT = httplib::Params;

enum class METHOD : uint8_t
{
    GET,
    POST,
    PUT,
    DEL,
    HEAD
};
enum class CONTENT_TYPE : uint8_t
{
    JSON,
    NON_JSON
};
enum class CODE : uint16_t
{ OK = 200 };

class response {
  public:
    response(uint16_t Code, const string& Json): code(Code) { parse(Code, Json); };

    explicit operator bool() const { return !error; };

    uint16_t code = 0;
    bool error = false;
    rjutils::rj::Document data;
    string errorType = "NoException";
    string message;

  private:
    void parse(uint16_t code, const string& json) {
        kc::rjutils::_parse(data, json);
        if (code != static_cast<uint16_t>(CODE::OK)) {
            string status;
            kc::rjutils::_getIfExists(data, status, "status");
            kc::rjutils::_getIfExists(data, errorType, "error_type");
            kc::rjutils::_getIfExists(data, message, "message");
            if (status != "success") { error = true; };
        };
    };
};

struct request {
    utils::http::METHOD method;
    string path;
    string authToken;
    paramsT params;
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON;
    string serializedBody;

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    response send(httplib::Client& client) const {
        const httplib::Headers headers = { { "Authorization", authToken } };
        uint16_t code = 0;
        string data;

        // httplib::Result doesn't have a default constructor and using a pointer causes segfault
        switch (method) {
            case utils::http::METHOD::GET:
                if (auto res = client.Get(path.c_str(), headers)) {
                    code = res->status;
                    data = res->body;
                } else {
                    // TODO convert enum to string
                    throw libException(FMT("request failed ({0})", res.error()));
                }
                break;
            case utils::http::METHOD::POST:
                if (contentType != CONTENT_TYPE::JSON) {
                    if (auto res = client.Post(path.c_str(), headers, params)) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(FMT("request failed ({0})", res.error()));
                    }
                } else {
                    if (auto res = client.Post(path.c_str(), headers, serializedBody, "application/json")) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(FMT("request failed({0})", res.error()));
                    }
                };
                break;
            case utils::http::METHOD::PUT:
                if (contentType != CONTENT_TYPE::JSON) {
                    if (auto res = client.Put(path.c_str(), headers, params)) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(FMT("request failed ({0})", res.error()));
                    }
                } else {
                    if (auto res = client.Put(path.c_str(), headers, serializedBody, "application/json")) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(FMT("request failed({0})", res.error()));
                    }
                }
                break;
            case utils::http::METHOD::DEL:
                if (auto res = client.Delete(path.c_str(), headers)) {
                    code = res->status;
                    data = res->body;
                } else {
                    throw libException(FMT("request failed ({0})", res.error()));
                }
                break;
            default: throw libException("unsupported http method");
        };

        return { code, data };
    };
};
} // namespace http
} // namespace kiteconnect::internal::utils