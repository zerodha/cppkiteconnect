#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "kitepp/kiteppexceptions.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace kitepp {

namespace RJHelper {

using std::string;
namespace rj = rapidjson;

enum class RJValueType : int { Object, Array };

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

        // in case returned number doesn't have decimal point. Directly calling GetDouble() will cause error if number doesn't have decimal
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

inline bool getIfExists(const rj::Value::Object& val, std::vector<double>& out, const char* name) {

    auto it = val.FindMember(name);
    if (it != val.MemberEnd()) {

        if (it->value.IsArray()) {

            for (const auto& v : it->value.GetArray()) {

                if (v.IsDouble()) {

                    out.emplace_back(v.GetDouble());
                    return true;
                };
                if (v.IsInt()) {

                    out.emplace_back(v.GetInt());
                    return true;
                };
                throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an array of doubles)", name));
            };
        };
        throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an array of double)", name));
    };

    return false;
};

inline bool getIfExists(const rj::Value::Object& val, rj::Value& out, const char* name, RJValueType type) {

    auto it = val.FindMember(name);

    if (type == RJValueType::Object) {

        if (it != val.MemberEnd()) {

            if (it->value.IsObject()) {
                out = it->value.GetObject();
                return true;
            };

            throw libException(FMT("Expected value({0})'s type wasn't the one expected (expected an Object)", name));
        };

        return false;

    } else if (type == RJValueType::Array) {

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

inline string dump(rj::Document& dom) {

    rj::StringBuffer buffer;
    rj::Writer<rj::StringBuffer> writer(buffer);
    dom.Accept(writer);

    return buffer.GetString();
}

} // namespace RJHelper

} // namespace kitepp