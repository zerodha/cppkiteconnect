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

/// represents a position from the margin calculation API
struct orderMarginsParams {
    orderMarginsParams() = default;

    double quantity = kc::DEFAULTDOUBLE;
    double price = kc::DEFAULTDOUBLE;
    double triggerPrice = kc::DEFAULTDOUBLE;
    string exchange;
    string tradingsymbol;
    string transactionType;
    string variety;
    string product;
    string orderType;
};

/// represents response from the margin calculation API
struct orderMargins {
    orderMargins() = default;
    explicit orderMargins(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, type, "type");
        kc::rjutils::_getIfExists(val, tradingSymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, span, "span");
        kc::rjutils::_getIfExists(val, exposure, "exposure");
        kc::rjutils::_getIfExists(val, optionPremium, "option_premium");
        kc::rjutils::_getIfExists(val, additional, "additional");
        kc::rjutils::_getIfExists(val, bo, "bo");
        kc::rjutils::_getIfExists(val, cash, "cash");
        kc::rjutils::_getIfExists(val, var, "var");

        rj::Value pnlVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, pnlVal, "pnl", kc::rjutils::_RJValueType::Object);
        pnl.parse(pnlVal.GetObject());

        kc::rjutils::_getIfExists(val, total, "total");
    };

    double span = kc::DEFAULTDOUBLE;
    double exposure = kc::DEFAULTDOUBLE;
    double optionPremium = kc::DEFAULTDOUBLE;
    double additional = kc::DEFAULTDOUBLE;
    double bo = kc::DEFAULTDOUBLE;
    double cash = kc::DEFAULTDOUBLE;
    double var = kc::DEFAULTDOUBLE;
    double total = kc::DEFAULTDOUBLE;
    string type;
    string tradingSymbol;
    string exchange;
    struct PNL {
        PNL() = default;
        explicit PNL(const rj::Value::Object& val) { parse(val); };

        void parse(const rj::Value::Object& val) {
            kc::rjutils::_getIfExists(val, realised, "realised");
            kc::rjutils::_getIfExists(val, unrealised, "unrealised");
        };

        double realised = kc::DEFAULTDOUBLE;
        double unrealised = kc::DEFAULTDOUBLE;
    } pnl;
};

} // namespace kiteconnect