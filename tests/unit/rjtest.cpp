/*
 *   Copyright (c) 2021 Bhumit Attarde

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <string>

#include <gtest/gtest.h>

#include "rapidjson/document.h"
#include "rapidjson/error/error.h"

// test if rapdjson is parsing correctly
TEST(RJTest, parseTest) {

    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    const std::string project_expected = "rapidjson";
    constexpr int stars_expected = 10;

    rapidjson::Document dom;
    rapidjson::ParseResult parseOK = dom.Parse(json);

    ASSERT_TRUE(parseOK);

    ASSERT_EQ(dom["project"].GetString(), project_expected);
    ASSERT_EQ(dom["stars"].GetInt(), stars_expected);
}