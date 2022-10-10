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

The official C++ client for communicating with [Kite Connect API](https://kite.trade/).

CPPKiteConnect is a header-only library that wraps around Zerodha's KiteConnect REST API and WebSockets API. It saves you the hassle of directly communicating with the APIs and provides an easy to use, native and modern C++ interface.

## Dependencies

CPPKiteConnect requires C++17 and following dependancies:

- [OpenSSL (devel)](https://github.com/openssl/openssl "OpenSSL")
- [uWebSockets v0.14 (devel)](https://github.com/uNetworking/uWebSockets/tree/v0.14) and [its dependancies](https://github.com/hoytech/uWebSockets/blob/master/docs/Misc.-details.md#dependencies).
- [googletest](https://github.com/google/googletest) and [googlemock](https://github.com/google/googletest) are required for running tests.
- Doxygen is required for generating documentation.

## Getting dependencies

### Linux

- On Fedora 32:
`sudo dnf install openssl-devel zlib-devel` + (uWS v0.14) + (`gtest-devel gmock-devel` for running tests)
- On Ubuntu:
`sudo apt install libssl-dev zlib1g-dev` + (uWS v0.14) + (googletest, googlemock for running tests)

### Others & uWS v0.14

Use package managers provided by your OS. Unless your package manager provides `v0.14` of `uWS`, you'll have to build and install it manually.

You can also download source of the required dependencies by running `cmake .` in `deps` directory. This will place files in the same directory.

## Building & Installation

CPPKiteConnect is a header-only library. Copy the `include` folder to system or project's include path.

### Build

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=On <other-options>
make
```

If `cmake` cannot find your `uWS` library, try providing it manually to `cmake` like `cmake .. -DUWS_LIB=/path/to/uWS.so`. Note that this will build the library but some tests might not be run.

#### Build options

|  Option          | Description    |
| :--------------  | ----------:    |
| `BUILD_TESTS`    | Build tests    |
| `BUILD_EXAMPLES` | Build examples |    |
| `BUILD_DOCS`     | Build docs     |

### Run tests

`make && make test ARGS='-V'`

### Generate docs

`make docs`

## Examples

### REST API

```c++
#include <cstdlib>
#include <iostream>
#include "kitepp.hpp"

namespace kc = kiteconnect;

int main() {
    try {
        kc::kite Kite(std::getenv("KITE_API_KEY"));
        std::string apiSecret = std::getenv("KITE_API_SECRET");

        std::cout << "login URL: " << Kite.loginURL() << '\n';
        std::cout << "login with this URL and obtain the request token\n";

        std::string reqToken;
        std::cout << "enter obtained request token: ";
        std::cin >> reqToken;

        std::string accessToken =
            Kite.generateSession(reqToken, apiSecret).tokens.accessToken;
        Kite.setAccessToken(accessToken);
        std::cout << "access token is " << Kite.getAccessToken() << '\n';

        kc::userProfile profile = Kite.profile();
        std::cout << "name: " << profile.userName << "\n";
        std::cout << "email: " << profile.email << "\n";

} catch (kc::kiteppException& e) {
    std::cerr << e.what() << ", " << e.code() << ", " << e.message() << '\n';
} catch (kc::libException& e) {
     std::cerr << e.what() << '\n';
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

void onConnect(kc::ticker* ws) {
    std::cout << "Connected.. Subscribing now..\n";
    ws->setMode("full", { 408065, 2953217 });
};

void onTicks(kc::ticker* ws, const std::vector<kc::tick>& ticks) {
    for (const auto& i : ticks) {
        std::cout << "instrument token: " << i.instrumentToken
                  << " last price: " << i.lastPrice << "\n";
    };
};

void onError(kc::ticker* ws, int code, const std::string& message) {
    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kc::ticker* ws) { std::cout << "Couldn't connect..\n"; };

void onClose(kc::ticker* ws, int code, const std::string& message) {
    std::cout << "Closed the connection.. code: " << code
              << " message: " << message << "\n";
};

int main(int argc, char const* argv[]) {
    kc::ticker Ticker(std::getenv("KITE_API_KEY"), 5, true, 5);

    Ticker.setAccessToken(std::getenv("KITE_ACCESS_TOKEN"));
    Ticker.onConnect = onConnect;
    Ticker.onTicks = onTicks;
    Ticker.onError = onError;
    Ticker.onConnectError = onConnectError;
    Ticker.onClose = onClose;

    Ticker.connect();
    Ticker.run();
    Ticker.stop();

    return 0;
};
```

More examples can be found in the [examples directory](https://github.com/zerodha/cppkiteconnect/tree/main/examples).

## Documentation

- [KiteConnect API documentation](https://kite.trade/docs/connect/v3/)
- [CPPKiteConnect reference documentation](https://kite.trade/docs/cppkiteconnect/index.htmll#autotoc_md14)

## License

[MIT](https://opensource.org/licenses/MIT)
