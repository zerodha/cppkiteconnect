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
    kiteppException(int Code_, string Message_): Code(Code_), Message(std::move(Message_)) {};

    /**
     * @brief classic `what()` method inherited from `std::exception`
     *
     * @return const char*
     */
    [[nodiscard]] const char* what() const noexcept override = 0;

    /**
     * @brief returns HTTP code sent by REST API
     *
     * @return int
     */
    [[nodiscard]] int code() const noexcept { return Code; };

    /**
     * @brief returns message sent by REST API
     *
     * @return const char*
     */
    [[nodiscard]] const char* message() const noexcept { return Message.c_str(); };

  private:
    int Code = 0;
    string Message;
};

/**
 * @brief Represents all token and authentication related errors.
 *
 */
class tokenException : public kiteppException {

  public:
    tokenException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "TokenException was thrown by Kite"; };
};

/**
 * @brief Represents user account related errors
 *
 */
class userException : public kiteppException {

  public:
    userException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "UserException was thrown by Kite"; };
};

/**
 * @brief Represents all order placement and manipulation errors.
 *
 */
class orderException : public kiteppException {

  public:
    orderException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "OrderException was thrown by Kite"; };
};

/**
 * @brief Represents user input errors such as missing and invalid parameters.
 *
 */
class inputException : public kiteppException {

  public:
    inputException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "InputException was thrown by Kite"; };
};

/**
 * @brief Represents a network issue between Kite and the backend Order Management System (OMS).
 *
 */
class networkException : public kiteppException {

  public:
    networkException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "NetworkException was thrown by Kite"; };
};

/**
 * @brief Represents a bad response from the backend Order Management System
 *
 */
class dataException : public kiteppException {

  public:
    dataException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "DataException was thrown by Kite"; };
};

/**
 * @brief An unclassified, general error.
 *
 */
class generalException : public kiteppException {

  public:
    generalException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "GeneralException was thrown by Kite"; };
};

/**
 * @brief Represents permission denied exceptions for certain calls.
 *
 */
class permissionException : public kiteppException {

  public:
    permissionException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "PermissionException was thrown by Kite"; };
};

/**
 * @brief This exception is thrown when REST API doesn't return a `HTTP OK 200` code and any exception string.
 *
 */
class unknownException : public kiteppException {

  public:
    unknownException(int code, string message): kiteppException(code, std::move(message)) {};

    [[nodiscard]] const char* what() const noexcept override { return "unknown exception was thrown by Kite"; };
};

/**
 * @brief This exception is thrown when some error occures while processing data on library level. Serves as the
 * placeholder exception for all library related errors.
 *
 */
class libException : public std::exception {

  public:
    explicit libException(string Message): message(std::move(Message)) {};

    const char* what() { return message.c_str(); };

  private:
    string message;
};

inline void throwException(const string& exceptionString, int code, const string& msg) {
    // exception strings sent by API
    static const string tokenExceptionString = "TokenException";
    static const string userExceptionString = "UserException";
    static const string orderExcptionString = "OrderException";
    static const string inputExceptionString = "InputException";
    static const string networkExceptionString = "NetworkException";
    static const string dataExceptionString = "DataException";
    static const string generalExceptionString = "GeneralException";
    static const string permissionExceptionString = "PermissionException";

    // when rest api doesn't send an exception but http status code isn't 200 either
    static const string unknownExceptionString = "NoException";

    if (exceptionString == tokenExceptionString) { throw tokenException(code, msg); };
    if (exceptionString == userExceptionString) { throw userException(code, msg); };
    if (exceptionString == orderExcptionString) { throw orderException(code, msg); };
    if (exceptionString == inputExceptionString) { throw inputException(code, msg); };
    if (exceptionString == networkExceptionString) { throw networkException(code, msg); };
    if (exceptionString == dataExceptionString) { throw dataException(code, msg); };
    if (exceptionString == generalExceptionString) { throw generalException(code, msg); };
    if (exceptionString == permissionExceptionString) { throw permissionException(code, msg); };
    if (exceptionString == unknownExceptionString) { throw unknownException(code, msg); };

    throw libException("unknown exception was thrown by REST API");
};

} // namespace kiteconnect