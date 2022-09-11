#pragma once

#include <string>

#include "../config.hpp"
#include "../rjutils.hpp"
#include "../utils.hpp"
#include "order.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;

/// represents parameters of a single gtt
struct gttParams {
    GENERATE_FLUENT_METHOD(gttParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(gttParams, double, price, Price);
    GENERATE_FLUENT_METHOD(gttParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(gttParams, const string&, orderType, OrderType);
    GENERATE_FLUENT_METHOD(gttParams, const string&, product, Product);

    int quantity = kc::DEFAULTINT;
    double price = kc::DEFAULTDOUBLE;
    string transactionType;
    string orderType;
    string product;
};

/// represents 'condition' for a single gtt
struct GTTCondition {
    GTTCondition() = default;
    explicit GTTCondition(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, tradingsymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
        kc::rjutils::_getIfExists(val, triggerValues, "trigger_values");
    };

    double lastPrice = kc::DEFAULTDOUBLE;
    string exchange;
    string tradingsymbol;
    std::vector<double> triggerValues;
};

/// represents parameters required for the `placeGtt` function
struct placeGttParams {
    GENERATE_FLUENT_METHOD(placeGttParams, double, lastPrice, LastPrice);
    GENERATE_FLUENT_METHOD(placeGttParams, const string&, triggerType, TriggerType);
    GENERATE_FLUENT_METHOD(placeGttParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(placeGttParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(placeGttParams, const std::vector<double>&, triggerValues, TriggerValues);
    GENERATE_FLUENT_METHOD(placeGttParams, const std::vector<gttParams>&, gttParamsList, GttParamsList);

    double lastPrice = kc::DEFAULTDOUBLE;
    string triggerType;
    string symbol;
    string exchange;
    std::vector<double> triggerValues;
    std::vector<gttParams> gttParamsList;
};

/// represents parameters required for the `modifyGtt` function
struct modifyGttParams {
    GENERATE_FLUENT_METHOD(modifyGttParams, int, triggerId, TriggerId);
    GENERATE_FLUENT_METHOD(modifyGttParams, double, lastPrice, LastPrice);
    GENERATE_FLUENT_METHOD(modifyGttParams, const string&, triggerType, TriggerType);
    GENERATE_FLUENT_METHOD(modifyGttParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(modifyGttParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(modifyGttParams, const std::vector<double>&, triggerValues, TriggerValues);
    GENERATE_FLUENT_METHOD(modifyGttParams, const std::vector<gttParams>&, gttParamsList, GttParamsList);

    int triggerId;
    double lastPrice;
    string triggerType;
    string symbol;
    string exchange;
    std::vector<double> triggerValues;
    std::vector<gttParams> gttParamsList;
};

/// GTT represents a single GTT order.
struct GTT {
    GTT() = default;
    explicit GTT(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, ID, "id");
        kc::rjutils::_getIfExists(val, userID, "user_id");
        kc::rjutils::_getIfExists(val, type, "type");
        kc::rjutils::_getIfExists(val, createdAt, "created_at");
        kc::rjutils::_getIfExists(val, updatedAt, "updated_at");
        kc::rjutils::_getIfExists(val, expiresAt, "expires_at");
        kc::rjutils::_getIfExists(val, status, "status");

        rj::Value condnVal(rj::kObjectType);
        kc::rjutils::_getIfExists(val, condnVal, "condition", kc::rjutils::_RJValueType::Object);
        condition.parse(condnVal.GetObject());

        auto it = val.FindMember("orders");
        if (it == val.MemberEnd()) { throw libException("Expected value wasn't found (GTT)"); };
        if (!it->value.IsArray()) {
            throw libException("Expected value's type wasn't the one expected. Expected Array");
        };

        for (auto& v : it->value.GetArray()) { orders.emplace_back(v.GetObject()); };
    };

    int ID = kc::DEFAULTINT;
    string userID;
    string type;
    string createdAt;
    string updatedAt;
    string expiresAt;
    string status;
    GTTCondition condition;
    std::vector<order> orders;
};
} // namespace kiteconnect