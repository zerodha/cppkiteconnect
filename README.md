# THE KITE CONNECT API C++ CLIENT

- [Overview](https://github.com/bhumitattarde/kitepp#overview)
- [Dependencies](https://github.com/bhumitattarde/kitepp#dependencies)
- [Getting dependencies](https://github.com/bhumitattarde/kitepp#getting-dependencies)
	- [Linux](https://github.com/bhumitattarde/kitepp#linux)
- [Building & installation](https://github.com/bhumitattarde/kitepp#building--installation)
- [Examples](https://github.com/bhumitattarde/kitepp#examples)
	- [REST API](https://github.com/bhumitattarde/kitepp#rest-api)
	- [Ticker](https://github.com/bhumitattarde/kitepp#ticker)
- [Documentation](https://github.com/bhumitattarde/kitepp#documentation)
- [License](https://github.com/bhumitattarde/kitepp#license)




## Overview

The official C++ client for communicating with the Kite Connect API. 

kitepp is a header only library that wraps around Zerodha's REST API, Websockets saving you the hassle of directly communicating with API and provides an easy to use, modern C++ interface.

Kite Connect is a set of REST-like APIs that expose many capabilities required to build a complete investment and trading platform. Execute orders in real time, manage user portfolio, stream live market data (WebSockets), and more, with the simple HTTP API collection.

## Dependencies

kitepp has following dependancies:
- [OpenSSL (devel)](https://github.com/openssl/openssl "OpenSSL")
- [uWebSockets v0.14 (devel)](https://github.com/uNetworking/uWebSockets/tree/v0.14) and [it's dependancies](https://github.com/hoytech/uWebSockets).
- [googletest (devel)]() and [googlemock (devel)]() are required for running tests.
- Doxygen is required for generating HTML/XML documentation.

## Getting dependencies

kitepp saves you the hassle of finding dependencies and provides convenient `cmake` options to download source packages as tarballs. 

Set `cmake`'s `GET_ALL_DEPS` or `GET_UWS` to `True` while configuring to fetch dependancies in `deps` directory. Unzip, build and install them where they can be discovered by `cmake`.
- To download all dependencies: `cmake .. -DGET_ALL_DEPS=True`
- To download just `uWS`: `cmake .. -DGET_UWS=True`

Alternatively and preferably, use package managers provided by your OS. Unless your package manager provides `v0.14` of `uWS`, you'll have to build and install it manually.

### Linux:
- On Fedora 32:
`sudo dnf install openssl-devel zlib-devel` + (`gtest-devel gmock-devel` for running tests)

## Building & installation

kitepp is a header-only C++ library. Just copy the `include` folder to system or project's include path.

### Build examples and tests:
```bash
mkdir build && cd build
cmake .. 
make
```

### Run tests:
`make test`

### Generate docs:
`make docs`

### Download dependancies:
`cmake .. -DGET_ALL_DEPS=True `

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

These and other more extensive examples as well as their `cmake` build files for reference can be found in the [examples directory](https://github.com/bhumitattarde/kitepp/tree/main/examples).

## Documentation

- [Kite Connect HTTP API documentation](https://kite.trade/docs/connect/v3/)
- [kitepp reference documentation](link_to_doxygen_docs_site)

## License

GPLv3




