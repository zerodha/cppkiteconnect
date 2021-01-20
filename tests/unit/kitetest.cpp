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

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, "/user/profile", _, _))
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

    EXPECT_CALL(Kite, _sendReq(_, kitepp::_methods::GET, "/user/margins", _, _))
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