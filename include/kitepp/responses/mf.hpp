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

/// represents parameters required for the `placeMfOrder` function
struct placeMfOrderParams {
    GENERATE_FLUENT_METHOD(placeMfOrderParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(placeMfOrderParams, double, amount, Amount);
    GENERATE_FLUENT_METHOD(placeMfOrderParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(
        placeMfOrderParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(placeMfOrderParams, const string&, tag, Tag);

    std::optional<int> quantity;
    std::optional<double> amount;
    string symbol;
    string transactionType;
    std::optional<string> tag;
};

/// represents parameters required for the `placeMfSip` function
struct placeMfSipParams {
    GENERATE_FLUENT_METHOD(placeMfSipParams, int, installments, Installments);
    GENERATE_FLUENT_METHOD(
        placeMfSipParams, int, installmentDay, InstallmentDay);
    GENERATE_FLUENT_METHOD(placeMfSipParams, double, amount, Amount);
    GENERATE_FLUENT_METHOD(
        placeMfSipParams, double, initialAmount, InitialAmount);
    GENERATE_FLUENT_METHOD(placeMfSipParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(
        placeMfSipParams, const string&, frequency, Frequency);
    GENERATE_FLUENT_METHOD(placeMfSipParams, const string&, tag, Tag);

    int installments;
    std::optional<int> installmentDay;
    double amount;
    std::optional<double> initialAmount;
    string symbol;
    string frequency;
    std::optional<string> tag;
};

/// represents response returned by the `placeMfSip` method
struct placeMfSipResponse {
    placeMfSipResponse() = default;
    explicit placeMfSipResponse(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        orderId = utils::json::get<string>(val, "order_id");
        sipId = utils::json::get<string>(val, "sip_id");
    };

    string orderId;
    string sipId;
};

/// represents parameters required for the `modifyMfSip` function
struct modifyMfSipParams {
    GENERATE_FLUENT_METHOD(modifyMfSipParams, int, installments, Installments);
    GENERATE_FLUENT_METHOD(
        modifyMfSipParams, int, installmentDay, InstallmentDay);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, double, amount, Amount);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, const string&, sipId, SipId);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, const string&, status, Status);
    GENERATE_FLUENT_METHOD(
        modifyMfSipParams, const string&, frequency, Frequency);

    std::optional<int> installmentDay;
    std::optional<int> installments;
    std::optional<double> amount;
    string sipId;
    std::optional<string> status;
    std::optional<string> frequency;
};

/// mfOrder represents an individual mutual fund order response
struct mfOrder {
    mfOrder() = default;
    explicit mfOrder(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        orderID = utils::json::get<string>(val, "order_id");
        exchangeOrderID = utils::json::get<string>(val, "exchange_order_id");
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        status = utils::json::get<string>(val, "status");
        statusMessage = utils::json::get<string>(val, "status_message");
        folio = utils::json::get<string>(val, "folio");
        fund = utils::json::get<string>(val, "fund");
        orderTimestamp = utils::json::get<string>(val, "order_timestamp");
        exchangeTimestamp = utils::json::get<string>(val, "exchange_timestamp");
        settlementID = utils::json::get<string>(val, "settlement_id");
        transactionType = utils::json::get<string>(val, "transaction_type");
        variety = utils::json::get<string>(val, "variety");
        purchaseType = utils::json::get<string>(val, "purchase_type");
        quantity = utils::json::get<double>(val, "quantity");
        amount = utils::json::get<double>(val, "amount");
        lastPrice = utils::json::get<double>(val, "last_price");
        averagePrice = utils::json::get<double>(val, "average_price");
        placedBy = utils::json::get<string>(val, "placed_by");
        tag = utils::json::get<string>(val, "tag");
    };

    double quantity = -1;
    double amount = -1;
    double lastPrice = -1;
    double averagePrice = -1;
    string orderID;
    string exchangeOrderID;
    string tradingsymbol;
    string status;
    string statusMessage;
    string folio;
    string fund;
    string orderTimestamp;
    string exchangeTimestamp;
    string settlementID;
    string transactionType;
    string variety;
    string purchaseType;
    string placedBy;
    string tag;
};

/// mfHolding represents an individual mutual fund holding
struct mfHolding {

    mfHolding() = default;
    explicit mfHolding(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        folio = utils::json::get<string>(val, "folio");
        fund = utils::json::get<string>(val, "fund");
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        averagePrice = utils::json::get<double>(val, "average_price");
        lastPrice = utils::json::get<double>(val, "last_price");
        lastPriceDate = utils::json::get<string>(val, "last_price_date");
        Pnl = utils::json::get<double>(val, "pnl");
        quantity = utils::json::get<double>(val, "quantity");
    };

    double averagePrice = -1;
    double lastPrice = -1;
    double Pnl = -1;
    double quantity = -1;
    string folio;
    string fund;
    string tradingsymbol;
    string lastPriceDate;
};

/// mfSip represents an individual mutual fund SIP response
struct mfSip {
    mfSip() = default;
    explicit mfSip(const rj::Value::Object& val) { parse(val); };

    void parse(const rj::Value::Object& val) {
        ID = utils::json::get<string>(val, "sip_id");
        tradingsymbol = utils::json::get<string>(val, "tradingsymbol");
        fundName = utils::json::get<string>(val, "fund");
        dividendType = utils::json::get<string>(val, "dividend_type");
        transactionType = utils::json::get<string>(val, "transaction_type");
        status = utils::json::get<string>(val, "status");
        SIPType = utils::json::get<string>(val, "sip_type");
        created = utils::json::get<string>(val, "created");
        frequency = utils::json::get<string>(val, "frequency");
        instalmentAmount = utils::json::get<double>(val, "instalment_amount");
        instalments = utils::json::get<int>(val, "instalments");
        lastInstalment = utils::json::get<string>(val, "last_instalment");
        pendingInstalments = utils::json::get<int>(val, "pending_instalments");
        instalmentDay = utils::json::get<int>(val, "instalment_day");
        completedInstalments =
            utils::json::get<int>(val, "completed_instalments");
        nextInstalment = utils::json::get<string>(val, "next_instalment");
        triggerPrice = utils::json::get<double>(val, "trigger_price");
        tag = utils::json::get<string>(val, "tag");
    };

    int pendingInstalments = -1;
    int instalmentDay = -1;
    int completedInstalments = -1;
    int instalments = -1;
    double instalmentAmount = -1;
    double triggerPrice = -1;
    string ID;
    string tradingsymbol;
    string fundName;
    string dividendType;
    string transactionType;
    string status;
    string SIPType;
    string created;
    string frequency;
    string lastInstalment;
    string nextInstalment;
    string tag;
};

/// mfInstrument represents an individual mutual fund instrument response
struct mfInstrument {
    mfInstrument() = default;
    explicit mfInstrument(const string& val) { parse(val); };

    void parse(const string& val) {
        std::vector<string> tokens = utils::split(val, ',');

        static const auto toDouble = [](const string& str) -> double {
            return (str.empty()) ? 0.0 : std::stod(str);
        };

        tradingsymbol = tokens[TRADINGSYMBOL_IDX];
        amc = tokens[AMC_IDX];
        name = tokens[NAME_IDX];
        purchaseAllowed =
            static_cast<bool>(std::stoi(tokens[PURCHASE_ALLOWED_IDX]));
        redemtpionAllowed =
            static_cast<bool>(std::stoi(tokens[REDEMPTION_ALLOWED_IDX]));
        minimumPurchaseAmount = toDouble(tokens[MIN_PURCHASE_AMOUNT_IDX]);
        purchaseAmountMultiplier = toDouble(tokens[PURCHASE_AMOUNT_MUL_IDX]);
        minimumAdditionalPurchaseAmount =
            toDouble(tokens[MIN_ADDITIONAL_PURCHASE_AMOUNT_IDX]);
        minimumRedemptionQuantity =
            toDouble(tokens[MIN_REDEMPTION_QUANTITY_IDX]);
        redemptionQuantityMultiplier =
            toDouble(tokens[REDEMPTION_QUANTITY_MUL_IDX]);
        dividendType = tokens[DIVIDEND_TYPE_IDX];
        schemeType = tokens[SCHEME_TYPE_IDX];
        plan = tokens[PLAN_IDX];
        settlementType = tokens[SETTLEMENT_TYPE_IDX];
        lastPrice = toDouble(tokens[LAST_PRICE_IDX]);
        lastPriceDate = tokens[LAST_PRICE_DATE_IDX];
    };

    double minimumPurchaseAmount = -1;
    double purchaseAmountMultiplier = -1;
    double minimumAdditionalPurchaseAmount = -1;
    double minimumRedemptionQuantity = -1;
    double redemptionQuantityMultiplier = -1;
    double lastPrice = -1;
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