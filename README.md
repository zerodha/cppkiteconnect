# KITEPP - C++ client for communicating with [Zerodha's kite API](https://kite.trade/).

## **!!The library is under works!!**

kitepp is a header only library that wraps around Zerodha's REST API, Websockets saving you the hassle of directly communicating with API and provides an easy to use, modern, fast C++ interface.

## Table of Contents

- [Dependancies](#Dependancies)
- [What is covered](#What is covered)
- [Installation](#Installation)
- [Usage and example](#Usage and example)
- [Documentation](#Documentation)
- [Notes and YSKs](#Notes and YSKs)

## What is covered

kitepp provides wrapping around almost every API endpoint documented [here](https://kite.trade/docs/connect/v3/).

Websocket interface is currently under work.

## Dependancies

- You need to have [cpprestsdk](https://github.com/microsoft/cpprestsdk "cpprestsdk") installed. The library was made and tested with version `2.10.16`. You're advised to install the same version. Other versions *might* break the library.

On windows using vcpkg:

`PS> vcpkg install cpprestsdk cpprestsdk:x64-windows`

With apt-get on Debian/Ubuntu:

`$ sudo apt-get install libcpprest-dev`

With dnf on Fedora

`$ sudo dnf install cpprest-devel`

With brew on OSX

`$ brew install cpprestsdk`

Visit [cpprestsdk's github](https://github.com/microsoft/cpprestsdk "cpprestsdk's github") for other operating systems and detailed instructions.

- You need a compiler that supports C++11


## Installation

All you need to do is copy `include/` to your project's include path and link cpprestsdk.

If you're using `cmake`, Here's how your `CMakeLists.txt` should look like:

```
cmake_minimum_required(VERSION 3.10.2)
project(myproject) 

include_directories("this/path/should/contain/kitepp's/include/folder")
find_package(cpprestsdk REQUIRED)

add_executable(myproject main.cpp)
target_link_libraries(myproject cpprestsdk::cpprest)
```
You can find a complete `cmake` project [here](LINK-TO-EXAMPLE-REPO(under works)).

## Usage and example

Before starting, it is highly recommended that you go through [REST API's documentation](https://kite.trade/docs/connect/v3/) at least once to get yourself familier with what inputs are expected by API and what outputs are returned. kitepp's sole job is to provide C++ interface around REST API endpoints and hence, explaining details about expected fields is outside the scope of this documentation.

Just include the kitepp header like:
`#include "kitepp.hpp"`. 

Whole of the kitepp library is under `kitepp` namespace.

You need to initialize the kite object with your API key. You should create one instance of `kite` per API key. All REST endpoints are covered by `kite` class

```cpp
        kitepp::kite Kite("---apikey---");
```

Login flow starts from here. See https://kite.trade/docs/connect/v3/user/#login-flow for more info.

First step is to get login URL using `loginURL()` method Put the login URL in your browser and login. 

```cpp
        std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";
```
You should have a request token now.

We will now store the request token you obtained and API secret in (separate) variables. We will need both of them for getting access token.

```cpp
		std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;
        std::cout << "Enter API secret: ";
        std::cin >> apiSecret;
```
to get access token, other user session details, and more importantly, generate a user session, you need to call `generateSession("---request token---"", "---apiSecret---")` method with the request token you obtained and your API's secret key.

```cpp
        kitepp::njson sessionDetails = Kite.generateSession(reqToken, apiSecret);
```
You must be wondering what `kitepp::njson` is. `kitepp::njson` is an extremely reliable, user friendly and fairly efficient C++ `json` data type created by [N Lohmann](https://github.com/nlohmann) using magic of templates. Reason for renaming it to `njson` is name clash with cpprestsdk. Here on, we shall refer to `kitepp::njson` as `njson`. Most methods of the `kite` class return njson response. njson is extremely easy to work with. Here's an example:
```cpp
//create an njson object

kitepp::njson ex = {
	"active" : false,
	"list1" : ["A", "B", "C"],
		"objList" : [
			{
				"key1" : "value1",
				"key2" : [ 0, 1 ]
			}
		]

};

//pretty print an njson object

std::cout<<ex.dump(4); //4 is the desired indent

//access values

bool flag = ex["active"]
//or
bool flag = ex.at("active")

//Notice how you don't have to tell compiler the *type*
```
For detailed use of the njson object, visit [github link of the project](https://github.com/nlohmann/json). Their README has ample documentation.
 
 let's pretty print the response received and store the access token
 
 ```cpp
std::cout << "User session details received: " << sessionDetails.dump(4) << "\n";
std::string accessToken = sessionDetails["data"]["access_token"];
std::cout << "access token is " << accessToken << "\n";
```
After successfully obtaining access token, you need to set it using `setAccessToken()` method

```cpp
Kite.setAccessToken(accessToken);
std::cout << "access token set\n";
```
we're done with the login flow. Now you can use methods you need.

let's try and get our equity segment's available cash balance. The method used for getting user margins is `margins("---segment---")`. If no segment is provided, margins for all segments are returned. You'll find this pattern repeated throughout the library.

```cpp
kitepp::njson Margins = Kite.margins("equity");
std::cout << Margins.dump(4) << "\n";
std::cout << "Available cash balance of equity segment is: " << Margins["data"]["available"]["cash"] << std::endl;
```
All methods of kite class and their purpose is documented [here](LINK TO DOCS. under work).

kitepp throws two kinds of exceptions: exceptions derived from`kitepp::kiteppException` and `kitepp::libException`. `kiteppException`s are exceptions returned by kite API and you can extract HTTP code, message from them. You should always try to catch them both during every call to `kite` class. Both of these exception types are documented [here](LINK TO DOCS. under work).
```cpp
} catch (kitepp::kiteppException& e) {
    
        std::cerr << e.what() << ", " << e.code() << ", " << e.message() << "\n";

} catch (kitepp::libException& e) {
        
         std::cerr << e.what() << "\n"; 

} catch (std::exception& e) {

        std::cerr << e.what() << std::endl;
    
    };
```
This complete example can be found in the [examples directory](examples/example1/example1.cpp).

The [other example](examples/example2/example2.cpp) describes how to use every method.

## Documentation
- [REST API documentation](https://kite.trade/docs/connect/v3/ "REST API documentation")
- [kitepp documentation](under work "kitepp documentation")
- kitepp documentation can be built by following commands:
```
cmake ..
make docs
```

## Notes and YSKs

- kitepp has `fmtlib`, `PicoSHA2` and `nlohmann/json` embedded into it. If you already use them in your project, there might be conflicts. To get around this, you can either, use versions provided by kitepp (these packages are not modified at all) or you may chose to replace them. Replacing them might cause undefined behaviour and it isn't advised to do so.
- kitepp *pollutes* the global namespace with `_a` user defined string literal and `FMT` macro. This will be rectified in future versions but untill then, keep this in mind,

## Used third party tools and libraries
--under work--

## Other fields..
are under work

##License
GPLv3

