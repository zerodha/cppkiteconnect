#pragma once

#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "../utils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

/// represents a position from the margin calculation API
struct orderMarginsParams {
    orderMarginsParams() = default;

    GENERATE_FLUENT_METHOD(orderMarginsParams, double, quantity, Quantity);
    GENERATE_FLUENT_METHOD(orderMarginsParams, double, price, Price);
    GENERATE_FLUENT_METHOD(orderMarginsParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, tradingsymbol, Tradingsymbol);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(orderMarginsParams, const string&, orderType, OrderType);

    double quantity = 0;
    double price = 0;
    double triggerPrice = 0;
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

    double span = -1;
    double exposure = -1;
    double optionPremium = -1;
    double additional = -1;
    double bo = -1;
    double cash = -1;
    double var = -1;
    double total = -1;
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

        double realised = -1;
        double unrealised = -1;
    } pnl;
};

} // namespace kiteconnect