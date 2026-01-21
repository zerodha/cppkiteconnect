# Kite Connect API C++ client

- [Overview](https://github.com/zerodha/cppkiteconnect#overview)
- [Dependencies](https://github.com/zerodha/cppkiteconnect#dependencies)
- [Getting dependencies](https://github.com/zerodha/cppkiteconnect#getting-dependencies)
  - [Linux](https://github.com/zerodha/cppkiteconnect#linux)
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

CPPKiteConnect requires C++23 and following dependencies:

- [OpenSSL (devel)](https://github.com/openssl/openssl "OpenSSL")
- [Boost](https://www.boost.org/) (1.74+ recommended) - Beast and Asio components for WebSocket functionality
- [fmt](https://github.com/fmtlib/fmt) (10.0+ recommended) - String formatting library
- [glaze](https://github.com/stephenberry/glaze) - High-performance JSON library for ticker/WebSocket
- [zlib (devel)](https://zlib.net/)
- [googletest](https://github.com/google/googletest) and [googlemock](https://github.com/google/googletest) are required for running tests.
- Doxygen is required for generating documentation.

## Getting dependencies

### Linux

- On Fedora:
`sudo dnf install openssl-devel zlib-devel boost-devel fmt-devel` + glaze (build from source) + (`gtest-devel gmock-devel` for running tests)
- On Ubuntu:
`sudo apt install libssl-dev zlib1g-dev libboost-all-dev libfmt-dev` + glaze (build from source) + (googletest, googlemock for running tests)

Note: [glaze](https://github.com/stephenberry/glaze) is header-only and can be installed with:
```bash
git clone https://github.com/stephenberry/glaze.git
cd glaze && cmake -B build && sudo cmake --install build
```

You can also download source of some dependencies by running `cmake .` in `deps` directory. This will place files in the same directory.

## Building & Installation

Clone the repository and fetch the submodules

```sh
git clone https://github.com/zerodha/cppkiteconnect.git
submodule update --init --recursive
```

CPPKiteConnect is a header-only library. Copy the `include` folder to system or project's include path.

### Build

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=On <other-options>
make
```

#### Build options

|  Option          | Description    |
| :--------------  | ----------:    |
| `BUILD_TESTS`    | Build tests    |
| `BUILD_EXAMPLES` | Build examples |
| `BUILD_DOCS`     | Build docs     |

### Run examples using Docker

#### Build the image

In project directory,

`docker build -t cppkiteconnect-dev .`

#### Run the built image

`docker run -it -e EXAMPLE_NUMBER=1 cppkiteconnect-dev`

##### env variables

| Option              | Description                  |
|:--------------------|-----------------------------:|
| `EXAMPLE_NUMBER`    | Example to run (e.g., `1`  ) |
| `KITE_API_KEY`      | Kite API key                 |
| `KITE_ACCESS_TOKEN` | Kite access token            |
| `KITE_API_SECRET`   | Kite API secret              |

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
- [CPPKiteConnect reference documentation](https://kite.trade/docs/cppkiteconnect/#documentation)

## License

[MIT](https://opensource.org/licenses/MIT)
