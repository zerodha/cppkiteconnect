#pragma once

#include <string>

#include "kiteppexception.hpp"

using std::string;


//exception strings sent by API

const string tokenExcpStr = "TokenException";
const string userExcpStr = "UserException";
const string orderExcpStr = "OrderException";
const string inputExcpStr = "InputException";
const string networkExcpStr = "NetworkException";
const string dataExcpStr = "DataException";
const string generalExcpStr = "GeneralException";


//exceptions

class tokenException: public kiteppException{


public:

//constructors and destructor

tokenException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "TokenException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class userException: public kiteppException{


public:

//constructors and destructor

userException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "UserException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class orderException: public kiteppException{


public:

//constructors and destructor

orderException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "OrderException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class inputException: public kiteppException{


public:

//constructors and destructor

inputException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "InputException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class networkException: public kiteppException{


public:

//constructors and destructor

networkException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "NetworkException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class dataException: public kiteppException{


public:

//constructors and destructor

dataException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "DataException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class generalException: public kiteppException{


public:

//constructors and destructor

generalException(int c, string msg): kiteppException(c, msg){};

//methods

const char* what() const noexcept{

    return "GeneralException was thrown by REST API. https://kite.trade/docs/connect/v3/exceptions/";

};


};


class libException: public std::exception{


public:

//constructors and destructor

libException(string msg): _message(msg){};

//methods

const char* what(){

        return _message.c_str();

    };


private:

string _message="";


};


//throw exception wrt string passed. Throws libException if string doesn't match with anything.

void throwException(const string& excpStr, int code, const string& msg){

    if(excpStr == tokenExcpStr){

        throw tokenException(code, msg);
    
    }else if(excpStr == userExcpStr){

        throw userException(code, msg);

    }else if(excpStr == orderExcpStr){

        throw orderException(code, msg);
        
    }else if(excpStr == inputExcpStr){

        throw inputException(code, msg);
        
    }else if(excpStr == networkExcpStr){

        throw networkException(code, msg);
        
    }else if(excpStr == dataExcpStr){

        throw dataException(code, msg);
        
    }else if(excpStr == generalExcpStr){

        throw generalException(code, msg);
        
    }else{

        throw libException("Unknown exception was thrown by REST API");

    };


};