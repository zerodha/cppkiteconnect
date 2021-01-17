#include <string>

#include "kitepp.hpp"
#include "gtest/gtest.h"

#include "rapidjson/document.h"
#include "rapidjson/error/error.h"

using std::string;

TEST(RJTest, parseTest)
{
    const char *json = "{\"project\":\"rapidjson\",\"stars\":10}";
    const string project_expected = "rapidjson";
    constexpr int stars_expected = 10;

    rapidjson::Document dom;
    rapidjson::ParseResult parseOK = dom.Parse(json);

    ASSERT_TRUE(parseOK);

    ASSERT_EQ(dom["project"].GetString(), project_expected);
    ASSERT_EQ(dom["stars"].GetInt(), stars_expected);
}