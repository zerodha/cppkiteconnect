#include <fstream>
#include <ios>
#include <iterator>
#include <string>

#include "kitepp.hpp"
#include "kitepp/kite.hpp"
#include "gtest/gtest.h"

using std::string;

class kiteTest : public ::testing::Test
{
  public:
    kiteTest() : Kite("apiKey123"){};

  protected:
    kitepp::kite Kite;
};

TEST_F(kiteTest, constructorTest)
{
    const string APIKey_expected = "apiKey123";

    EXPECT_EQ(Kite.getAPIKey(), APIKey_expected);
};

TEST_F(kiteTest, loginURLTest)
{
    const string loginURL_expected = "https://kite.zerodha.com/connect/login?v=3&api_key=apiKey123";

    EXPECT_EQ(Kite.loginURL(), loginURL_expected);
};

TEST_F(kiteTest, generateSessionTest)
{
    std::ifstream jsonFile("../mock_responses/generate_session.json");
    ASSERT_TRUE(jsonFile);

    // read the entire file into string
    string json(std::istreambuf_iterator<char>{jsonFile}, {});
};
