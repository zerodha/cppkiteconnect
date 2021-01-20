#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ios>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "kitepp.hpp"
#include "kitepp/responses.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using std::string;
using ::testing::_;
namespace rj = rapidjson;

class mockKite : public kitepp::kite {

  public:
    mockKite(): kite("apiKey123") {};

    MOCK_METHOD(void, _sendReq,
        (rj::Document & data, const kitepp::_methods& mtd, const string& endpoint,
            (const std::vector<std::pair<string, string>>) &bodyParams, bool isJson),
        (override));
};

TEST(kiteTest, constructorTest) {

    mockKite Kite;
    const string APIKey_expected = "apiKey123";

    EXPECT_EQ(Kite.getAPIKey(), APIKey_expected);
};

TEST(kiteTest, loginURLTest) {

    mockKite Kite;
    const string loginURL_expected = "https://kite.zerodha.com/connect/login?v=3&api_key=apiKey123";

    EXPECT_EQ(Kite.loginURL(), loginURL_expected);
};

TEST(kiteTest, generateSessionTest) {

    std::ifstream jsonFile("../../tests/mock_responses/generate_session.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    // We need to pass an lambda instead of directly modifying the argument because rapidjson::Document objcect's copy
    // constructor is deleted and setArgumentRefree<> copies the object passed internally
    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::POST, "/session/token", _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kitepp::userSession session = Kite.generateSession("arg1", "arg2");

    // Expected values
    EXPECT_EQ(session.profile.userName, "Kite Connect");
    EXPECT_EQ(session.profile.userShortName, "Kite");
    EXPECT_EQ(session.profile.avatarURL, "");
    EXPECT_EQ(session.profile.userType, "investor");
    EXPECT_EQ(session.profile.email, "kite@kite.trade");
    EXPECT_EQ(session.profile.phone, "");
    EXPECT_EQ(session.profile.broker, "ZERODHA");
    EXPECT_THAT(session.profile.products, ::testing::ElementsAre("BO", "CNC", "CO", "MIS", "NRML"));
    EXPECT_THAT(session.profile.orderTypes, ::testing::ElementsAre("LIMIT", "MARKET", "SL", "SL-M"));
    EXPECT_THAT(session.profile.exchanges, ::testing::ElementsAre("MCX", "BSE", "NSE", "BFO", "NFO", "CDS"));

    EXPECT_EQ(session.tokens.userID, "XX000");
    EXPECT_EQ(session.tokens.accessToken, "yyyyyy");
    EXPECT_EQ(session.tokens.refreshToken, "");

    EXPECT_EQ(session.apiKey, "xxxxxx");
    EXPECT_EQ(session.publicToken, "zzzzzz");
    EXPECT_EQ(session.loginTime, "2018-01-01 16:15:14");
};

TEST(kiteTest, invalidateSessionTest) {

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::DEL, _, _, _)).WillOnce(::testing::Return());

    Kite.invalidateSession();
};

TEST(kiteTest, profile) {

    std::ifstream jsonFile("../../tests/mock_responses/profile.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kitepp::userProfile profile = Kite.profile();

    // Expected values
    EXPECT_EQ(profile.userName, "AxAx Bxx");
    EXPECT_EQ(profile.userShortName, "abc");
    EXPECT_EQ(profile.avatarURL, "");
    EXPECT_EQ(profile.userType, "investor");
    EXPECT_EQ(profile.email, "xxxyyy@gmail.com");
    EXPECT_EQ(profile.phone, "");
    EXPECT_EQ(profile.broker, "ZERODHA");
    EXPECT_THAT(profile.products, ::testing::ElementsAre("BO", "CNC", "CO", "MIS", "NRML"));
    EXPECT_THAT(profile.orderTypes, ::testing::ElementsAre("LIMIT", "MARKET", "SL", "SL-M"));
    EXPECT_THAT(profile.exchanges, ::testing::ElementsAre("BSE", "BFO", "NFO", "MCX", "CDS", "NSE"));
};

TEST(kiteTest, getMarginsTest1) {

    std::ifstream jsonFile("../../tests/mock_responses/margins.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kitepp::allMargins margins = Kite.getMargins();

    // Expected values
    EXPECT_EQ(margins.equity.enabled, true);
    EXPECT_DOUBLE_EQ(margins.equity.net, 15481.524);

    EXPECT_DOUBLE_EQ(margins.equity.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.equity.available.cash, 9929.024);
    EXPECT_DOUBLE_EQ(margins.equity.available.collateral, 5554.5);
    EXPECT_DOUBLE_EQ(margins.equity.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.equity.used.debits, 2);
    EXPECT_DOUBLE_EQ(margins.equity.used.exposure, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.M2MRealised, -2);
    EXPECT_DOUBLE_EQ(margins.equity.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.span, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.turnover, 0);

    EXPECT_EQ(margins.commodity.enabled, true);
    EXPECT_DOUBLE_EQ(margins.commodity.net, 29675.93);

    EXPECT_DOUBLE_EQ(margins.commodity.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.available.cash, 29249.93);
    EXPECT_DOUBLE_EQ(margins.commodity.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.commodity.used.debits, -426);
    EXPECT_DOUBLE_EQ(margins.commodity.used.exposure, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.M2MRealised, 426);
    EXPECT_DOUBLE_EQ(margins.commodity.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.span, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.turnover, 0);
}

TEST(kiteTest, getMarginsTest2) {

    std::ifstream jsonFile("../../tests/mock_responses/margins_equity.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kitepp::margins margins = Kite.getMargins("equity");

    // Expected values
    EXPECT_EQ(margins.enabled, true);
    EXPECT_DOUBLE_EQ(margins.net, 15481.524);

    EXPECT_DOUBLE_EQ(margins.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.available.cash, 9929.024);
    EXPECT_DOUBLE_EQ(margins.available.collateral, 5554.5);
    EXPECT_DOUBLE_EQ(margins.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.used.debits, 2);
    EXPECT_DOUBLE_EQ(margins.used.exposure, 0);
    EXPECT_DOUBLE_EQ(margins.used.M2MRealised, -2);
    EXPECT_DOUBLE_EQ(margins.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.used.span, 0);
    EXPECT_DOUBLE_EQ(margins.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.used.turnover, 0);
}

TEST(kiteTest, placeOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.placeOrder("arg1", "arg2", "arg3", "arg4", 1, "arg6", "arg7");

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, modifyOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.modifyOrder("arg1", "arg2");

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, cancelOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.cancelOrder("arg1", "arg2");

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, exitOrderTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_response.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    string orderID = Kite.exitOrder("arg1", "arg2");

    EXPECT_EQ(orderID, "151220000000000");
}

TEST(kiteTest, ordersTest) {

    std::ifstream jsonFile("../../tests/mock_responses/orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kitepp::order> Orders = Kite.orders();

    // Expected values
    ASSERT_THAT(Orders.size(), 7);

    kitepp::order order1 = Orders[0];
    EXPECT_EQ(order1.accountID, "");
    EXPECT_EQ(order1.placedBy, "DA0017");
    EXPECT_EQ(order1.orderID, "171228000850038");
    EXPECT_EQ(order1.exchangeOrderID, "211736200053802");
    EXPECT_EQ(order1.parentOrderID, "");
    EXPECT_EQ(order1.status, "COMPLETE");
    EXPECT_EQ(order1.statusMessage, "");
    EXPECT_EQ(order1.orderTimestamp, "2017-12-28 11:39:14");
    EXPECT_EQ(order1.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order1.exchangeTimestamp, "2017-12-28 11:39:14");
    EXPECT_EQ(order1.rejectedBy, "");
    EXPECT_EQ(order1.variety, "regular");
    EXPECT_EQ(order1.exchange, "MCX");
    EXPECT_EQ(order1.tradingSymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(order1.instrumentToken, 53505799);
    EXPECT_EQ(order1.orderType, "LIMIT");
    EXPECT_EQ(order1.transactionType, "SELL");
    EXPECT_EQ(order1.validity, "DAY");
    EXPECT_EQ(order1.product, "NRML");
    EXPECT_EQ(order1.quantity, 3);
    EXPECT_EQ(order1.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order1.price, 23337);
    EXPECT_DOUBLE_EQ(order1.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order1.averagePrice, 23337);
    EXPECT_EQ(order1.filledQuantity, 3);
    EXPECT_EQ(order1.pendingQuantity, 0);
    EXPECT_EQ(order1.cancelledQuantity, 0);

    kitepp::order order2 = Orders[1];
    EXPECT_EQ(order2.accountID, "");
    EXPECT_EQ(order2.placedBy, "DA0017");
    EXPECT_EQ(order2.orderID, "171228000912853");
    EXPECT_EQ(order2.exchangeOrderID, "1300000002730006");
    EXPECT_EQ(order2.parentOrderID, "");
    EXPECT_EQ(order2.status, "COMPLETE");
    EXPECT_EQ(order2.statusMessage, "");
    EXPECT_EQ(order2.orderTimestamp, "2017-12-28 12:09:31");
    EXPECT_EQ(order2.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order2.exchangeTimestamp, "2017-12-28 12:00:28");
    EXPECT_EQ(order2.rejectedBy, "");
    EXPECT_EQ(order2.variety, "co");
    EXPECT_EQ(order2.exchange, "NSE");
    EXPECT_EQ(order2.tradingSymbol, "SBIN");
    EXPECT_EQ(order2.instrumentToken, 779521);
    EXPECT_EQ(order2.orderType, "LIMIT");
    EXPECT_EQ(order2.transactionType, "BUY");
    EXPECT_EQ(order2.validity, "DAY");
    EXPECT_EQ(order2.product, "CO");
    EXPECT_EQ(order2.quantity, 1);
    EXPECT_EQ(order2.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order2.price, 311);
    EXPECT_DOUBLE_EQ(order2.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order2.averagePrice, 311);
    EXPECT_EQ(order2.filledQuantity, 1);
    EXPECT_EQ(order2.pendingQuantity, 0);
    EXPECT_EQ(order2.cancelledQuantity, 0);

    kitepp::order order3 = Orders[2];
    EXPECT_EQ(order3.accountID, "");
    EXPECT_EQ(order3.placedBy, "DA0017");
    EXPECT_EQ(order3.orderID, "171228001116651");
    EXPECT_EQ(order3.exchangeOrderID, "211736200111089");
    EXPECT_EQ(order3.parentOrderID, "");
    EXPECT_EQ(order3.status, "COMPLETE");
    EXPECT_EQ(order3.statusMessage, "");
    EXPECT_EQ(order3.orderTimestamp, "2017-12-28 13:08:49");
    EXPECT_EQ(order3.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order3.exchangeTimestamp, "2017-12-28 13:08:49");
    EXPECT_EQ(order3.rejectedBy, "");
    EXPECT_EQ(order3.variety, "regular");
    EXPECT_EQ(order3.exchange, "MCX");
    EXPECT_EQ(order3.tradingSymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(order3.instrumentToken, 53505799);
    EXPECT_EQ(order3.orderType, "LIMIT");
    EXPECT_EQ(order3.transactionType, "BUY");
    EXPECT_EQ(order3.validity, "DAY");
    EXPECT_EQ(order3.product, "NRML");
    EXPECT_EQ(order3.quantity, 1);
    EXPECT_EQ(order3.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order3.price, 23388);
    EXPECT_DOUBLE_EQ(order3.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order3.averagePrice, 23388);
    EXPECT_EQ(order3.filledQuantity, 1);
    EXPECT_EQ(order3.pendingQuantity, 0);
    EXPECT_EQ(order3.cancelledQuantity, 0);

    kitepp::order order4 = Orders[3];
    EXPECT_EQ(order4.accountID, "");
    EXPECT_EQ(order4.placedBy, "DA0017");
    EXPECT_EQ(order4.orderID, "171228000912854");
    EXPECT_EQ(order4.exchangeOrderID, "1300000002730007");
    EXPECT_EQ(order4.parentOrderID, "171228000912853");
    EXPECT_EQ(order4.status, "COMPLETE");
    EXPECT_EQ(order4.statusMessage, "");
    EXPECT_EQ(order4.orderTimestamp, "2017-12-28 15:00:40");
    EXPECT_EQ(order4.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order4.exchangeTimestamp, "2017-12-28 15:00:40");
    EXPECT_EQ(order4.rejectedBy, "");
    EXPECT_EQ(order4.variety, "co");
    EXPECT_EQ(order4.exchange, "NSE");
    EXPECT_EQ(order4.tradingSymbol, "SBIN");
    EXPECT_EQ(order4.instrumentToken, 779521);
    EXPECT_EQ(order4.orderType, "LIMIT");
    EXPECT_EQ(order4.transactionType, "SELL");
    EXPECT_EQ(order4.validity, "DAY");
    EXPECT_EQ(order4.product, "CO");
    EXPECT_EQ(order4.quantity, 1);
    EXPECT_EQ(order4.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order4.price, 0);
    EXPECT_DOUBLE_EQ(order4.triggerPrice, 309);
    EXPECT_DOUBLE_EQ(order4.averagePrice, 309);
    EXPECT_EQ(order4.filledQuantity, 1);
    EXPECT_EQ(order4.pendingQuantity, 0);
    EXPECT_EQ(order4.cancelledQuantity, 0);

    kitepp::order order5 = Orders[4];
    EXPECT_EQ(order5.accountID, "");
    EXPECT_EQ(order5.placedBy, "DA0017");
    EXPECT_EQ(order5.orderID, "171228001686586");
    EXPECT_EQ(order5.exchangeOrderID, "211736200181323");
    EXPECT_EQ(order5.parentOrderID, "");
    EXPECT_EQ(order5.status, "COMPLETE");
    EXPECT_EQ(order5.statusMessage, "");
    EXPECT_EQ(order5.orderTimestamp, "2017-12-28 15:28:56");
    EXPECT_EQ(order5.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order5.exchangeTimestamp, "2017-12-28 15:28:56");
    EXPECT_EQ(order5.rejectedBy, "");
    EXPECT_EQ(order5.variety, "regular");
    EXPECT_EQ(order5.exchange, "MCX");
    EXPECT_EQ(order5.tradingSymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(order5.instrumentToken, 53505799);
    EXPECT_EQ(order5.orderType, "LIMIT");
    EXPECT_EQ(order5.transactionType, "SELL");
    EXPECT_EQ(order5.validity, "DAY");
    EXPECT_EQ(order5.product, "NRML");
    EXPECT_EQ(order5.quantity, 1);
    EXPECT_EQ(order5.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order5.price, 23349);
    EXPECT_DOUBLE_EQ(order5.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order5.averagePrice, 23349);
    EXPECT_EQ(order5.filledQuantity, 1);
    EXPECT_EQ(order5.pendingQuantity, 0);
    EXPECT_EQ(order5.cancelledQuantity, 0);

    kitepp::order order6 = Orders[5];
    EXPECT_EQ(order6.accountID, "");
    EXPECT_EQ(order6.placedBy, "DA0017");
    EXPECT_EQ(order6.orderID, "171228001730092");
    EXPECT_EQ(order6.exchangeOrderID, "211736200297236");
    EXPECT_EQ(order6.parentOrderID, "");
    EXPECT_EQ(order6.status, "COMPLETE");
    EXPECT_EQ(order6.statusMessage, "");
    EXPECT_EQ(order6.orderTimestamp, "2017-12-28 19:28:27");
    EXPECT_EQ(order6.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order6.exchangeTimestamp, "2017-12-28 19:28:27");
    EXPECT_EQ(order6.rejectedBy, "");
    EXPECT_EQ(order6.variety, "regular");
    EXPECT_EQ(order6.exchange, "MCX");
    EXPECT_EQ(order6.tradingSymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(order6.instrumentToken, 53496327);
    EXPECT_EQ(order6.orderType, "LIMIT");
    EXPECT_EQ(order6.transactionType, "BUY");
    EXPECT_EQ(order6.validity, "DAY");
    EXPECT_EQ(order6.product, "NRML");
    EXPECT_EQ(order6.quantity, 1);
    EXPECT_EQ(order6.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order6.price, 161.05);
    EXPECT_DOUBLE_EQ(order6.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order6.averagePrice, 161.05);
    EXPECT_EQ(order6.filledQuantity, 1);
    EXPECT_EQ(order6.pendingQuantity, 0);
    EXPECT_EQ(order6.cancelledQuantity, 0);

    kitepp::order order7 = Orders[6];
    EXPECT_EQ(order7.accountID, "");
    EXPECT_EQ(order7.placedBy, "DA0017");
    EXPECT_EQ(order7.orderID, "171228001731490");
    EXPECT_EQ(order7.exchangeOrderID, "211736200302177");
    EXPECT_EQ(order7.parentOrderID, "");
    EXPECT_EQ(order7.status, "COMPLETE");
    EXPECT_EQ(order7.statusMessage, "");
    EXPECT_EQ(order7.orderTimestamp, "2017-12-28 19:37:12");
    EXPECT_EQ(order7.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order7.exchangeTimestamp, "2017-12-28 19:37:12");
    EXPECT_EQ(order7.rejectedBy, "");
    EXPECT_EQ(order7.variety, "regular");
    EXPECT_EQ(order7.exchange, "MCX");
    EXPECT_EQ(order7.tradingSymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(order7.instrumentToken, 53496327);
    EXPECT_EQ(order7.orderType, "LIMIT");
    EXPECT_EQ(order7.transactionType, "SELL");
    EXPECT_EQ(order7.validity, "DAY");
    EXPECT_EQ(order7.product, "NRML");
    EXPECT_EQ(order7.quantity, 1);
    EXPECT_EQ(order7.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order7.price, 161.2);
    EXPECT_DOUBLE_EQ(order7.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order7.averagePrice, 161.2);
    EXPECT_EQ(order7.filledQuantity, 1);
    EXPECT_EQ(order7.pendingQuantity, 0);
    EXPECT_EQ(order7.cancelledQuantity, 0);
}

TEST(kiteTest, orderHistoryTest) {

    std::ifstream jsonFile("../../tests/mock_responses/orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kitepp::order> Orders = Kite.orderHistory("arg1");

    // Expected values
    ASSERT_THAT(Orders.size(), 7);

    kitepp::order order1 = Orders[0];
    EXPECT_EQ(order1.accountID, "");
    EXPECT_EQ(order1.placedBy, "DA0017");
    EXPECT_EQ(order1.orderID, "171228000850038");
    EXPECT_EQ(order1.exchangeOrderID, "211736200053802");
    EXPECT_EQ(order1.parentOrderID, "");
    EXPECT_EQ(order1.status, "COMPLETE");
    EXPECT_EQ(order1.statusMessage, "");
    EXPECT_EQ(order1.orderTimestamp, "2017-12-28 11:39:14");
    EXPECT_EQ(order1.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order1.exchangeTimestamp, "2017-12-28 11:39:14");
    EXPECT_EQ(order1.rejectedBy, "");
    EXPECT_EQ(order1.variety, "regular");
    EXPECT_EQ(order1.exchange, "MCX");
    EXPECT_EQ(order1.tradingSymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(order1.instrumentToken, 53505799);
    EXPECT_EQ(order1.orderType, "LIMIT");
    EXPECT_EQ(order1.transactionType, "SELL");
    EXPECT_EQ(order1.validity, "DAY");
    EXPECT_EQ(order1.product, "NRML");
    EXPECT_EQ(order1.quantity, 3);
    EXPECT_EQ(order1.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order1.price, 23337);
    EXPECT_DOUBLE_EQ(order1.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order1.averagePrice, 23337);
    EXPECT_EQ(order1.filledQuantity, 3);
    EXPECT_EQ(order1.pendingQuantity, 0);
    EXPECT_EQ(order1.cancelledQuantity, 0);

    kitepp::order order2 = Orders[1];
    EXPECT_EQ(order2.accountID, "");
    EXPECT_EQ(order2.placedBy, "DA0017");
    EXPECT_EQ(order2.orderID, "171228000912853");
    EXPECT_EQ(order2.exchangeOrderID, "1300000002730006");
    EXPECT_EQ(order2.parentOrderID, "");
    EXPECT_EQ(order2.status, "COMPLETE");
    EXPECT_EQ(order2.statusMessage, "");
    EXPECT_EQ(order2.orderTimestamp, "2017-12-28 12:09:31");
    EXPECT_EQ(order2.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order2.exchangeTimestamp, "2017-12-28 12:00:28");
    EXPECT_EQ(order2.rejectedBy, "");
    EXPECT_EQ(order2.variety, "co");
    EXPECT_EQ(order2.exchange, "NSE");
    EXPECT_EQ(order2.tradingSymbol, "SBIN");
    EXPECT_EQ(order2.instrumentToken, 779521);
    EXPECT_EQ(order2.orderType, "LIMIT");
    EXPECT_EQ(order2.transactionType, "BUY");
    EXPECT_EQ(order2.validity, "DAY");
    EXPECT_EQ(order2.product, "CO");
    EXPECT_EQ(order2.quantity, 1);
    EXPECT_EQ(order2.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order2.price, 311);
    EXPECT_DOUBLE_EQ(order2.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order2.averagePrice, 311);
    EXPECT_EQ(order2.filledQuantity, 1);
    EXPECT_EQ(order2.pendingQuantity, 0);
    EXPECT_EQ(order2.cancelledQuantity, 0);

    kitepp::order order3 = Orders[2];
    EXPECT_EQ(order3.accountID, "");
    EXPECT_EQ(order3.placedBy, "DA0017");
    EXPECT_EQ(order3.orderID, "171228001116651");
    EXPECT_EQ(order3.exchangeOrderID, "211736200111089");
    EXPECT_EQ(order3.parentOrderID, "");
    EXPECT_EQ(order3.status, "COMPLETE");
    EXPECT_EQ(order3.statusMessage, "");
    EXPECT_EQ(order3.orderTimestamp, "2017-12-28 13:08:49");
    EXPECT_EQ(order3.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order3.exchangeTimestamp, "2017-12-28 13:08:49");
    EXPECT_EQ(order3.rejectedBy, "");
    EXPECT_EQ(order3.variety, "regular");
    EXPECT_EQ(order3.exchange, "MCX");
    EXPECT_EQ(order3.tradingSymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(order3.instrumentToken, 53505799);
    EXPECT_EQ(order3.orderType, "LIMIT");
    EXPECT_EQ(order3.transactionType, "BUY");
    EXPECT_EQ(order3.validity, "DAY");
    EXPECT_EQ(order3.product, "NRML");
    EXPECT_EQ(order3.quantity, 1);
    EXPECT_EQ(order3.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order3.price, 23388);
    EXPECT_DOUBLE_EQ(order3.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order3.averagePrice, 23388);
    EXPECT_EQ(order3.filledQuantity, 1);
    EXPECT_EQ(order3.pendingQuantity, 0);
    EXPECT_EQ(order3.cancelledQuantity, 0);

    kitepp::order order4 = Orders[3];
    EXPECT_EQ(order4.accountID, "");
    EXPECT_EQ(order4.placedBy, "DA0017");
    EXPECT_EQ(order4.orderID, "171228000912854");
    EXPECT_EQ(order4.exchangeOrderID, "1300000002730007");
    EXPECT_EQ(order4.parentOrderID, "171228000912853");
    EXPECT_EQ(order4.status, "COMPLETE");
    EXPECT_EQ(order4.statusMessage, "");
    EXPECT_EQ(order4.orderTimestamp, "2017-12-28 15:00:40");
    EXPECT_EQ(order4.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order4.exchangeTimestamp, "2017-12-28 15:00:40");
    EXPECT_EQ(order4.rejectedBy, "");
    EXPECT_EQ(order4.variety, "co");
    EXPECT_EQ(order4.exchange, "NSE");
    EXPECT_EQ(order4.tradingSymbol, "SBIN");
    EXPECT_EQ(order4.instrumentToken, 779521);
    EXPECT_EQ(order4.orderType, "LIMIT");
    EXPECT_EQ(order4.transactionType, "SELL");
    EXPECT_EQ(order4.validity, "DAY");
    EXPECT_EQ(order4.product, "CO");
    EXPECT_EQ(order4.quantity, 1);
    EXPECT_EQ(order4.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order4.price, 0);
    EXPECT_DOUBLE_EQ(order4.triggerPrice, 309);
    EXPECT_DOUBLE_EQ(order4.averagePrice, 309);
    EXPECT_EQ(order4.filledQuantity, 1);
    EXPECT_EQ(order4.pendingQuantity, 0);
    EXPECT_EQ(order4.cancelledQuantity, 0);

    kitepp::order order5 = Orders[4];
    EXPECT_EQ(order5.accountID, "");
    EXPECT_EQ(order5.placedBy, "DA0017");
    EXPECT_EQ(order5.orderID, "171228001686586");
    EXPECT_EQ(order5.exchangeOrderID, "211736200181323");
    EXPECT_EQ(order5.parentOrderID, "");
    EXPECT_EQ(order5.status, "COMPLETE");
    EXPECT_EQ(order5.statusMessage, "");
    EXPECT_EQ(order5.orderTimestamp, "2017-12-28 15:28:56");
    EXPECT_EQ(order5.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order5.exchangeTimestamp, "2017-12-28 15:28:56");
    EXPECT_EQ(order5.rejectedBy, "");
    EXPECT_EQ(order5.variety, "regular");
    EXPECT_EQ(order5.exchange, "MCX");
    EXPECT_EQ(order5.tradingSymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(order5.instrumentToken, 53505799);
    EXPECT_EQ(order5.orderType, "LIMIT");
    EXPECT_EQ(order5.transactionType, "SELL");
    EXPECT_EQ(order5.validity, "DAY");
    EXPECT_EQ(order5.product, "NRML");
    EXPECT_EQ(order5.quantity, 1);
    EXPECT_EQ(order5.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order5.price, 23349);
    EXPECT_DOUBLE_EQ(order5.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order5.averagePrice, 23349);
    EXPECT_EQ(order5.filledQuantity, 1);
    EXPECT_EQ(order5.pendingQuantity, 0);
    EXPECT_EQ(order5.cancelledQuantity, 0);

    kitepp::order order6 = Orders[5];
    EXPECT_EQ(order6.accountID, "");
    EXPECT_EQ(order6.placedBy, "DA0017");
    EXPECT_EQ(order6.orderID, "171228001730092");
    EXPECT_EQ(order6.exchangeOrderID, "211736200297236");
    EXPECT_EQ(order6.parentOrderID, "");
    EXPECT_EQ(order6.status, "COMPLETE");
    EXPECT_EQ(order6.statusMessage, "");
    EXPECT_EQ(order6.orderTimestamp, "2017-12-28 19:28:27");
    EXPECT_EQ(order6.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order6.exchangeTimestamp, "2017-12-28 19:28:27");
    EXPECT_EQ(order6.rejectedBy, "");
    EXPECT_EQ(order6.variety, "regular");
    EXPECT_EQ(order6.exchange, "MCX");
    EXPECT_EQ(order6.tradingSymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(order6.instrumentToken, 53496327);
    EXPECT_EQ(order6.orderType, "LIMIT");
    EXPECT_EQ(order6.transactionType, "BUY");
    EXPECT_EQ(order6.validity, "DAY");
    EXPECT_EQ(order6.product, "NRML");
    EXPECT_EQ(order6.quantity, 1);
    EXPECT_EQ(order6.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order6.price, 161.05);
    EXPECT_DOUBLE_EQ(order6.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order6.averagePrice, 161.05);
    EXPECT_EQ(order6.filledQuantity, 1);
    EXPECT_EQ(order6.pendingQuantity, 0);
    EXPECT_EQ(order6.cancelledQuantity, 0);

    kitepp::order order7 = Orders[6];
    EXPECT_EQ(order7.accountID, "");
    EXPECT_EQ(order7.placedBy, "DA0017");
    EXPECT_EQ(order7.orderID, "171228001731490");
    EXPECT_EQ(order7.exchangeOrderID, "211736200302177");
    EXPECT_EQ(order7.parentOrderID, "");
    EXPECT_EQ(order7.status, "COMPLETE");
    EXPECT_EQ(order7.statusMessage, "");
    EXPECT_EQ(order7.orderTimestamp, "2017-12-28 19:37:12");
    EXPECT_EQ(order7.exchangeUpdateTimestamp, "");
    EXPECT_EQ(order7.exchangeTimestamp, "2017-12-28 19:37:12");
    EXPECT_EQ(order7.rejectedBy, "");
    EXPECT_EQ(order7.variety, "regular");
    EXPECT_EQ(order7.exchange, "MCX");
    EXPECT_EQ(order7.tradingSymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(order7.instrumentToken, 53496327);
    EXPECT_EQ(order7.orderType, "LIMIT");
    EXPECT_EQ(order7.transactionType, "SELL");
    EXPECT_EQ(order7.validity, "DAY");
    EXPECT_EQ(order7.product, "NRML");
    EXPECT_EQ(order7.quantity, 1);
    EXPECT_EQ(order7.disclosedQuantity, 0);
    EXPECT_DOUBLE_EQ(order7.price, 161.2);
    EXPECT_DOUBLE_EQ(order7.triggerPrice, 0);
    EXPECT_DOUBLE_EQ(order7.averagePrice, 161.2);
    EXPECT_EQ(order7.filledQuantity, 1);
    EXPECT_EQ(order7.pendingQuantity, 0);
    EXPECT_EQ(order7.cancelledQuantity, 0);
}

TEST(kiteTest, tradesTest) {

    std::ifstream jsonFile("../../tests/mock_responses/trades.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kitepp::trade> Trades = Kite.trades();

    // Expected values
    ASSERT_EQ(Trades.size(), 1);

    kitepp::trade trade1 = Trades[0];
    EXPECT_DOUBLE_EQ(trade1.averagePrice, 310.7);
    EXPECT_EQ(trade1.exchange, "NSE");
    EXPECT_EQ(trade1.exchangeOrderID, "1300000001887410");
    EXPECT_EQ(trade1.exchangeTimestamp, "2017-12-29 12:02:05");
    EXPECT_EQ(trade1.InstrumentToken, 779521);
    EXPECT_EQ(trade1.orderID, "171229000724687");
    EXPECT_EQ(trade1.product, "CNC");
    EXPECT_DOUBLE_EQ(trade1.quantity, 1);
    EXPECT_EQ(trade1.tradeID, "75894751");
    EXPECT_EQ(trade1.tradingSymbol, "SBIN");
    EXPECT_EQ(trade1.transactionType, "BUY");
}

TEST(kiteTest, orderTradesTest) {

    std::ifstream jsonFile("../../tests/mock_responses/order_trades.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kitepp::trade> Trades = Kite.orderTrades("arg1");

    // Expected values
    ASSERT_EQ(Trades.size(), 1);

    kitepp::trade trade1 = Trades[0];
    EXPECT_DOUBLE_EQ(trade1.averagePrice, 310.7);
    EXPECT_EQ(trade1.exchange, "NSE");
    EXPECT_EQ(trade1.exchangeOrderID, "1300000001887410");
    EXPECT_EQ(trade1.exchangeTimestamp, "2017-12-29 12:02:05");
    EXPECT_EQ(trade1.InstrumentToken, 779521);
    EXPECT_EQ(trade1.orderID, "171229000724687");
    EXPECT_EQ(trade1.product, "CNC");
    EXPECT_DOUBLE_EQ(trade1.quantity, 1);
    EXPECT_EQ(trade1.tradeID, "75894751");
    EXPECT_EQ(trade1.tradingSymbol, "SBIN");
    EXPECT_EQ(trade1.transactionType, "BUY");
}