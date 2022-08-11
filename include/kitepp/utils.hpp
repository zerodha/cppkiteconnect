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

/// @brief indicates whether a response should be parsed as an object or as an array
enum class PARSE_AS : uint8_t
{
    OBJECT,
    ARRAY
};

/**
 * @brief parses a rapidjson document into \a resT
 *
 * @tparam resT    the type document should be parsed into.
 *                 \a resT must have a constructor that accepts \a rapidjson::Document as
 *                 the sole argument and proceeds to parse it.
 * @tparam parseAs \a document's underlying data type

 * @param doc rapidjson document to parse

 * @return resT parsed object
 */
template <class resT, json::PARSE_AS parseAs>
resT parse(rj::Document& doc) {
    static_assert(std::is_constructible<resT, const rj::Value::Object&> {} ||
                  std::is_constructible<resT, const rj::Value::Array&> {});

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

/// @brief represents http status codes
namespace code {
constexpr uint16_t OK = 200;
} // namespace code

/// @brief represents http methods
enum class METHOD : uint8_t
{
    GET,
    POST,
    PUT,
    DEL,
    HEAD
};

/// @brief represents http content type header values
enum class CONTENT_TYPE : uint8_t
{
    JSON,
    NON_JSON
};

/// @brief represents a REST endpoint
struct endpoint {
    METHOD method;                                     /// http method
    string path;                                       /// endpoint path
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON; /// content type expected

    /**
     * @brief compares two \a endpoint objects
     *
     * @param lhs \a endpoint to compare
     * @return true if the objects are equal
     * @return false if the objects are not equal
     */
    bool operator==(const endpoint& lhs) const {
        return lhs.method == this->method && lhs.path == this->path && lhs.contentType == this->contentType;
    }
};

/// @brief represents a http response
class response {
  public:
    /**
     * @brief construct a new \a response object
     *
     * @param Code status code
     * @param Json json body
     */
    response(uint16_t Code, const string& Json): code(Code) { parse(Code, Json); };

    explicit operator bool() const { return !error; };

    uint16_t code = 0;                /// http code
    bool error = false;               /// true if kite api reported an error (\a status field)
    rjutils::rj::Document data;       /// parsed body
    string errorType = "NoException"; /// corresponds to kite api's \a error_type field (if \a error is \a true)
    string message;                   /// corresponds to kite api's \a message field (if \a error is \a true)

  private:
    /**
     * @brief parse a json response
     *
     * @param code status code
     * @param json json body
     */
    void parse(uint16_t code, const string& json) {
        kc::rjutils::_parse(data, json);
        if (code != static_cast<uint16_t>(code::OK)) {
            string status;
            kc::rjutils::_getIfExists(data, status, "status");
            kc::rjutils::_getIfExists(data, errorType, "error_type");
            kc::rjutils::_getIfExists(data, message, "message");
            if (status != "success") { error = true; };
        };
    };
};

/// @brief represents a http request
struct request {

    /**
     * @brief send a \a request
     *
     * @param client http client
     *
     * @return response response
     */
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
                    if (auto res = client.Post(path.c_str(), headers, body)) {
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
                    if (auto res = client.Put(path.c_str(), headers, body)) {
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

    utils::http::METHOD method;                        /// http method
    string path;                                       /// request path
    string authToken;                                  /// \a Authorization header string
    paramsT body;                                      /// request body (sent as form encoded)
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON; /// content type
    string serializedBody;                             /// serialized body (if sending data as json)
};
} // namespace http
} // namespace kiteconnect::internal::utils