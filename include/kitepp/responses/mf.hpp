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

/// represents parameters required for the `placeMfOrder` function
struct placeMfOrderParams {
    GENERATE_FLUENT_METHOD(placeMfOrderParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(placeMfOrderParams, double, amount, Amount);
    GENERATE_FLUENT_METHOD(placeMfOrderParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(placeMfOrderParams, const string&, transactionType, TransactionType);
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
    GENERATE_FLUENT_METHOD(placeMfSipParams, int, installmentDay, InstallmentDay);
    GENERATE_FLUENT_METHOD(placeMfSipParams, double, amount, Amount);
    GENERATE_FLUENT_METHOD(placeMfSipParams, double, initialAmount, InitialAmount);
    GENERATE_FLUENT_METHOD(placeMfSipParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(placeMfSipParams, const string&, frequency, Frequency);
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
        kc::rjutils::_getIfExists(val, orderId, "order_id");
        kc::rjutils::_getIfExists(val, sipId, "sip_id");
    };

    string orderId;
    string sipId;
};

/// represents parameters required for the `modifyMfSip` function
struct modifyMfSipParams {
    GENERATE_FLUENT_METHOD(modifyMfSipParams, int, installments, Installments);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, int, installmentDay, InstallmentDay);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, double, amount, Amount);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, const string&, sipId, SipId);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, const string&, status, Status);
    GENERATE_FLUENT_METHOD(modifyMfSipParams, const string&, frequency, Frequency);

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
        kc::rjutils::_getIfExists(val, orderID, "order_id");
        kc::rjutils::_getIfExists(val, exchangeOrderID, "exchange_order_id");
        kc::rjutils::_getIfExists(val, tradingsymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, status, "status");
        kc::rjutils::_getIfExists(val, statusMessage, "status_message");
        kc::rjutils::_getIfExists(val, folio, "folio");
        kc::rjutils::_getIfExists(val, fund, "fund");
        kc::rjutils::_getIfExists(val, orderTimestamp, "order_timestamp");
        kc::rjutils::_getIfExists(val, exchangeTimestamp, "exchange_timestamp");
        kc::rjutils::_getIfExists(val, settlementID, "settlement_id");
        kc::rjutils::_getIfExists(val, transactionType, "transaction_type");
        kc::rjutils::_getIfExists(val, variety, "variety");
        kc::rjutils::_getIfExists(val, purchaseType, "purchase_type");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
        kc::rjutils::_getIfExists(val, amount, "amount");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, placedBy, "placed_by");
        kc::rjutils::_getIfExists(val, tag, "tag");
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
        kc::rjutils::_getIfExists(val, folio, "folio");
        kc::rjutils::_getIfExists(val, fund, "fund");
        kc::rjutils::_getIfExists(val, tradingsymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, averagePrice, "average_price");
        kc::rjutils::_getIfExists(val, lastPrice, "last_price");
        kc::rjutils::_getIfExists(val, lastPriceDate, "last_price_date");
        kc::rjutils::_getIfExists(val, Pnl, "pnl");
        kc::rjutils::_getIfExists(val, quantity, "quantity");
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
        kc::rjutils::_getIfExists(val, ID, "sip_id");
        kc::rjutils::_getIfExists(val, tradingsymbol, "tradingsymbol");
        kc::rjutils::_getIfExists(val, fundName, "fund");
        kc::rjutils::_getIfExists(val, dividendType, "dividend_type");
        kc::rjutils::_getIfExists(val, transactionType, "transaction_type");
        kc::rjutils::_getIfExists(val, status, "status");
        kc::rjutils::_getIfExists(val, SIPType, "sip_type");
        kc::rjutils::_getIfExists(val, created, "created");
        kc::rjutils::_getIfExists(val, frequency, "frequency");
        kc::rjutils::_getIfExists(val, instalmentAmount, "instalment_amount");
        kc::rjutils::_getIfExists(val, instalments, "instalments");
        kc::rjutils::_getIfExists(val, lastInstalment, "last_instalment");
        kc::rjutils::_getIfExists(val, pendingInstalments, "pending_instalments");
        kc::rjutils::_getIfExists(val, instalmentDay, "instalment_day");
        kc::rjutils::_getIfExists(val, completedInstalments, "completed_instalments");
        kc::rjutils::_getIfExists(val, nextInstalment, "next_instalment");
        kc::rjutils::_getIfExists(val, triggerPrice, "trigger_price");
        kc::rjutils::_getIfExists(val, tag, "tag");
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

} // namespace kiteconnect