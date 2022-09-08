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
#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "config.hpp"
#include "cpp-httplib/httplib.h"
#include "kiteppexceptions.hpp"
#include "rapidjson/encodings.h"
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
//! init_list doesn't have compare operator & gmock needs it
using FmtArgs = std::vector<string>;

namespace json {

using JsonObject = rj::GenericValue<rj::UTF8<>>::Object;
using JsonArray = rj::GenericValue<rj::UTF8<>>::Array;
template <class Res>
using CustomObjectParser = std::function<Res(JsonObject&)>;
template <class Res>
using CustomArrayParser = std::function<Res(JsonArray&)>;
template <class Res, class Data, bool UseCustomParser>
using CustomParser =
    std::conditional_t<std::is_same_v<Data, JsonObject>, const CustomObjectParser<Res>&, const CustomArrayParser<Res>&>;

inline JsonObject extractObject(rj::Document& doc) {
    try {
        return doc["data"].GetObject();
    } catch (const std::exception& ex) { throw libException("invalid body"); }
}

inline JsonArray extractArray(rj::Document& doc) {
    try {
        return doc["data"].GetArray();
    } catch (const std::exception& ex) { throw libException("invalid body"); }
}

template <class Res, class Data, bool UseCustomParser>
Res parse(rj::Document& doc, CustomParser<Res, Data, UseCustomParser> customParser) {
    if constexpr (std::is_same_v<Data, JsonObject>) {
        auto object = extractObject(doc);
        if constexpr (UseCustomParser) {
            return customParser(object);
        } else {
            static_assert(std::is_constructible_v<Res, JsonObject>, "Res should be constructable using JsonObject");
            return Res(object);
        }
    } else if constexpr (std::is_same_v<Data, JsonArray>) {
        auto array = extractArray(doc);
        if constexpr (UseCustomParser) {
            return customParser(array);
        } else {
            static_assert(std::is_constructible_v<Res, JsonArray>, "Res should be constructable using JsonArray");
            return Res(array);
        }
    }
}
} // namespace json

namespace http {

using Params = httplib::Params;

namespace code {
constexpr uint16_t OK = 200;
} // namespace code

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

struct endpoint {
    bool operator==(const endpoint& lhs) const {
        return lhs.method == this->method && lhs.Path.Path == this->Path.Path && lhs.contentType == this->contentType;
    }

    METHOD method = METHOD::GET;
    struct path {

        string operator()(const FmtArgs& fmtArgs = {}) const {
            if (fmtArgs.empty()) {
                fmt::dynamic_format_arg_store<fmt::format_context> store;
                for (const auto& arg : fmtArgs) { store.push_back(arg); };
                return fmt::vformat(Path, store);
            };
            return Path;
        };

        string Path;
    } Path;
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON;
};

class response {
  public:
    response(uint16_t Code, const string& Json): code(Code) { parse(Code, Json); };

    explicit operator bool() const { return !error; };

    uint16_t code = 0;                /// http code
    bool error = false;               /// true if kite api reported an error (\a status field)
    rjutils::rj::Document data;       /// parsed body
    string errorType = "NoException"; /// corresponds to kite api's \a error_type field (if \a error is \a true)
    string message;                   /// corresponds to kite api's \a message field (if \a error is \a true)

  private:
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

struct request {

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

    utils::http::METHOD method;
    string path;
    string authToken;
    Params body;
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON;
    string serializedBody;
};
} // namespace http

template <typename>
struct isOptional : std::false_type {};
template <typename T>
struct isOptional<std::optional<T>> : std::true_type {};

template <class Param>
void addParam(http::Params& bodyParams, Param& param, const string& fieldName) {
    static_assert(isOptional<std::decay_t<Param>>::value, "Param must be std::optional");
    if (param.has_value()) {
        string fieldValue;
        if constexpr (!std::is_same_v<typename Param::value_type, string>) {
            fieldValue = std::to_string(param.value());
        } else {
            fieldValue = param.value();
        }
        if (param.has_value()) { bodyParams.emplace(fieldName, fieldValue); }
    }
};

} // namespace kiteconnect::internal::utils