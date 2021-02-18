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

#include <exception>
#include <string>
#include <utility>

namespace kiteconnect {

using std::string;

/**
 * @brief kitepp saves you the hassle of detecting API errors by looking at HTTP codes or JSON error responses. Instead,
 * it raises aptly named exceptions that you can catch. `kiteppException` is derived from `std::exception` and, along
 * with other useful methods like `code()`, `message()`, provides `what()` method. You should catch exceptions of this
 * class. All kitepp exceptions barring `libException` are derived from this class. `kiteppException` being a abstract
 * class, you cannot create it direectly. Instead, use exceptions provided in `kiteexceptions.hpp` file.
 * @attention libExcpetion isn't derived from this class. You'll either have to catch it directly or catch
 * `std::exception`.
 *
 * @paragraph ex1 example
 * @snippet example2.cpp dealing with kitepp exceptions
 */
class kiteppException : public std::exception {

  public:
    // constructors and destructor

    kiteppException(int c, string msg): _code(c), _message(std::move(msg)) {};

    // methods

    /**
     * @brief classic `what()` method inherited from `std::exception`
     *
     * @return const char*
     */
    const char* what() const noexcept override = 0;

    /**
     * @brief returns HTTP code sent by REST API
     *
     * @return int
     */
    int code() const noexcept { return _code; };

    /**
     * @brief returns message sent by REST API
     *
     * @return const char*
     */
    const char* message() const noexcept { return _message.c_str(); };

  private:
    int _code = 0;
    string _message = "";
};

} // namespace kiteconnect