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

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "exceptions.hpp"

#include "cpp-httplib/httplib.h"
#define FMT_HEADER_ONLY 1
#include "fmt/include/fmt/format.h"
#include "rapidcsv/src/rapidcsv.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/encodings.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
#include "rapidjson/include/rapidjson/writer.h"

// Check endieness of platform
#if defined(_WIN32)
// Do nothing (Assuming all modern Windows machines are little endian)
#else // Windows check
#ifdef __BIG_ENDIAN__
#define WORDS_BIGENDIAN 1
#else /* __BIG_ENDIAN__ */
#ifdef __LITTLE_ENDIAN__
#undef WORDS_BIGENDIAN
#else
#ifdef BSD
#include <sys/endian.h>
#else
#include <endian.h>
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#undef WORDS_BIGENDIAN
#else
#error "unable to determine endianess!"
#endif /* __BYTE_ORDER */
#endif /* __LITTLE_ENDIAN__ */
#endif /* __BIG_ENDIAN__ */
#endif // Windows check

// NOLINTNEXTLINE(google-global-names-in-headers, misc-unused-using-decls)
using fmt::literals::operator""_a;
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FMT fmt::format

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GENERATE_FLUENT_METHOD(returnType, fieldType, fieldName, methodName)   \
    returnType& methodName(fieldType arg) {                                    \
        (fieldName) = arg;                                                     \
        return *this;                                                          \
    };

namespace kiteconnect::internal::utils {
using std::string;
namespace rj = rapidjson;

//! init_list doesn't have compare operator & gmock needs it
using FmtArgs = std::vector<string>;
template <typename>
struct isOptional : std::false_type {};
template <typename T>
struct isOptional<std::optional<T>> : std::true_type {};

template <typename>
struct isVector : std::false_type {};
template <typename T>
struct isVector<std::vector<T>> : std::true_type {};

constexpr uint16_t MILLISECONDS_IN_A_SECOND = 1000;

namespace json {

using JsonObject = rj::GenericValue<rj::UTF8<>>::Object;
using JsonArray = rj::GenericValue<rj::UTF8<>>::Array;
template <class Res>
using CustomObjectParser = std::function<Res(JsonObject&)>;
template <class Res>
using CustomArrayParser = std::function<Res(JsonArray&)>;
template <class Res, class Data, bool UseCustomParser>
using CustomParser = std::conditional_t<std::is_same_v<Data, JsonObject>,
    const CustomObjectParser<Res>&, const CustomArrayParser<Res>&>;
template <class T>
using JsonEncoder = std::function<void(const T&, rj::Value&)>;

// FIXME templatize extract* methods
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

inline bool extractBool(rj::Document& doc) {
    try {
        return doc["data"].GetBool();
    } catch (const std::exception& ex) { throw libException("invalid body"); }
}

inline string extractString(rj::Document& doc) {
    try {
        return doc["data"].GetString();
    } catch (const std::exception& ex) { throw libException("invalid body"); }
}

template <class Output, class Document = rj::Value::Object>
// NOLINTNEXTLINE(readability-function-cognitive-complexity)
inline Output get(const Document& val, const char* name) {
    static const auto exceptionString = [&name](const string& type) {
        return FMT("type of {0} not is not {1}", name, type);
    };

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if constexpr (!isVector<Output>::value) {
            if constexpr (std::is_same_v<std::decay_t<Output>, string>) {
                if (it->value.IsString()) { return it->value.GetString(); };
                if (it->value.IsNull()) { return ""; };
                throw libException(exceptionString("string"));
            } else if constexpr (std::is_same_v<std::decay_t<Output>, double>) {
                if (it->value.IsDouble()) { return it->value.GetDouble(); };
                // if the sent value doesn't have a floating point (this time),
                // GetDouble() will throw error
                if (it->value.IsInt()) { return it->value.GetInt(); };
                throw libException(exceptionString("double"));
            } else if constexpr (std::is_same_v<std::decay_t<Output>, int>) {
                if (it->value.IsInt()) { return it->value.GetInt(); };
                throw libException(exceptionString("int"));
            } else if constexpr (std::is_same_v<std::decay_t<Output>, bool>) {
                if (it->value.IsBool()) { return it->value.GetBool(); };
                throw libException(exceptionString("bool"));
            } else {
                throw libException("type not supported");
            }
        } else {
            if (it->value.IsArray()) {
                Output out;
                for (const auto& v : it->value.GetArray()) {
                    if constexpr (std::is_same_v<
                                      std::decay_t<typename Output::value_type>,
                                      string>) {
                        (v.IsString()) ?
                            out.emplace_back(v.GetString()) :
                            throw libException(exceptionString("string"));
                    } else if constexpr (std::is_same_v<
                                             std::decay_t<
                                                 typename Output::value_type>,
                                             double>) {
                        if (v.IsDouble()) {
                            out.emplace_back(v.GetDouble());
                            continue;
                        };
                        // if the sent value doesn't have a floating point (this
                        // time), GetDouble() will throw error
                        if (v.IsInt()) {
                            out.emplace_back(v.GetInt());
                            continue;
                        };
                        throw libException(exceptionString("array of doubles"));
                    } else {
                        throw libException("type not supported");
                    }
                }
                return out;
            };
            throw libException(exceptionString("array"));
        }
    } else {
        return {};
    }
};

template <class Val, class Output>
Output get(const rj::Value::Object& val, const char* name) {
    static const auto exceptionString = [&name](const string& type) {
        return FMT("type of {0} not is not {1}", name, type);
    };

    auto it = val.FindMember(name);
    if constexpr (std::is_same_v<Val, JsonObject>) {
        static_assert(std::is_constructible_v<Output, rj::Value::Object>);
        rj::Value out(rj::kObjectType);
        if (it != val.MemberEnd()) {
            if (it->value.IsObject()) { return Output(it->value.GetObject()); };
            throw libException(exceptionString("object"));
        };
        return {};
    } else if constexpr (std::is_same_v<Val, JsonArray>) {
        static_assert(std::is_constructible_v<Output, rj::Value::Array>);
        rj::Value out(rj::kArrayType);
        if (it != val.MemberEnd()) {
            if (it->value.IsArray()) { return Output(it->value.GetArray()); };
            throw libException(exceptionString("array"));
        };
        return {};
    };
    return {};
};

template <class Val>
bool get(const rj::Value::Object& val, rj::Value& out, const char* name) {
    static const auto exceptionString = [&name](const string& type) {
        return FMT("type of {0} not is not {1}", name, type);
    };

    auto it = val.FindMember(name);
    if constexpr (std::is_same_v<Val, JsonObject>) {
        if (it != val.MemberEnd()) {
            if (it->value.IsObject()) {
                out = it->value.GetObject();
                return true;
            };
            throw libException(exceptionString("object"));
        };
        return false;
    } else if constexpr (std::is_same_v<Val, JsonArray>) {
        if (it != val.MemberEnd()) {
            if (it->value.IsArray()) {
                out = it->value.GetArray();
                return true;
            };
            throw libException(exceptionString("array"));
        };
        return false;
    };
    return false;
};

template <class Res, class Data, bool UseCustomParser>
Res parse(
    rj::Document& doc, CustomParser<Res, Data, UseCustomParser> customParser) {
    if constexpr (std::is_same_v<Data, JsonObject>) {
        auto object = extractObject(doc);
        if constexpr (UseCustomParser) {
            return customParser(object);
        } else {
            static_assert(std::is_constructible_v<Res, JsonObject>,
                "Res should be constructable using JsonObject");
            return Res(object);
        }
    } else if constexpr (std::is_same_v<Data, JsonArray>) {
        auto array = extractArray(doc);
        if constexpr (UseCustomParser) {
            return customParser(array);
        } else {
            static_assert(std::is_constructible_v<Res, JsonArray>,
                "Res should be constructable using JsonArray");
            return Res(array);
        }
    } else if constexpr (std::is_same_v<Data, bool>) {
        static_assert(
            std::is_same_v<Res, bool>, "Res needs to be bool if Data is bool");
        return extractBool(doc);
    }
}

inline bool parse(rj::Document& dom, const string& str) {
    rj::ParseResult result = dom.Parse(str.c_str());
    if (result == nullptr) {
        throw libException(FMT("failed to parse json string: {0}", str));
    };
    return true;
};

inline string serialize(rj::Document& dom) {
    rj::StringBuffer buffer;
    rj::Writer<rj::StringBuffer> writer(buffer);
    dom.Accept(writer);
    return buffer.GetString();
}

template <class T>
class json {
  public:
    json() {
        if constexpr (std::is_same_v<T, JsonObject>) {
            dom.SetObject();
        } else {
            dom.StartArray();
        }
    };

    template <class Value>
    void field(const string& name, const Value& value,
        rj::Value* docOverride = nullptr) {
        auto& allocater = dom.GetAllocator();

        if constexpr (std::is_same_v<std::decay_t<Value>, string>) {
            buffer.SetString(value.c_str(), value.size(), allocater);
        } else if constexpr (std::is_integral_v<std::decay_t<Value>>) {
            buffer.SetInt64(value);
        } else if constexpr (std::is_floating_point_v<std::decay_t<Value>>) {
            buffer.SetDouble(value);
        };

        if (docOverride == nullptr) {
            dom.AddMember(
                rj::Value(name.c_str(), allocater).Move(), buffer, allocater);
        } else {
            docOverride->AddMember(
                rj::Value(name.c_str(), allocater).Move(), buffer, allocater);
        }
    }

    template <class Value>
    void field(const string& name, const std::vector<Value>& values,
        const JsonEncoder<Value>& encode = {}) {
        auto& allocater = dom.GetAllocator();
        rj::Value arrayBuffer(rj::kArrayType);
        for (const auto& i : values) {
            if constexpr (std::is_fundamental_v<std::decay_t<Value>>) {
                arrayBuffer.PushBack(i, allocater);
            } else {
                rj::Value objectBuffer(rj::kObjectType);
                encode(i, objectBuffer);
                arrayBuffer.PushBack(objectBuffer, allocater);
            }
        };

        if constexpr (std::is_same_v<T, JsonObject>) {
            dom.AddMember(rj::Value(name.c_str(), dom.GetAllocator()).Move(),
                arrayBuffer, allocater);
        } else {
            dom.Swap(arrayBuffer);
        }
    }

    template <class Value>
    void array(const std::vector<Value>& values,
        const JsonEncoder<Value>& encode = {}) {
        field("", values, encode);
    }

    string serialize() {
        rj::StringBuffer strBuffer;
        rj::Writer<rj::StringBuffer> writer(strBuffer);
        dom.Accept(writer);
        return strBuffer.GetString();
    }

  private:
    rj::Document dom;
    rj::Value buffer;
};
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
        return lhs.method == this->method && lhs.Path.Path == this->Path.Path &&
               lhs.contentType == this->contentType;
    }

    METHOD method = METHOD::GET;
    struct path {

        string operator()(const FmtArgs& fmtArgs = {}) const {
            if (!fmtArgs.empty()) {
                fmt::dynamic_format_arg_store<fmt::format_context> store;
                for (const auto& arg : fmtArgs) { store.push_back(arg); };
                return fmt::vformat(Path, store);
            };
            return Path;
        };

        string Path;
    } Path;
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON;
    CONTENT_TYPE responseType = CONTENT_TYPE::JSON;
};

class response {
  public:
    response(uint16_t Code, const string& body, bool json = true): code(Code) {
        parse(Code, body, json);
    };

    explicit operator bool() const { return !error; };

    uint16_t code = 0;  /// http code
    bool error = false; /// true if kite api reported an error (\a status field)
    rj::Document data;  /// parsed body
    string errorType =
        "NoException"; /// corresponds to kite api's \a error_type field (if \a
                       /// error is \a true)
    string message; /// corresponds to kite api's \a message field (if \a error
                    /// is \a true)
    string rawBody; /// raw body, set in case of non-json response

  private:
    void parse(uint16_t code, const string& body, bool json) {
        if (json) {
            json::parse(data, body);
            if (code != static_cast<uint16_t>(code::OK)) {
                string status;
                status = utils::json::get<string, rj::Document>(data, "status");
                errorType =
                    utils::json::get<string, rj::Document>(data, "error_type");
                message =
                    utils::json::get<string, rj::Document>(data, "message");
                if (status != "success") { error = true; };
            };
        } else {
            if (code != static_cast<uint16_t>(code::OK)) { error = true; };
            rawBody = body;
        }
    };
};

struct request {

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    response send(httplib::Client& client) const {
        const httplib::Headers headers = { { "Authorization", authToken } };
        uint16_t code = 0;
        string data;

        // httplib::Result doesn't have a default constructor and using a
        // pointer causes segfault
        switch (method) {
            case utils::http::METHOD::GET:
                if (auto res = client.Get(path.c_str(), headers)) {
                    code = res->status;
                    data = res->body;
                } else {
                    // TODO convert enum to string
                    throw libException(
                        FMT("request failed ({0})", res.error()));
                }
                break;
            case utils::http::METHOD::POST:
                if (contentType != CONTENT_TYPE::JSON) {
                    if (auto res = client.Post(path.c_str(), headers, body)) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(
                            FMT("request failed ({0})", res.error()));
                    }
                } else {
                    if (auto res = client.Post(path.c_str(), headers,
                            serializedBody, "application/json")) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(
                            FMT("request failed({0})", res.error()));
                    }
                };
                break;
            case utils::http::METHOD::PUT:
                if (contentType != CONTENT_TYPE::JSON) {
                    if (auto res = client.Put(path.c_str(), headers, body)) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(
                            FMT("request failed ({0})", res.error()));
                    }
                } else {
                    if (auto res = client.Put(path.c_str(), headers,
                            serializedBody, "application/json")) {
                        code = res->status;
                        data = res->body;
                    } else {
                        throw libException(
                            FMT("request failed({0})", res.error()));
                    }
                }
                break;
            case utils::http::METHOD::DEL:
                if (auto res = client.Delete(path.c_str(), headers)) {
                    code = res->status;
                    data = res->body;
                } else {
                    throw libException(
                        FMT("request failed ({0})", res.error()));
                }
                break;
            default: throw libException("unsupported http method");
        };

        return { code, data, responseType == CONTENT_TYPE::JSON };
    };

    utils::http::METHOD method;
    string path;
    string authToken;
    Params body;
    CONTENT_TYPE contentType = CONTENT_TYPE::NON_JSON;
    CONTENT_TYPE responseType = CONTENT_TYPE::JSON;
    string serializedBody;
};
} // namespace http

namespace ws::ERROR_CODE {
const unsigned int NORMAL_CLOSURE = 1000;
const unsigned int NO_REASON = 1006;
} // namespace ws::ERROR_CODE

template <class Param>
void addParam(http::Params& bodyParams, Param& param, const string& fieldName) {
    static_assert(
        isOptional<std::decay_t<Param>>::value, "Param must be std::optional");
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

template <class Instrument>
inline std::vector<Instrument> parseInstruments(const std::string& data) {
    static_assert(std::is_constructible_v<Instrument, std::vector<string>>,
        "Instrument must have a constructor that accepts vector of strings");

    std::stringstream sstream(data);
    rapidcsv::Document csv(sstream, rapidcsv::LabelParams(0, -1));
    const size_t numberOfRows = csv.GetRowCount();

    std::vector<Instrument> instruments;
    for (size_t row = 0; row < numberOfRows; row++) {
        instruments.emplace_back(csv.GetRow<string>(row));
    }
    return instruments;
};

} // namespace kiteconnect::internal::utils