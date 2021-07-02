# Kite Connect API C++ client

- [Overview](https://github.com/zerodha/cppkiteconnect#overview)
- [Dependencies](https://github.com/zerodha/cppkiteconnect#dependencies)
- [Getting dependencies](https://github.com/zerodha/cppkiteconnect#getting-dependencies)
	- [Linux](https://github.com/zerodha/cppkiteconnect#linux)
	- [Others & uWS v0.14](https://github.com/zerodha/cppkiteconnect#others--uws-v014)
- [Building & installation](https://github.com/zerodha/cppkiteconnect#building--installation)
- [Examples](https://github.com/zerodha/cppkiteconnect#examples)
	- [REST API](https://github.com/zerodha/cppkiteconnect#rest-api)
	- [Ticker](https://github.com/zerodha/cppkiteconnect#ticker)
- [Documentation](https://github.com/zerodha/cppkiteconnect#documentation)
- [License](https://github.com/zerodha/cppkiteconnect#license)

## Overview

The official C++ client for communicating with the [Kite Connect API](https://kite.trade/). 

CPPKiteConnect is a header-only library that wraps around Zerodha's KiteConnect REST API and WebSockets API. It saves you the hassle of directly communicating with the APIs and provides an easy to use, native, modern C++ interface.

## Dependencies

CPPKiteConnect has following dependancies:
- [OpenSSL (devel)](https://github.com/openssl/openssl "OpenSSL")
- [uWebSockets v0.14 (devel)](https://github.com/uNetworking/uWebSockets/tree/v0.14) and [it's dependancies](https://github.com/hoytech/uWebSockets/blob/master/docs/Misc.-details.md#dependencies).
- [googletest (devel)]() and [googlemock (devel)]() are required for running tests.
- Doxygen is required for generating HTML/XML documentation.

## Getting dependencies

### Linux:
- On Fedora 32:
`sudo dnf install openssl-devel zlib-devel` + (uWS v0.14) + (`gtest-devel gmock-devel` for running tests)
- On Ubuntu:
`sudo apt install libssl-dev zlib1g-dev` + (uWS v0.14) + (googletest & googlemock for running tests)

### Others & uWS v0.14

CPPKiteConnect saves you the hassle of manually downloading dependencies and provides convenient cmake target to download source packages as tarballs. 

- To download dependencies (including uWS v0.14): `cmake .` in `deps` directory.

Alternatively and preferably, use package managers provided by your OS. Unless your package manager provides `v0.14` of `uWS`, you'll have to build and install it manually.

## Building & Installation

CPPKiteConnect is a header-only C++ library. Just copy the `include` folder to system or project's include path.

Refer to [this](https://github.com/bhumitattarde/kiteppex) repository to see a project using CPPKiteConnect and a boilerplate `CMakeLists.txt` to get started.

### Build examples and tests:
```bash
mkdir build && cd build
cmake .. 
make
```

If `cmake` cannot find your `uWS` library, try providing it manually to `cmake` like `cmake .. -DUWS_LIB=/path/to/uWS.so`. Note that this will 
build the library but some tests won't be run.

### Run tests:
`make && make test ARGS='-V'`

### Generate docs:
`make docs`

## Examples

### REST API

```c++
#include <iostream>
#include <kitepp.hpp>

namespace kc = kiteconnect;

int main() {

    std::cout << "Running..\n";

    try {

        kc::kite Kite("---apikey---");

        std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";

        std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;
        std::cout << "Enter API secret: ";
        std::cin >> apiSecret;

        std::string accessToken = Kite.generateSession(reqToken, apiSecret).tokens.accessToken;
        std::cout << "access token is " << accessToken << "\n";

        Kite.setAccessToken(accessToken);

        kc::userProfile profile = Kite.profile();
        std::cout << "email is :" << profile.email << "\n";
        std::cout << "Order types are: \n";
        for (const std::string& type : profile.orderTypes) { std::cout << type << ", "; };

} catch (kc::kiteppException& e) {
    std::cerr << e.what() << ", " << e.code() << ", " << e.message() << "\n";
} catch (kc::libException& e) {
    std::cerr << e.what() << "\n";
}
catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
};
    return 0;
};
```


### Ticker

```c++
#include <iostream>
#include "kitepp.hpp"

namespace kc = kiteconnect;

void onConnect(kc::kiteWS* ws) {
    std::cout << "connected.. Subscribing now..\n";
    ws->setMode("full", { 408065, 2953217 });
};

void onTicks(kc::kiteWS* ws, const std::vector<kc::tick>& ticks) {
    for (const auto& i : ticks) {
        std::cout << "instrument token: " << i.instrumentToken << " last price: " << i.lastPrice << "\n";
    };
};

void onError(kc::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kc::kiteWS* ws) { std::cout << "Couldn't connect..\n"; };

void onClose(kc::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Closed the connection.. code: " << code << " message: " << message << "\n";
};

int main(int argc, char const* argv[]) {

    kc::kiteWS kWS("---APIKEY---", 5, true, 5);

    kWS.setAccessToken("---ACCESSTOKEN---");

    kWS.onConnect = onConnect;
    kWS.onTicks = onTicks;
    kWS.onError = onError;
    kWS.onConnectError = onConnectError;
    kWS.onClose = onClose;

    kWS.connect();

    kWS.run();

    kWS.stop();

    return 0;
};
```

These and other more extensive examples as well as their `cmake` build files for reference can be found in the [examples directory](https://github.com/zerodha/cppkiteconnect/tree/main/examples).

## Documentation

- [Kite Connect HTTP API documentation](https://kite.trade/docs/connect/v3/)
- [CPPKiteConnect reference documentation](https://kite.trade/docs/cppkiteconnect/)

## License

MIT




