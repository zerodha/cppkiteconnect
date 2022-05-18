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

/// respresents an individual holding
struct holding {
    holding() = default;
    explicit holding(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, tradingsymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, ISIN, "isin");
        kc::rjutils::_getIfExists(val, product, "product");
        kc::rjutils::_getIfExists(val, price, "price");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, t1Quantity, "t1_quantity");
        kc::rjutils::_getIfExists(val, realisedQuantity, "realised_quantity");
        kc::rjutils::_getIfExists(val, collateralQuantity, "collateral_quantity");
        kc::rjutils::_getIfExists(val, collateralType, "collateral_type");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
        kc::rjutils::_getIfExists(val, closePrice, "close_price");
        kc::rjutils::_getIfExists(val, PnL, "pnl");
        kc::rjutils::_getIfExists(val, dayChange, "day_change");
        kc::rjutils::_getIfExists(val, dayChangePercentage, "day_change_percentage");
    };

    int instrumentToken = kc::DEFAULTINT;
    int quantity = kc::DEFAULTINT;
    int t1Quantity = kc::DEFAULTINT;
    int realisedQuantity = kc::DEFAULTINT;
    int collateralQuantity = kc::DEFAULTINT;
    double price = kc::DEFAULTDOUBLE;
    double averagePrice = kc::DEFAULTDOUBLE;
    double lastPrice = kc::DEFAULTDOUBLE;
    double closePrice = kc::DEFAULTDOUBLE;
    double PnL = kc::DEFAULTDOUBLE;
    double dayChange = kc::DEFAULTDOUBLE;
    double dayChangePercentage = kc::DEFAULTDOUBLE;
    string tradingsymbol;
    string exchange;
    string ISIN;
    string product;
    string collateralType;
};

/// represents an individual position
struct position {
    position() = default;
    explicit position(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        kc::rjutils::_getIfExists(val, tradingsymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, exchange, "exchange");
        kc::rjutils::_getIfExists(val, instrumentToken, "instrument_token");
        kc::rjutils::_getIfExists(val, product, "product");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, overnightQuantity, "overnight_quantity");
        kc::rjutils::_getIfExists(val, multiplier, "multiplier");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, closePrice, "close_price");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
        kc::rjutils::_getIfExists(val, value, "value");
        kc::rjutils::_getIfExists(val, PnL, "pnl");
        kc::rjutils::_getIfExists(val, M2M, "m2m");
        kc::rjutils::_getIfExists(val, unrealised, "unrealised");
        kc::rjutils::_getIfExists(val, realised, "realised");
        kc::rjutils::_getIfExists(val, buyQuantity, "buy_quantity");
        kc::rjutils::_getIfExists(val, buyPrice, "buy_price");
        kc::rjutils::_getIfExists(val, buyValue, "buy_value");
        kc::rjutils::_getIfExists(val, buyM2MValue, "buy_m2m");
        kc::rjutils::_getIfExists(val, sellQuantity, "sell_quantity");
        kc::rjutils::_getIfExists(val, sellPrice, "sell_price");
        kc::rjutils::_getIfExists(val, sellValue, "sell_value");
        kc::rjutils::_getIfExists(val, sellM2MValue, "sell_m2m");
        kc::rjutils::_getIfExists(val, dayBuyQuantity, "day_buy_quantity");
        kc::rjutils::_getIfExists(val, dayBuyPrice, "day_buy_price");
        kc::rjutils::_getIfExists(val, dayBuyValue, "day_buy_value");
        kc::rjutils::_getIfExists(val, daySellQuantity, "day_sell_quantity");
        kc::rjutils::_getIfExists(val, daySellPrice, "day_sell_price");
        kc::rjutils::_getIfExists(val, daySellValue, "day_sell_value");
    };

    int instrumentToken = kc::DEFAULTINT;
    int quantity = kc::DEFAULTINT;
    int buyQuantity = kc::DEFAULTINT;
    int overnightQuantity = kc::DEFAULTINT;
    int sellQuantity = kc::DEFAULTINT;
    int dayBuyQuantity = kc::DEFAULTINT;
    int daySellQuantity = kc::DEFAULTINT;
    double multiplier = kc::DEFAULTDOUBLE;
    double averagePrice = kc::DEFAULTDOUBLE;
    double closePrice = kc::DEFAULTDOUBLE;
    double lastPrice = kc::DEFAULTDOUBLE;
    double value = kc::DEFAULTDOUBLE;
    double PnL = kc::DEFAULTDOUBLE;
    double M2M = kc::DEFAULTDOUBLE;
    double unrealised = kc::DEFAULTDOUBLE;
    double realised = kc::DEFAULTDOUBLE;
    double buyPrice = kc::DEFAULTDOUBLE;
    double buyValue = kc::DEFAULTDOUBLE;
    double buyM2MValue = kc::DEFAULTDOUBLE;
    double sellPrice = kc::DEFAULTDOUBLE;
    double sellValue = kc::DEFAULTDOUBLE;
    double sellM2MValue = kc::DEFAULTDOUBLE;
    double dayBuyPrice = kc::DEFAULTDOUBLE;
    double dayBuyValue = kc::DEFAULTDOUBLE;
    double daySellPrice = kc::DEFAULTDOUBLE;
    double daySellValue = kc::DEFAULTDOUBLE;
    string tradingsymbol;
    string exchange;
    string product;
};

/// represents response of the fetch all positions call
struct positions {
    positions() = default;
    explicit positions(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        rj::Value netVal(rj::kArrayType);
        kc::rjutils::_getIfExists(val, netVal, "net", kc::rjutils::_RJValueType::Array);
        for (auto& i : netVal.GetArray()) { net.emplace_back(i.GetObject()); };

        rj::Value dayVal(rj::kArrayType);
        kc::rjutils::_getIfExists(val, dayVal, "day", kc::rjutils::_RJValueType::Array);
        for (auto& i : dayVal.GetArray()) { day.emplace_back(i.GetObject()); };
    };

    std::vector<position> net;
    std::vector<position> day;
};

/// represents parameters for the `convertPosition` function
struct convertPositionParams {
    GENERATE_FLUENT_METHOD(convertPositionParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(convertPositionParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(convertPositionParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(convertPositionParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(convertPositionParams, const string&, positionType, PositionType);
    GENERATE_FLUENT_METHOD(convertPositionParams, const string&, oldProduct, OldProduct);
    GENERATE_FLUENT_METHOD(convertPositionParams, const string&, newProduct, NewProduct);

    int quantity;
    string exchange;
    string symbol;
    string transactionType;
    string positionType;
    string oldProduct;
    string newProduct;
};

} // namespace kiteconnect