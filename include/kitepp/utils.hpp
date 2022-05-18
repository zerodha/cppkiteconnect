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
#pragma once

#include <string>
#include <vector>

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GENERATE_FLUENT_METHOD(returnType, fieldType, fieldName, methodName)                                           \
    returnType& methodName(fieldType arg) {                                                                            \
        (fieldName) = arg;                                                                                             \
        return *this;                                                                                                  \
    };

namespace kiteconnect {

using std::string;

inline std::vector<string> _split(const std::string& text, char sep) {

    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.emplace_back(text.substr(start, end - start));

        start = end + 1;
    };

    tokens.emplace_back(text.substr(start));

    return tokens;
};

// ! std::isnan should be used but doing so breaks the compilation on Windows
// ! utilizes a IEEE 754 property that states NaN != NaN, can fail because of compiler optimizations
// NOLINTNEXTLINE(clang-diagnostic-tautological-compare, misc-redundant-expression)
inline bool isValidArg(int value) { return value == value; };
// NOLINTNEXTLINE(clang-diagnostic-tautological-compare, misc-redundant-expression)
inline bool isValidArg(double value) { return value == value; };
inline bool isValidArg(const string& value) { return !value.empty(); };

} // namespace kiteconnect