#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "kitepp/kiteppexceptions.hpp"
#include "rapidjson/document.h"

namespace kitepp {

namespace RJHelper {

using std::string;
namespace rj = rapidjson;

inline bool parse(rj::Document& dom, const string& str) {

    rj::ParseResult parseOK = dom.Parse(str.c_str());
    if (parseOK == nullptr) { throw libException(FMT("Failed to parse json string: {0}", str)); };

    return true;
};

inline bool getIfExists(const rj::Value::Object& val, string& out, const char* name) {

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

inline bool getIfExists(const rj::Value::Object& val, double& out, const char* name) {


    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {

        if (it->value.IsDouble()) {

            out = it->value.GetDouble();
            return true;
        };

        if (it->value.IsInt()) {

            out = it->value.GetInt(); //! may lead to precision loss
            return true;
        };

        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected a double)", name));
    };

    return false;
};

inline bool getIfExists(const rj::Value::Object& val, int& out, const char* name) {

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

inline bool getIfExists(const rj::Value::Object& val, bool& out, const char* name) {

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

inline bool getIfExists(const rj::Value::Object& val, std::vector<string>& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {

        if (it->value.IsArray()) {

            for (const auto& v : it->value.GetArray()) {

                (v.IsString()) ? out.emplace_back(v.GetString()) :
                                 throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an array of strings)", name));
            };
            return true;
        };
        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an array of strings)", name));
    };

    return false;
};

inline bool getIfExists(const rj::Value::Object& val, rj::Value& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {

        if (it->value.IsObject()) {
            out = it->value.GetObject();
            return true;
        };

        throw libException(FMT("Expected value({0}) not found or it's type wasn't the one expected (expected an Object)", name));
    };
    return false;
};


} // namespace RJHelper

} // namespace kitepp