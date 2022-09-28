#pragma once

#include <string>

#include "../config.hpp"
#include "../utils.hpp"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

namespace kiteconnect {

using std::string;
namespace rj = rapidjson;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

/// respresents an individual holding
struct holding {
    holding() = default;
    explicit holding(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        exchange = utils::json::get<string>(val, "exchange");
        instrumentToken = utils::json::get<int>(val, "instrument_token");
        ISIN = utils::json::get<string>(val, "isin");
        product = utils::json::get<string>(val, "product");
        price = utils::json::get<double>(val, "price");
        quantity = utils::json::get<int>(val, "quantity");
        t1Quantity = utils::json::get<int>(val, "t1_quantity");
        realisedQuantity = utils::json::get<int>(val, "realised_quantity");
        collateralQuantity = utils::json::get<int>(val, "collateral_quantity");
        collateralType = utils::json::get<string>(val, "collateral_type");
        averagePrice = utils::json::get<double>(val, "average_price");
        lastPrice = utils::json::get<double>(val, "last_price");
        closePrice = utils::json::get<double>(val, "close_price");
        PnL = utils::json::get<double>(val, "pnl");
        dayChange = utils::json::get<double>(val, "day_change");
        dayChangePercentage = utils::json::get<double>(val, "day_change_percentage");
    };

    int instrumentToken = -1;
    int quantity = -1;
    int t1Quantity = -1;
    int realisedQuantity = -1;
    int collateralQuantity = -1;
    double price = -1;
    double averagePrice = -1;
    double lastPrice = -1;
    double closePrice = -1;
    double PnL = -1;
    double dayChange = -1;
    double dayChangePercentage = -1;
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
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        exchange = utils::json::get<string>(val, "exchange");
        instrumentToken = utils::json::get<int>(val, "instrument_token");
        product = utils::json::get<string>(val, "product");
        quantity = utils::json::get<int>(val, "quantity");
        overnightQuantity = utils::json::get<int>(val, "overnight_quantity");
        multiplier = utils::json::get<double>(val, "multiplier");
        averagePrice = utils::json::get<double>(val, "average_price");
        closePrice = utils::json::get<double>(val, "close_price");
        lastPrice = utils::json::get<double>(val, "last_price");
        value = utils::json::get<double>(val, "value");
        PnL = utils::json::get<double>(val, "pnl");
        M2M = utils::json::get<double>(val, "m2m");
        unrealised = utils::json::get<double>(val, "unrealised");
        realised = utils::json::get<double>(val, "realised");
        buyQuantity = utils::json::get<int>(val, "buy_quantity");
        buyPrice = utils::json::get<double>(val, "buy_price");
        buyValue = utils::json::get<double>(val, "buy_value");
        buyM2MValue = utils::json::get<double>(val, "buy_m2m");
        sellQuantity = utils::json::get<int>(val, "sell_quantity");
        sellPrice = utils::json::get<double>(val, "sell_price");
        sellValue = utils::json::get<double>(val, "sell_value");
        sellM2MValue = utils::json::get<double>(val, "sell_m2m");
        dayBuyQuantity = utils::json::get<int>(val, "day_buy_quantity");
        dayBuyPrice = utils::json::get<double>(val, "day_buy_price");
        dayBuyValue = utils::json::get<double>(val, "day_buy_value");
        daySellQuantity = utils::json::get<int>(val, "day_sell_quantity");
        daySellPrice = utils::json::get<double>(val, "day_sell_price");
        daySellValue = utils::json::get<double>(val, "day_sell_value");
    };

    int instrumentToken = -1;
    int quantity = -1;
    int buyQuantity = -1;
    int overnightQuantity = -1;
    int sellQuantity = -1;
    int dayBuyQuantity = -1;
    int daySellQuantity = -1;
    double multiplier = -1;
    double averagePrice = -1;
    double closePrice = -1;
    double lastPrice = -1;
    double value = -1;
    double PnL = -1;
    double M2M = -1;
    double unrealised = -1;
    double realised = -1;
    double buyPrice = -1;
    double buyValue = -1;
    double buyM2MValue = -1;
    double sellPrice = -1;
    double sellValue = -1;
    double sellM2MValue = -1;
    double dayBuyPrice = -1;
    double dayBuyValue = -1;
    double daySellPrice = -1;
    double daySellValue = -1;
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
        utils::json::get<utils::json::JsonArray>(val, netVal, "net");
        for (auto& i : netVal.GetArray()) { net.emplace_back(i.GetObject()); };

        rj::Value dayVal(rj::kArrayType);
        utils::json::get<utils::json::JsonArray>(val, dayVal, "day");
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

    int quantity = -1;
    string exchange;
    string symbol;
    string transactionType;
    string positionType;
    string oldProduct;
    string newProduct;
};

} // namespace kiteconnect