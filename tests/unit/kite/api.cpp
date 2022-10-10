/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2022 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a
 * copy of this software and associated  documentation files (the "Software"),
 * to deal in the Software  without restriction, including without  limitation
 * the rights to  use, copy,  modify, merge,  publish, distribute,  sublicense,
 * and/or  sell copies  of  the Software,  and  to  permit persons  to  whom the
 * Software  is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS
 * OR IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN
 * NO EVENT  SHALL THE AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY
 * CLAIM,  DAMAGES OR  OTHER LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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

TEST(kiteTest, constructorTest) {
    StrictMock<kc::test::mockKite> Kite;
    const string API_KEY = "Uz7Mdn29ZGya31a";

    EXPECT_EQ(Kite.getApiKey(), API_KEY);
};

TEST(kiteTest, loginURLTest) {
    StrictMock<kc::test::mockKite> Kite;
    const string LOGIN_URL =
        "https://kite.zerodha.com/connect/login?v=3&api_key=Uz7Mdn29ZGya31a";

    EXPECT_EQ(Kite.loginURL(), LOGIN_URL);
};

TEST(kiteTest, generateSessionTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_custom/generate_session.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::POST,
                                  "/session/token" },
                          utils::http::Params {
                              { "api_key", "Uz7Mdn29ZGya31a" },
                              { "request_token", "qKLeSUycwFEvWGw" },
                              { "checksum", "ac90aa6cafb2bab90a172d38f70c66cbc1"
                                            "d96601852123530459fcabbc487d4f" },
                          },
                          utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    kc::userSession session =
        Kite.generateSession(kc::test::REQUEST_TOKEN, kc::test::API_SECRET);

    // Expected values
    EXPECT_EQ(session.profile.userID, "XX000");
    EXPECT_EQ(session.profile.userName, "Kite Connect");
    EXPECT_EQ(session.profile.userShortName, "Kite");
    EXPECT_EQ(session.profile.avatarURL, "");
    EXPECT_EQ(session.profile.userType, "investor");
    EXPECT_EQ(session.profile.email, "kite@kite.trade");
    EXPECT_EQ(session.profile.broker, "ZERODHA");
    EXPECT_THAT(session.profile.products,
        ::testing::ElementsAre("BO", "CNC", "CO", "MIS", "NRML"));
    EXPECT_THAT(session.profile.orderTypes,
        ::testing::ElementsAre("LIMIT", "MARKET", "SL", "SL-M"));
    EXPECT_THAT(session.profile.exchanges,
        ::testing::ElementsAre("MCX", "BSE", "NSE", "BFO", "NFO", "CDS"));

    EXPECT_EQ(session.tokens.userID, "XX000");
    EXPECT_EQ(session.tokens.accessToken, "yyyyyy");
    EXPECT_EQ(session.tokens.refreshToken, "");

    EXPECT_EQ(session.apiKey, "xxxxxx");
    EXPECT_EQ(session.publicToken, "zzzzzz");
    EXPECT_EQ(session.loginTime, "2018-01-01 16:15:14");
};

TEST(kiteTest, invalidateSessionTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_custom/invalidate_session.json");
    StrictMock<kc::test::mockKite> Kite;
    Kite.setAccessToken(kc::test::ACCESS_TOKEN);

    EXPECT_CALL(
        Kite, sendReq(utils::http::endpoint { utils::http::METHOD::DEL,
                          "/session/token?api_key={0}&access_token={1}" },
                  utils::http::Params {},
                  utils::FmtArgs { kc::test::API_KEY, kc::test::ACCESS_TOKEN }))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const bool RESULT = Kite.invalidateSession();
    EXPECT_EQ(RESULT, true);
};
