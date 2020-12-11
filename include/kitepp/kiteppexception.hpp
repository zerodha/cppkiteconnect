#pragma once

#include <exception>
#include <string>
#include <utility>


namespace kitepp {


using std::string;


class kiteppException : public std::exception {


  public:
    // constructors and destructor

    kiteppException(int c, string msg): _code(c), _message(std::move(msg)) {};

    // methods

    const char* what() const noexcept override = 0;

    int code() const noexcept { return _code; };

    const char* message() const noexcept { return _message.c_str(); };


  private:
    int _code = 0;
    string _message = "";
};


} // namespace kitepp