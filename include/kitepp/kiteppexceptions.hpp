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
#include <utility>

#include "kiteppexception.hpp"

namespace kiteconnect {

using kiteconnect::kiteppException;
using std::string;

// exceptions

/**
 * @brief Represents all token and authentication related errors.
 *
 */
class tokenException : public kiteppException {

  public:
    // constructors and destructor

    tokenException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "TokenException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief Represents user account related errors
 *
 */
class userException : public kiteppException {

  public:
    // constructors and destructor

    userException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "UserException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief Represents all order placement and manipulation errors.
 *
 */
class orderException : public kiteppException {

  public:
    // constructors and destructor

    orderException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "OrderException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief Represents user input errors such as missing and invalid parameters.
 *
 */
class inputException : public kiteppException {

  public:
    // constructors and destructor

    inputException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "InputException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief Represents a network issue between Kite and the backend Order Management System (OMS).
 *
 */
class networkException : public kiteppException {

  public:
    // constructors and destructor

    networkException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "NetworkException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief Represents a bad response from the backend Order Management System
 *
 */
class dataException : public kiteppException {

  public:
    // constructors and destructor

    dataException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "DataException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief An unclassified, general error.
 *
 */
class generalException : public kiteppException {

  public:
    // constructors and destructor

    generalException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "GeneralException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief Represents permission denied exceptions for certain calls.
 *
 */
class permissionException : public kiteppException {

  public:
    // constructors and destructor

    permissionException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "PermissionException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief This exception is thrown when REST API doesn't return a `HTTP OK 200` code and any exception string.
 *
 */
class noException : public kiteppException {

  public:
    // constructors and destructor

    noException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "No exception was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};

/**
 * @brief This exception is thrown when some error occures while processing data on library level. Serves as the
 * placeholder exception for all library related errors.
 *
 */
class libException : public std::exception {

  public:
    // constructors and destructor

    explicit libException(string msg): _message(std::move(msg)) {};

    // methods

    const char* what() { return _message.c_str(); };

  private:
    string _message = "";
};

// throw exception wrt string passed. Throws libException if string doesn't match with anything.

inline void _throwException(const string& excpStr, int code, const string& msg) {

    // exception strings sent by API
    static const string tokenExcpStr = "TokenException";
    static const string userExcpStr = "UserException";
    static const string orderExcpStr = "OrderException";
    static const string inputExcpStr = "InputException";
    static const string networkExcpStr = "NetworkException";
    static const string dataExcpStr = "DataException";
    static const string generalExcpStr = "GeneralException";
    static const string permissionExcpStr = "PermissionException";
    static const string noExcpStr = "NoException"; // when REST API doesn't return any exception

    if (excpStr == tokenExcpStr) { throw tokenException(code, msg); };
    if (excpStr == userExcpStr) { throw userException(code, msg); };
    if (excpStr == orderExcpStr) { throw orderException(code, msg); };
    if (excpStr == inputExcpStr) { throw inputException(code, msg); };
    if (excpStr == networkExcpStr) { throw networkException(code, msg); };
    if (excpStr == dataExcpStr) { throw dataException(code, msg); };
    if (excpStr == generalExcpStr) { throw generalException(code, msg); };
    if (excpStr == permissionExcpStr) { throw permissionException(code, msg); };
    if (excpStr == noExcpStr) { throw noException(code, msg); };

    throw libException("Unknown exception was thrown by REST API");
};

} // namespace kiteconnect