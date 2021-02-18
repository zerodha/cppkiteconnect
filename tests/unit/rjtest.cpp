/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
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