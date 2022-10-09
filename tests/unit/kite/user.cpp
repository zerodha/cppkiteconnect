#include <string>
#include <utility>
#include <vector>

#include <gmock/gmock-actions.h>
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock-generated-function-mockers.h>
#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../kitepp.hpp"
#include "../utils.hpp"

using std::string;
using ::testing::ByMove;
using ::testing::Return;
using ::testing::StrictMock;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

TEST(kiteTest, profile) {
    const string JSON = kc::test::readFile("../tests/mock_responses/profile.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/user/profile" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    kc::userProfile profile = Kite.profile();

    // Expected values
    EXPECT_EQ(profile.userID, "AB1234");
    EXPECT_EQ(profile.userName, "AxAx Bxx");
    EXPECT_EQ(profile.userShortName, "abc");
    EXPECT_EQ(profile.avatarURL, "");
    EXPECT_EQ(profile.userType, "investor");
    EXPECT_EQ(profile.email, "xxxyyy@gmail.com");
    EXPECT_EQ(profile.broker, "ZERODHA");
    EXPECT_THAT(profile.products, ::testing::ElementsAre("BO", "CNC", "CO", "MIS", "NRML"));
    EXPECT_THAT(profile.orderTypes, ::testing::ElementsAre("LIMIT", "MARKET", "SL", "SL-M"));
    EXPECT_THAT(profile.exchanges, ::testing::ElementsAre("BSE", "BFO", "NFO", "MCX", "CDS", "NSE"));
    EXPECT_EQ(profile.meta.dematConsent, "physical");
};

TEST(kiteTest, getMarginsTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/margins.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/user/margins" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    kc::allMargins margins = Kite.getMargins();

    // Expected values
    EXPECT_EQ(margins.equity.enabled, true);
    EXPECT_DOUBLE_EQ(margins.equity.net, 99725.050000000017);

    EXPECT_DOUBLE_EQ(margins.equity.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.equity.available.cash, 245431.60000000001);
    EXPECT_DOUBLE_EQ(margins.equity.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.equity.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.equity.used.debits, 145706.54999999999);
    EXPECT_DOUBLE_EQ(margins.equity.used.exposure, 38981.25);
    EXPECT_DOUBLE_EQ(margins.equity.used.M2MRealised, 761.70000000000005);
    EXPECT_DOUBLE_EQ(margins.equity.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.span, 101989);
    EXPECT_DOUBLE_EQ(margins.equity.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.equity.used.turnover, 0);

    EXPECT_EQ(margins.commodity.enabled, true);
    EXPECT_DOUBLE_EQ(margins.commodity.net, 100661.7);

    EXPECT_DOUBLE_EQ(margins.commodity.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.available.cash, 100661.7);
    EXPECT_DOUBLE_EQ(margins.commodity.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.commodity.used.debits, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.exposure, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.M2MRealised, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.span, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.commodity.used.turnover, 0);
}

TEST(kiteTest, getMarginsSegmentTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/margins_equity.json");
    const string SEGMENT = "equity";

    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/user/margins/{0}" },
                          utils::http::Params {}, utils::FmtArgs { SEGMENT }))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    kc::margins margins = Kite.getMargins(SEGMENT);

    // Expected values
    EXPECT_EQ(margins.enabled, true);
    EXPECT_DOUBLE_EQ(margins.net, 99725.050000000017);

    EXPECT_DOUBLE_EQ(margins.available.adHocMargin, 0);
    EXPECT_DOUBLE_EQ(margins.available.cash, 245431.60000000001);
    EXPECT_DOUBLE_EQ(margins.available.collateral, 0);
    EXPECT_DOUBLE_EQ(margins.available.intradayPayin, 0);

    EXPECT_DOUBLE_EQ(margins.used.debits, 145706.54999999999);
    EXPECT_DOUBLE_EQ(margins.used.exposure, 38981.25);
    EXPECT_DOUBLE_EQ(margins.used.M2MRealised, 761.70000000000005);
    EXPECT_DOUBLE_EQ(margins.used.M2MUnrealised, 0);
    EXPECT_DOUBLE_EQ(margins.used.optionPremium, 0);
    EXPECT_DOUBLE_EQ(margins.used.payout, 0);
    EXPECT_DOUBLE_EQ(margins.used.span, 101989);
    EXPECT_DOUBLE_EQ(margins.used.holdingSales, 0);
    EXPECT_DOUBLE_EQ(margins.used.turnover, 0);
}