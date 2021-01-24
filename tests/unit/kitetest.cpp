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

TEST(kiteTest, placeGTTTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_place_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::POST, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    int orderID = Kite.placeGTT("arg1", "arg2", "arg3", {}, 0.0, {});

    // Expected value
    EXPECT_EQ(orderID, 123);
}

TEST(kiteTest, getGTTsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_get_orders.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kitepp::GTT> Orders = Kite.getGTTs();

    // Exptected values
    ASSERT_EQ(Orders.size(), 2);

    kitepp::GTT order1 = Orders[0];
    EXPECT_EQ(order1.ID, 112127);
    EXPECT_EQ(order1.userID, "XX0000");
    EXPECT_EQ(order1.type, "single");
    EXPECT_EQ(order1.createdAt, "2019-09-12 13:25:16");
    EXPECT_EQ(order1.updatedAt, "2019-09-12 13:25:16");
    EXPECT_EQ(order1.expiresAt, "2020-09-12 13:25:16");
    EXPECT_EQ(order1.status, "active");
    EXPECT_EQ(order1.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order1.condition.lastPrice, 798);
    EXPECT_EQ(order1.condition.tradingsymbol, "INFY");
    EXPECT_DOUBLE_EQ(order1.condition.triggerValues[0], 702);
    EXPECT_EQ(order1.orders[0].exchange, "NSE");
    EXPECT_EQ(order1.orders[0].tradingSymbol, "INFY");
    EXPECT_EQ(order1.orders[0].product, "CNC");
    EXPECT_EQ(order1.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order1.orders[0].transactionType, "BUY");
    EXPECT_EQ(order1.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order1.orders[0].price, 702.5);

    kitepp::GTT order2 = Orders[1];
    EXPECT_EQ(order2.ID, 105099);
    EXPECT_EQ(order2.userID, "XX0000");
    EXPECT_EQ(order2.type, "two-leg");
    EXPECT_EQ(order2.createdAt, "2019-09-09 15:13:22");
    EXPECT_EQ(order2.updatedAt, "2019-09-09 15:15:08");
    EXPECT_EQ(order2.expiresAt, "2020-01-01 12:00:00");
    EXPECT_EQ(order2.status, "triggered");
    EXPECT_EQ(order2.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order2.condition.lastPrice, 102.6);
    EXPECT_EQ(order2.condition.tradingsymbol, "RAIN");
    EXPECT_THAT(order2.condition.triggerValues, ::testing::ElementsAre(102.0, 103.7));
    EXPECT_EQ(order2.orders[0].tradingSymbol, "RAIN");
    EXPECT_EQ(order2.orders[0].product, "CNC");
    EXPECT_EQ(order2.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order2.orders[0].transactionType, "SELL");
    EXPECT_EQ(order2.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order2.orders[0].price, 1);
    EXPECT_EQ(order2.orders[1].tradingSymbol, "RAIN");
    EXPECT_EQ(order2.orders[1].product, "CNC");
    EXPECT_EQ(order2.orders[1].orderType, "LIMIT");
    EXPECT_EQ(order2.orders[1].transactionType, "SELL");
    EXPECT_EQ(order2.orders[1].quantity, 1);
    EXPECT_DOUBLE_EQ(order2.orders[1].price, 1);
}

TEST(kiteTest, getGTTTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_get_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kitepp::GTT order = Kite.getGTT(0);

    // Expected values
    EXPECT_EQ(order.ID, 123);
    EXPECT_EQ(order.userID, "XX0000");
    EXPECT_EQ(order.type, "two-leg");
    EXPECT_EQ(order.createdAt, "2019-09-09 15:13:22");
    EXPECT_EQ(order.updatedAt, "2019-09-09 15:15:08");
    EXPECT_EQ(order.expiresAt, "2020-01-01 12:00:00");
    EXPECT_EQ(order.status, "triggered");
    EXPECT_EQ(order.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order.condition.lastPrice, 102.6);
    EXPECT_EQ(order.condition.tradingsymbol, "RAIN");
    EXPECT_THAT(order.condition.triggerValues, ::testing::ElementsAre(102.0, 103.7));
    EXPECT_EQ(order.orders[0].tradingSymbol, "RAIN");
    EXPECT_EQ(order.orders[0].product, "CNC");
    EXPECT_EQ(order.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order.orders[0].transactionType, "SELL");
    EXPECT_EQ(order.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order.orders[0].price, 1);
}

TEST(kiteTest, modifyGTTTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_modify_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    int orderID = Kite.modifyGTT(0, "arg1", "arg2", "arg3", {}, 0, {});

    // Expected values
    EXPECT_EQ(orderID, 123);
}

TEST(kiteTest, deleteGTTTest) {

    std::ifstream jsonFile("../../tests/mock_responses/gtt_delete_order.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::DEL, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    int orderID = Kite.deleteGTT(0);

    // Expected values
    EXPECT_EQ(orderID, 123);
}

TEST(kiteTest, holdingsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/holdings.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    std::vector<kitepp::holding> Holdings = Kite.holdings();

    // Expected values
    ASSERT_EQ(Holdings.size(), 3);

    kitepp::holding holding1 = Holdings[0];
    EXPECT_EQ(holding1.tradingsymbol, "BENGALASM");
    EXPECT_EQ(holding1.exchange, "BSE");
    EXPECT_EQ(holding1.instrumentToken, 136472324);
    EXPECT_EQ(holding1.ISIN, "INE083K01017");
    EXPECT_EQ(holding1.product, "CNC");
    EXPECT_DOUBLE_EQ(holding1.price, 0);
    EXPECT_EQ(holding1.quantity, 1);
    EXPECT_EQ(holding1.t1Quantity, 0);
    EXPECT_EQ(holding1.realisedQuantity, 1);
    EXPECT_EQ(holding1.collateralQuantity, 0);
    EXPECT_EQ(holding1.collateralType, "");
    EXPECT_DOUBLE_EQ(holding1.averagePrice, 1150);
    EXPECT_DOUBLE_EQ(holding1.lastPrice, 2620);
    EXPECT_DOUBLE_EQ(holding1.closePrice, 2751.1);
    EXPECT_DOUBLE_EQ(holding1.PnL, 1470);
    EXPECT_DOUBLE_EQ(holding1.dayChange, -131.0999999999999);
    EXPECT_DOUBLE_EQ(holding1.dayChangePercentage, -4.7653665806404675);

    kitepp::holding holding2 = Holdings[1];
    EXPECT_EQ(holding2.tradingsymbol, "CONFIPET");
    EXPECT_EQ(holding2.exchange, "BSE");
    EXPECT_EQ(holding2.instrumentToken, 134868228);
    EXPECT_EQ(holding2.ISIN, "INE552D01024");
    EXPECT_EQ(holding2.product, "CNC");
    EXPECT_DOUBLE_EQ(holding2.price, 0);
    EXPECT_EQ(holding2.quantity, 1);
    EXPECT_EQ(holding2.t1Quantity, 0);
    EXPECT_EQ(holding2.realisedQuantity, 1);
    EXPECT_EQ(holding2.collateralQuantity, 0);
    EXPECT_EQ(holding2.collateralType, "");
    EXPECT_DOUBLE_EQ(holding2.averagePrice, 5.89);
    EXPECT_DOUBLE_EQ(holding2.lastPrice, 31.35);
    EXPECT_DOUBLE_EQ(holding2.closePrice, 31.5);
    EXPECT_DOUBLE_EQ(holding2.PnL, 25.46);
    EXPECT_DOUBLE_EQ(holding2.dayChange, -0.14999999999999858);
    EXPECT_DOUBLE_EQ(holding2.dayChangePercentage, -0.4761904761904716);

    kitepp::holding holding3 = Holdings[2];
    EXPECT_EQ(holding3.tradingsymbol, "IPOWER");
    EXPECT_EQ(holding3.exchange, "BSE");
    EXPECT_EQ(holding3.instrumentToken, 131175684);
    EXPECT_EQ(holding3.ISIN, "INE468F01010");
    EXPECT_EQ(holding3.product, "CNC");
    EXPECT_DOUBLE_EQ(holding3.price, 0);
    EXPECT_EQ(holding3.quantity, 1);
    EXPECT_EQ(holding3.t1Quantity, 0);
    EXPECT_EQ(holding3.realisedQuantity, 1);
    EXPECT_EQ(holding3.collateralQuantity, 0);
    EXPECT_EQ(holding3.collateralType, "");
    EXPECT_DOUBLE_EQ(holding3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(holding3.lastPrice, 1.95);
    EXPECT_DOUBLE_EQ(holding3.closePrice, 0);
    EXPECT_DOUBLE_EQ(holding3.PnL, 1.95);
    EXPECT_DOUBLE_EQ(holding3.dayChange, 0);
    EXPECT_DOUBLE_EQ(holding3.dayChangePercentage, 0);
}

TEST(kiteTest, getPositionsTest) {

    std::ifstream jsonFile("../../tests/mock_responses/positions.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    kitepp::positions Positions = Kite.getPositions();

    // Expected values
    ASSERT_EQ(Positions.net.size(), 3);
    ASSERT_EQ(Positions.day.size(), 3);

    kitepp::position netPosition1 = Positions.net[0];
    EXPECT_EQ(netPosition1.tradingsymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(netPosition1.exchange, "MCX");
    EXPECT_EQ(netPosition1.instrumentToken, 53496327);
    EXPECT_EQ(netPosition1.product, "NRML");
    EXPECT_EQ(netPosition1.quantity, 1);
    EXPECT_EQ(netPosition1.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.multiplier, 1000);
    EXPECT_DOUBLE_EQ(netPosition1.averagePrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.closePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.lastPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.value, -161050);
    EXPECT_DOUBLE_EQ(netPosition1.PnL, 0);
    EXPECT_DOUBLE_EQ(netPosition1.M2M, 0);
    EXPECT_DOUBLE_EQ(netPosition1.unrealised, 0);
    EXPECT_DOUBLE_EQ(netPosition1.realised, 0);
    EXPECT_EQ(netPosition1.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition1.buyPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.buyValue, 161050);
    EXPECT_DOUBLE_EQ(netPosition1.buyM2MValue, 161050);
    EXPECT_EQ(netPosition1.sellQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellPrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellValue, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellM2MValue, 0);
    EXPECT_EQ(netPosition1.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition1.dayBuyPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.dayBuyValue, 161050);
    EXPECT_EQ(netPosition1.daySellQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.daySellPrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.daySellValue, 0);

    kitepp::position netPosition2 = Positions.net[1];
    EXPECT_EQ(netPosition2.tradingsymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(netPosition2.exchange, "MCX");
    EXPECT_EQ(netPosition2.instrumentToken, 53505799);
    EXPECT_EQ(netPosition2.product, "NRML");
    EXPECT_EQ(netPosition2.quantity, 0);
    EXPECT_EQ(netPosition2.overnightQuantity, 3);
    EXPECT_DOUBLE_EQ(netPosition2.multiplier, 1);
    EXPECT_DOUBLE_EQ(netPosition2.averagePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition2.closePrice, 23232);
    EXPECT_DOUBLE_EQ(netPosition2.lastPrice, 23355);
    EXPECT_DOUBLE_EQ(netPosition2.value, 801);
    EXPECT_DOUBLE_EQ(netPosition2.PnL, 801);
    EXPECT_DOUBLE_EQ(netPosition2.M2M, 276);
    EXPECT_DOUBLE_EQ(netPosition2.unrealised, 801);
    EXPECT_DOUBLE_EQ(netPosition2.realised, 0);
    EXPECT_EQ(netPosition2.buyQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.buyPrice, 23139.75);
    EXPECT_DOUBLE_EQ(netPosition2.buyValue, 92559);
    EXPECT_DOUBLE_EQ(netPosition2.buyM2MValue, 93084);
    EXPECT_EQ(netPosition2.sellQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.sellPrice, 23340);
    EXPECT_DOUBLE_EQ(netPosition2.sellValue, 93360);
    EXPECT_DOUBLE_EQ(netPosition2.sellM2MValue, 93360);
    EXPECT_EQ(netPosition2.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition2.dayBuyPrice, 23388);
    EXPECT_DOUBLE_EQ(netPosition2.dayBuyValue, 23388);
    EXPECT_EQ(netPosition2.daySellQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.daySellPrice, 23340);
    EXPECT_DOUBLE_EQ(netPosition2.daySellValue, 93360);

    kitepp::position netPosition3 = Positions.net[2];
    EXPECT_EQ(netPosition3.tradingsymbol, "SBIN");
    EXPECT_EQ(netPosition3.exchange, "NSE");
    EXPECT_EQ(netPosition3.instrumentToken, 779521);
    EXPECT_EQ(netPosition3.product, "CO");
    EXPECT_EQ(netPosition3.quantity, 0);
    EXPECT_EQ(netPosition3.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition3.multiplier, 1);
    EXPECT_DOUBLE_EQ(netPosition3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition3.closePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition3.lastPrice, 308.4);
    EXPECT_DOUBLE_EQ(netPosition3.value, -2);
    EXPECT_DOUBLE_EQ(netPosition3.PnL, -2);
    EXPECT_DOUBLE_EQ(netPosition3.M2M, -2);
    EXPECT_DOUBLE_EQ(netPosition3.unrealised, -2);
    EXPECT_DOUBLE_EQ(netPosition3.realised, 0);
    EXPECT_EQ(netPosition3.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.buyPrice, 311);
    EXPECT_DOUBLE_EQ(netPosition3.buyValue, 311);
    EXPECT_DOUBLE_EQ(netPosition3.buyM2MValue, 311);
    EXPECT_EQ(netPosition3.sellQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.sellPrice, 309);
    EXPECT_DOUBLE_EQ(netPosition3.sellValue, 309);
    EXPECT_DOUBLE_EQ(netPosition3.sellM2MValue, 309);
    EXPECT_EQ(netPosition3.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.dayBuyPrice, 311);
    EXPECT_DOUBLE_EQ(netPosition3.dayBuyValue, 311);
    EXPECT_EQ(netPosition3.daySellQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.daySellPrice, 309);
    EXPECT_DOUBLE_EQ(netPosition3.daySellValue, 309);

    kitepp::position dayPosition1 = Positions.day[0];
    EXPECT_EQ(dayPosition1.tradingsymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(dayPosition1.exchange, "MCX");
    EXPECT_EQ(dayPosition1.instrumentToken, 53505799);
    EXPECT_EQ(dayPosition1.product, "NRML");
    EXPECT_EQ(dayPosition1.quantity, -3);
    EXPECT_EQ(dayPosition1.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition1.multiplier, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.averagePrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.closePrice, 23232);
    EXPECT_DOUBLE_EQ(dayPosition1.lastPrice, 23355);
    EXPECT_DOUBLE_EQ(dayPosition1.value, 69972);
    EXPECT_DOUBLE_EQ(dayPosition1.PnL, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.M2M, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.unrealised, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.realised, 0);
    EXPECT_EQ(dayPosition1.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.buyPrice, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.buyValue, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.buyM2MValue, 23388);
    EXPECT_EQ(dayPosition1.sellQuantity, 4);
    EXPECT_DOUBLE_EQ(dayPosition1.sellPrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.sellValue, 93360);
    EXPECT_DOUBLE_EQ(dayPosition1.sellM2MValue, 93360);
    EXPECT_EQ(dayPosition1.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.dayBuyPrice, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.dayBuyValue, 23388);
    EXPECT_EQ(dayPosition1.daySellQuantity, 4);
    EXPECT_DOUBLE_EQ(dayPosition1.daySellPrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.daySellValue, 93360);

    kitepp::position dayPosition2 = Positions.day[1];
    EXPECT_EQ(dayPosition2.tradingsymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(dayPosition2.exchange, "MCX");
    EXPECT_EQ(dayPosition2.instrumentToken, 53496327);
    EXPECT_EQ(dayPosition2.product, "NRML");
    EXPECT_EQ(dayPosition2.quantity, 1);
    EXPECT_EQ(dayPosition2.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.multiplier, 1000);
    EXPECT_DOUBLE_EQ(dayPosition2.averagePrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.closePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.lastPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.value, -161050);
    EXPECT_DOUBLE_EQ(dayPosition2.PnL, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.M2M, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.unrealised, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.realised, 0);
    EXPECT_EQ(dayPosition2.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition2.buyPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.buyValue, 161050);
    EXPECT_DOUBLE_EQ(dayPosition2.buyM2MValue, 161050);
    EXPECT_EQ(dayPosition2.sellQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellPrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellValue, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellM2MValue, 0);
    EXPECT_EQ(dayPosition2.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition2.dayBuyPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.dayBuyValue, 161050);
    EXPECT_EQ(dayPosition2.daySellQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.daySellPrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.daySellValue, 0);

    kitepp::position dayPosition3 = Positions.day[2];
    EXPECT_EQ(dayPosition3.tradingsymbol, "SBIN");
    EXPECT_EQ(dayPosition3.exchange, "NSE");
    EXPECT_EQ(dayPosition3.instrumentToken, 779521);
    EXPECT_EQ(dayPosition3.product, "CO");
    EXPECT_EQ(dayPosition3.quantity, 0);
    EXPECT_EQ(dayPosition3.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.multiplier, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.closePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.lastPrice, 308.4);
    EXPECT_DOUBLE_EQ(dayPosition3.value, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.PnL, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.M2M, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.unrealised, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.realised, 0);
    EXPECT_EQ(dayPosition3.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.buyPrice, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.buyValue, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.buyM2MValue, 311);
    EXPECT_EQ(dayPosition3.sellQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.sellPrice, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.sellValue, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.sellM2MValue, 309);
    EXPECT_EQ(dayPosition3.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.dayBuyPrice, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.dayBuyValue, 311);
    EXPECT_EQ(dayPosition3.daySellQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.daySellPrice, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.daySellValue, 309);
}

TEST(kiteTest, convertPositionTest) {

    std::ifstream jsonFile("../../tests/mock_responses/convert_position.json");
    ASSERT_TRUE(jsonFile);
    rj::IStreamWrapper jsonFWrap(jsonFile);

    mockKite Kite;

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::PUT, _, _, _))
        .WillOnce(testing::Invoke([&jsonFWrap](rj::Document& data, const kitepp::_methods& mtd, const string& endpoint,
                                      const std::vector<std::pair<string, string>>& bodyParams = {},
                                      bool isJson = false) { data.ParseStream(jsonFWrap); }));

    bool result = Kite.convertPosition("arg1", "arg2", "arg3", "arg4", 0, "arg5", "arg6");

    // Expected values
    EXPECT_EQ(result, true);
}