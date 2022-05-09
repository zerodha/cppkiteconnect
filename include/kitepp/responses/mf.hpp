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

    double quantity = kc::DEFAULTDOUBLE;
    double amount = kc::DEFAULTDOUBLE;
    double lastPrice = kc::DEFAULTDOUBLE;
    double averagePrice = kc::DEFAULTDOUBLE;
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

    double averagePrice = kc::DEFAULTDOUBLE;
    double lastPrice = kc::DEFAULTDOUBLE;
    double Pnl = kc::DEFAULTDOUBLE;
    double quantity = kc::DEFAULTDOUBLE;
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

    int pendingInstalments = kc::DEFAULTINT;
    int instalmentDay = kc::DEFAULTINT;
    int completedInstalments = kc::DEFAULTINT;
    int instalments = kc::DEFAULTINT;
    double instalmentAmount = kc::DEFAULTDOUBLE;
    double triggerPrice = kc::DEFAULTDOUBLE;
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