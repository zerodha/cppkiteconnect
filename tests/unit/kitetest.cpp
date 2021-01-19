#include <fstream>
#include <gmock/gmock-actions.h>
#include <gmock/gmock-more-actions.h>
#include <ios>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "kitepp.hpp"
#include "kitepp/kite.hpp"
#include "kitepp/responses.hpp"
#include "rapidjson/document.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::string;
using ::testing::_;
namespace rj = rapidjson;

class mockKite : public kitepp::kite
{

  public:
    mockKite() : kite("apiKey123"){};

    MOCK_METHOD(void, _sendReq,
                (rj::Document & data, const kitepp::_methods &mtd, const string &endpoint,
                 (const std::vector<std::pair<string, string>>)&bodyParams, bool isJson),
                (override));
};

TEST(kiteTest, constructorTest)
{
    mockKite Kite;
    const string APIKey_expected = "apiKey123";

    EXPECT_EQ(Kite.getAPIKey(), APIKey_expected);
};

TEST(kiteTest, loginURLTest)
{
    mockKite Kite;
    const string loginURL_expected = "https://kite.zerodha.com/connect/login?v=3&api_key=apiKey123";

    EXPECT_EQ(Kite.loginURL(), loginURL_expected);
};

TEST(kiteTest, generateSessionTest)
{

    std::ifstream jsonFile("../../tests/mock_responses/generate_session.json");
    ASSERT_TRUE(jsonFile);

    string json(std::istreambuf_iterator<char>{jsonFile}, {});
    ASSERT_FALSE(json.empty());

    mockKite Kite;
    EXPECT_CALL(Kite, _sendReq(_, _, _, _, _))
        .WillOnce(testing::Invoke([&json](rj::Document &data, const kitepp::_methods &mtd, const string &endpoint,
                                          const std::vector<std::pair<string, string>> &bodyParams = {},
                                          bool isJson = false) { data.Parse(json.c_str()); }));

    kitepp::userSession session = Kite.generateSession("reqtoken", "apisecret");

    EXPECT_EQ(session.apiKey, "xxxxxx");
}