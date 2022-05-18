#pragma once

#include <cstdint>
#include <string>

#include "../config.hpp"
#include "../utils.hpp"

namespace kiteconnect {

using std::string;
namespace kc = kiteconnect;

/// represents an individual instrument response
struct instrument {
    instrument() = default;
    explicit instrument(const string& val) { parse(val); };

    void parse(const string& val) {
        std::vector<string> tokens = kc::_split(val, ',');

        static const auto toInt = [](const string& str) -> int { return (str.empty()) ? 0 : std::stoi(str); };
        static const auto toDouble = [](const string& str) -> double { return (str.empty()) ? 0.0 : std::stod(str); };

        instrumentToken = toInt(tokens[INSTRUMENT_TOKEN_IDX]);
        exchangeToken = toInt(tokens[EXCHANGE_TOKEN_IDX]);
        tradingsymbol = tokens[TRADINGSYMBOL_IDX];
        name = tokens[NAME_IDX];
        lastPrice = toDouble(tokens[LAST_PRICE_IDX]);
        expiry = tokens[EXPIRY_IDX];
        strikePrice = toDouble(tokens[STRIKE_PRICE_IDX]);
        tickSize = toDouble(tokens[TICK_SIZE_IDX]);
        lotSize = toDouble(tokens[LOT_SIZE_IDX]);
        instrumentType = tokens[INSTRUMENT_TYPE_IDX];
        segment = tokens[SEGMENT_IDX];
        exchange = tokens[EXCHANGE_IDX];
    };

    int instrumentToken = kc::DEFAULTINT;
    int exchangeToken = kc::DEFAULTINT;
    double lastPrice = kc::DEFAULTDOUBLE;
    double strikePrice = kc::DEFAULTDOUBLE;
    double tickSize = kc::DEFAULTDOUBLE;
    double lotSize = kc::DEFAULTDOUBLE;
    string tradingsymbol;
    string name;
    string expiry;
    string instrumentType;
    string segment;
    string exchange;

    static constexpr uint8_t INSTRUMENT_TOKEN_IDX = 0;
    static constexpr uint8_t EXCHANGE_TOKEN_IDX = 1;
    static constexpr uint8_t TRADINGSYMBOL_IDX = 2;
    static constexpr uint8_t NAME_IDX = 3;
    static constexpr uint8_t LAST_PRICE_IDX = 4;
    static constexpr uint8_t EXPIRY_IDX = 5;
    static constexpr uint8_t STRIKE_PRICE_IDX = 6;
    static constexpr uint8_t TICK_SIZE_IDX = 7;
    static constexpr uint8_t LOT_SIZE_IDX = 8;
    static constexpr uint8_t INSTRUMENT_TYPE_IDX = 9;
    static constexpr uint8_t SEGMENT_IDX = 10;
    static constexpr uint8_t EXCHANGE_IDX = 11;
};

/// mfInstrument represents an individual mutual fund instrument response
struct mfInstrument {
    mfInstrument() = default;
    explicit mfInstrument(const string& val) { parse(val); };

    void parse(const string& val) {
        std::vector<string> tokens = kc::_split(val, ',');

        static const auto toDouble = [](const string& str) -> double { return (str.empty()) ? 0.0 : std::stod(str); };

        tradingsymbol = tokens[TRADINGSYMBOL_IDX];
        amc = tokens[AMC_IDX];
        name = tokens[NAME_IDX];
        purchaseAllowed = static_cast<bool>(std::stoi(tokens[PURCHASE_ALLOWED_IDX]));
        redemtpionAllowed = static_cast<bool>(std::stoi(tokens[REDEMPTION_ALLOWED_IDX]));
        minimumPurchaseAmount = toDouble(tokens[MIN_PURCHASE_AMOUNT_IDX]);
        purchaseAmountMultiplier = toDouble(tokens[PURCHASE_AMOUNT_MUL_IDX]);
        minimumAdditionalPurchaseAmount = toDouble(tokens[MIN_ADDITIONAL_PURCHASE_AMOUNT_IDX]);
        minimumRedemptionQuantity = toDouble(tokens[MIN_REDEMPTION_QUANTITY_IDX]);
        redemptionQuantityMultiplier = toDouble(tokens[REDEMPTION_QUANTITY_MUL_IDX]);
        dividendType = tokens[DIVIDEND_TYPE_IDX];
        schemeType = tokens[SCHEME_TYPE_IDX];
        plan = tokens[PLAN_IDX];
        settlementType = tokens[SETTLEMENT_TYPE_IDX];
        lastPrice = toDouble(tokens[LAST_PRICE_IDX]);
        lastPriceDate = tokens[LAST_PRICE_DATE_IDX];
    };

    double minimumPurchaseAmount = kc::DEFAULTDOUBLE;
    double purchaseAmountMultiplier = kc::DEFAULTDOUBLE;
    double minimumAdditionalPurchaseAmount = kc::DEFAULTDOUBLE;
    double minimumRedemptionQuantity = kc::DEFAULTDOUBLE;
    double redemptionQuantityMultiplier = kc::DEFAULTDOUBLE;
    double lastPrice = kc::DEFAULTDOUBLE;
    bool purchaseAllowed = false;
    bool redemtpionAllowed = false;
    string tradingsymbol;
    string amc;
    string name;
    string dividendType;
    string schemeType;
    string plan;
    string settlementType;
    string lastPriceDate;

    static constexpr uint8_t TRADINGSYMBOL_IDX = 0;
    static constexpr uint8_t AMC_IDX = 1;
    static constexpr uint8_t NAME_IDX = 2;
    static constexpr uint8_t PURCHASE_ALLOWED_IDX = 3;
    static constexpr uint8_t REDEMPTION_ALLOWED_IDX = 4;
    static constexpr uint8_t MIN_PURCHASE_AMOUNT_IDX = 5;
    static constexpr uint8_t PURCHASE_AMOUNT_MUL_IDX = 6;
    static constexpr uint8_t MIN_ADDITIONAL_PURCHASE_AMOUNT_IDX = 7;
    static constexpr uint8_t MIN_REDEMPTION_QUANTITY_IDX = 8;
    static constexpr uint8_t REDEMPTION_QUANTITY_MUL_IDX = 9;
    static constexpr uint8_t DIVIDEND_TYPE_IDX = 10;
    static constexpr uint8_t SCHEME_TYPE_IDX = 11;
    static constexpr uint8_t PLAN_IDX = 12;
    static constexpr uint8_t SETTLEMENT_TYPE_IDX = 13;
    static constexpr uint8_t LAST_PRICE_IDX = 14;
    static constexpr uint8_t LAST_PRICE_DATE_IDX = 15;
};

} // namespace kiteconnect