#pragma once

#include <string>
#include <utility>

#include "kiteppexception.hpp"


namespace kitepp {


using kitepp::kiteppException;
using std::string;


// exceptions

class tokenException : public kiteppException {


  public:
    // constructors and destructor

    tokenException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "TokenException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class userException : public kiteppException {


  public:
    // constructors and destructor

    userException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "UserException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class orderException : public kiteppException {


  public:
    // constructors and destructor

    orderException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "OrderException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class inputException : public kiteppException {


  public:
    // constructors and destructor

    inputException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "InputException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class networkException : public kiteppException {


  public:
    // constructors and destructor

    networkException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "NetworkException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class dataException : public kiteppException {


  public:
    // constructors and destructor

    dataException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "DataException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class generalException : public kiteppException {


  public:
    // constructors and destructor

    generalException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "GeneralException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


class permissionException : public kiteppException {


  public:
    // constructors and destructor

    permissionException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override {
        return "PermissionException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";
    };
};


class noException : public kiteppException {


  public:
    // constructors and destructor

    noException(int c, string msg): kiteppException(c, std::move(msg)) {};

    // methods

    const char* what() const noexcept override { return "No exception was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/"; };
};


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

inline void throwException(const string& excpStr, int code, const string& msg) {


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


} // namespace kitepp