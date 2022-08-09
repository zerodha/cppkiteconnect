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

// this file has helper functions for rapidjson library

#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "kitepp/kiteppexceptions.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace kiteconnect {

namespace rjutils {

using std::string;
namespace rj = rapidjson;

enum class _RJValueType : int { Object, Array };

inline bool _parse(rj::Document& dom, const string& str) {

    rj::ParseResult parseOK = dom.Parse(str.c_str());
    if (parseOK == nullptr) { throw libException(FMT("Failed to parse json string: {0}", str)); };

    return true;
};

// TODO use templates
inline bool _getIfExists(const rj::Value::Object& val, string& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsString()) {
            out = it->value.GetString();
            return true;
        };

        if (it->value.IsNull()) {
            out = "";
            return true;
        };

        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected a string)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Document& val, string& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsString()) {
            out = it->value.GetString();
            return true;
        };

        if (it->value.IsNull()) {
            out = "";
            return true;
        };

        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected a string)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Value::Object& val, double& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsDouble()) {
            out = it->value.GetDouble();
            return true;
        };

        // in case returned number doesn't have decimal point. Directly calling GetDouble() will cause error if number
        // doesn't have decimal
        if (it->value.IsInt()) {
            out = it->value.GetInt(); //! may lead to precision loss
            return true;
        };

        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected a double)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Value::Object& val, int& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsInt()) {
            out = it->value.GetInt();
            return true;
        };

        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected a int)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Value::Object& val, bool& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsBool()) {
            out = it->value.GetBool();
            return true;
        };
        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected a bool)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Value::Object& val, std::vector<string>& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsArray()) {
            for (const auto& v : it->value.GetArray()) {
                (v.IsString()) ?
                    out.emplace_back(v.GetString()) :
                    throw libException(
                        FMT("Expected value({0})'s type wasn't the one expected (expected an array of strings)", name));
            };
            return true;
        };
        throw libException(
            FMT("Expected value({0})'s type wasn't the one expected (expected an array of strings)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Value::Object& val, std::vector<double>& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {
        if (it->value.IsArray()) {
            for (const auto& v : it->value.GetArray()) {
                if (v.IsDouble()) {
                    out.emplace_back(v.GetDouble());
                    continue;
                };
                if (v.IsInt()) {
                    out.emplace_back(v.GetInt());
                    continue;
                };
                throw libException(
                    FMT("Expected value({0})'s type wasn't the one expected (expected an array of doubles)", name));
            };

            return true;
        };
        throw libException(
            FMT("Expected value({0})'s type wasn't the one expected (expected an array of double)", name));
    };

    return false;
};

inline bool _getIfExists(const rj::Value::Object& val, rj::Value& out, const char* name, _RJValueType type) {

    auto it = val.FindMember(name);

    if (type == _RJValueType::Object) {
        if (it != val.MemberEnd()) {
            if (it->value.IsObject()) {
                out = it->value.GetObject();
                return true;
            };

            throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an Object)", name));
        };

        return false;
    } else if (type == _RJValueType::Array) {
        if (it != val.MemberEnd()) {
            if (it->value.IsArray()) {
                out = it->value.GetArray();
                return true;
            };

            throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an Array)", name));
        };

        return false;
    };

    return false;
};

inline string _dump(rj::Document& dom) {

    rj::StringBuffer buffer;
    rj::Writer<rj::StringBuffer> writer(buffer);
    dom.Accept(writer);

    return buffer.GetString();
}

} // namespace rjutils

} // namespace kiteconnect
