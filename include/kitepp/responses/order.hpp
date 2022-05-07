#include <string>

#include "../config.hpp"

#define GENERATE_FLUENT_METHOD(returnType, fieldType, fieldName, methodName)                                           \
    returnType& methodName(fieldType arg) {                                                                            \
        fieldName = arg;                                                                                               \
        return *this;                                                                                                  \
    };

namespace kiteconnect {

namespace kc = kiteconnect;
using std::string;

struct placeOrderParams {
    GENERATE_FLUENT_METHOD(placeOrderParams, int, quantity, Quantity);
    GENERATE_FLUENT_METHOD(placeOrderParams, int, disclosedQuantity, DisclosedQuantity);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, price, Price);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, triggerPrice, TriggerPrice);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, squareOff, SquareOff);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, stopLoss, StopLoss);
    GENERATE_FLUENT_METHOD(placeOrderParams, double, trailingStopLoss, TrailingStopLoss);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, variety, Variety);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, exchange, Exchange);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, symbol, Symbol);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, transactionType, TransactionType);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, product, Product);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, orderType, OrderType);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, validity, Validity);
    GENERATE_FLUENT_METHOD(placeOrderParams, const string&, tag, Tag);

    int quantity = kc::DEFAULTINT;
    int disclosedQuantity = kc::DEFAULTINT;
    double price = kc::DEFAULTDOUBLE;
    double triggerPrice = kc::DEFAULTDOUBLE;
    double squareOff = kc::DEFAULTDOUBLE;
    double stopLoss = kc::DEFAULTDOUBLE;
    double trailingStopLoss = kc::DEFAULTDOUBLE;
    string variety;
    string exchange;
    string symbol;
    string transactionType;
    string product;
    string orderType;
    string validity;
    string tag;
};

#undef GENERATE_FLUENT_METHOD

} // namespace kiteconnect