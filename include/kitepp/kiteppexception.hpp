#pragma once

#include <string>
#include <exception>


namespace kitepp{


using std::string;


class kiteppException: public std::exception{


public:

//constructors and destructor

kiteppException(int c, string msg): _code(c), _message(msg){};

//methods

virtual const char* what() const noexcept = 0;

int code() const noexcept{

    return _code;

};

const char* message() const noexcept{

    return _message.c_str();

};


protected:

int _code = 0;
string _message = "";


};


}