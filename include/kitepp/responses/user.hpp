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

#include "../utils.hpp"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/// Represents an user's profile.
struct userProfile {
    userProfile() = default;
    explicit userProfile(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        userID = utils::json::get<string>(val, "user_id");
        userName = utils::json::get<string>(val, "user_name");
        userShortName = utils::json::get<string>(val, "user_shortname");
        avatarURL = utils::json::get<string>(val, "avatar_url");
        userType = utils::json::get<string>(val, "user_type");
        email = utils::json::get<string>(val, "email");
        broker = utils::json::get<string>(val, "broker");
        products = utils::json::get<std::vector<string>>(val, "products");
        orderTypes = utils::json::get<std::vector<string>>(val, "order_types");
        exchanges = utils::json::get<std::vector<string>>(val, "exchanges");

        meta = utils::json::get<utils::json::JsonObject, Meta>(val, "meta");
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

        void parse(const rj::Value::Object& val) {
            dematConsent = utils::json::get<string>(val, "demat_consent");
        }

        string dematConsent;
    } meta;
};

/// Represents tokens received after successful authentication.
struct userTokens {
    userTokens() = default;
    explicit userTokens(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        userID = utils::json::get<string>(val, "user_id");
        accessToken = utils::json::get<string>(val, "access_token");
        refreshToken = utils::json::get<string>(val, "refresh_token");
    };

    string userID;
    string accessToken;
    string refreshToken;
};

/// Represents information of a user session.
struct userSession {
    userSession() = default;
    explicit userSession(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        profile.parse(val);
        tokens.parse(val);
        apiKey = utils::json::get<string>(val, "api_key");
        publicToken = utils::json::get<string>(val, "public_token");
        loginTime = utils::json::get<string>(val, "login_time");
    };

    string apiKey;
    string publicToken;
    string loginTime;
    userProfile profile;
    userTokens tokens;
};

/// Represents available margins of a single segment.
struct availableMargins {
    availableMargins() = default;
    explicit availableMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        adHocMargin = utils::json::get<double>(val, "adhoc_margin");
        cash = utils::json::get<double>(val, "cash");
        collateral = utils::json::get<double>(val, "collateral");
        intradayPayin = utils::json::get<double>(val, "intraday_payin");
    };

    double adHocMargin = -1;
    double cash = -1;
    double collateral = -1;
    double intradayPayin = -1;
};

/// Represents used margins of a single segment.
struct usedMargins {
    usedMargins() = default;
    explicit usedMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        debits = utils::json::get<double>(val, "debits");
        exposure = utils::json::get<double>(val, "exposure");
        M2MRealised = utils::json::get<double>(val, "m2m_realised");
        M2MUnrealised = utils::json::get<double>(val, "m2m_unrealised");
        optionPremium = utils::json::get<double>(val, "option_premium");
        payout = utils::json::get<double>(val, "payout");
        span = utils::json::get<double>(val, "span");
        holdingSales = utils::json::get<double>(val, "holding_sales");
        turnover = utils::json::get<double>(val, "turnover");
    };

    double debits = -1;
    double exposure = -1;
    double M2MRealised = -1;
    double M2MUnrealised = -1;
    double optionPremium = -1;
    double payout = -1;
    double span = -1;
    double holdingSales = -1;
    double turnover = -1;
};

/// Represents user margins for a segment.
struct margins {
    margins() = default;
    explicit margins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        enabled = utils::json::get<bool>(val, "enabled");
        net = utils::json::get<double>(val, "net");
        available = utils::json::get<utils::json::JsonObject, availableMargins>(
            val, "available");
        used = utils::json::get<utils::json::JsonObject, usedMargins>(
            val, "utilised");
    };

    double net = -1;
    bool enabled = false;
    availableMargins available;
    usedMargins used;
};

/// Represents all margins (equity and commodity).
struct allMargins {
    allMargins() = default;
    explicit allMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        equity =
            utils::json::get<utils::json::JsonObject, margins>(val, "equity");
        commodity = utils::json::get<utils::json::JsonObject, margins>(
            val, "commodity");
    };

    margins equity;
    margins commodity;
};

} // namespace kiteconnect