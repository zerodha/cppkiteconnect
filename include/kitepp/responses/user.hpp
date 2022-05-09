#pragma once

#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

/// represents a user's profile
struct userProfile {
    userProfile() = default;
    explicit userProfile(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, userID, "user_id");
        kc::rjutils::_getIfExists(val, userName, "user_name");
        kc::rjutils::_getIfExists(val, userShortName, "user_shortname");
        kc::rjutils::_getIfExists(val, avatarURL, "avatar_url");
        kc::rjutils::_getIfExists(val, userType, "user_type");
        kc::rjutils::_getIfExists(val, email, "email");
        kc::rjutils::_getIfExists(val, broker, "broker");
        kc::rjutils::_getIfExists(val, products, "products");
        kc::rjutils::_getIfExists(val, orderTypes, "order_types");
        kc::rjutils::_getIfExists(val, exchanges, "exchanges");

        rj::Value metaVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, metaVal, "meta", kc::rjutils::_RJValueType::Object);
        meta.parse(metaVal.GetObject());
    };

    string userID;
    string userName;
    string userShortName;
    string avatarURL;
    string userType;
    string email;
    string broker;
    std::vector<string> products;
    std::vector<string> orderTypes;
    std::vector<string> exchanges;
    struct Meta {
        Meta() = default;
        explicit Meta(const rj::Value::Object& val) { parse(val); };

        void parse(const rj::Value::Object& val) { kc::rjutils::_getIfExists(val, dematConsent, "demat_consent"); }

        string dematConsent;
    } meta;
};

/// represents tokens received after successful authentication
struct userTokens {
    userTokens() = default;
    explicit userTokens(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, userID, "user_id");
        kc::rjutils::_getIfExists(val, accessToken, "access_token");
        kc::rjutils::_getIfExists(val, refreshToken, "refresh_token");
    };

    string userID;
    string accessToken;
    string refreshToken;
};

/// represents the response after a successful authentication
struct userSession {
    userSession() = default;
    explicit userSession(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        profile.parse(val);
        tokens.parse(val);
        kc::rjutils::_getIfExists(val, apiKey, "api_key");
        kc::rjutils::_getIfExists(val, publicToken, "public_token");
        kc::rjutils::_getIfExists(val, loginTime, "login_time");
    };

    string apiKey;
    string publicToken;
    string loginTime;
    userProfile profile;
    userTokens tokens;
};

/// represents available margins from margins response for a single segment
struct availableMargins {
    availableMargins() = default;
    explicit availableMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, adHocMargin, "adhoc_margin");
        kc::rjutils::_getIfExists(val, cash, "cash");
        kc::rjutils::_getIfExists(val, collateral, "collateral");
        kc::rjutils::_getIfExists(val, intradayPayin, "intraday_payin");
    };

    double adHocMargin = kc::DEFAULTDOUBLE;
    double cash = kc::DEFAULTDOUBLE;
    double collateral = kc::DEFAULTDOUBLE;
    double intradayPayin = kc::DEFAULTDOUBLE;
};

/// represents used margins from margins response for a single segment
struct usedMargins {
    usedMargins() = default;
    explicit usedMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, debits, "debits");
        kc::rjutils::_getIfExists(val, exposure, "exposure");
        kc::rjutils::_getIfExists(val, M2MRealised, "m2m_realised");
        kc::rjutils::_getIfExists(val, M2MUnrealised, "m2m_unrealised");
        kc::rjutils::_getIfExists(val, optionPremium, "option_premium");
        kc::rjutils::_getIfExists(val, payout, "payout");
        kc::rjutils::_getIfExists(val, span, "span");
        kc::rjutils::_getIfExists(val, holdingSales, "holding_sales");
        kc::rjutils::_getIfExists(val, turnover, "turnover");
    };

    double debits = kc::DEFAULTDOUBLE;
    double exposure = kc::DEFAULTDOUBLE;
    double M2MRealised = kc::DEFAULTDOUBLE;
    double M2MUnrealised = kc::DEFAULTDOUBLE;
    double optionPremium = kc::DEFAULTDOUBLE;
    double payout = kc::DEFAULTDOUBLE;
    double span = kc::DEFAULTDOUBLE;
    double holdingSales = kc::DEFAULTDOUBLE;
    double turnover = kc::DEFAULTDOUBLE;
};

/// represents user margins for a segment
struct margins {
    margins() = default;
    explicit margins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, enabled, "enabled");
        kc::rjutils::_getIfExists(val, net, "net");
        rj::Value avlVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, avlVal, "available", kc::rjutils::_RJValueType::Object);
        rj::Value usedVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, usedVal, "utilised", kc::rjutils::_RJValueType::Object);
        available.parse(avlVal.GetObject());
        used.parse(usedVal.GetObject());
    };

    double net = kc::DEFAULTDOUBLE;
    bool enabled = false;
    availableMargins available;
    usedMargins used;
};

/// represents all margins (contains both equity and commodity margins)
struct allMargins {
    allMargins() = default;
    explicit allMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        rj::Value eqVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, eqVal, "equity", kc::rjutils::_RJValueType::Object);
        equity.parse(eqVal.GetObject());
        rj::Value cmVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, cmVal, "commodity", kc::rjutils::_RJValueType::Object);
        commodity.parse(cmVal.GetObject());
    };

    margins equity;
    margins commodity;
};

} // namespace kiteconnect