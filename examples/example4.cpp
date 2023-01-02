/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a
 * copy of this software and associated  documentation files (the "Software"),
 * to deal in the Software  without restriction, including without  limitation
 * the rights to  use, copy,  modify, merge,  publish, distribute,  sublicense,
 * and/or  sell copies  of  the Software,  and  to  permit persons  to  whom the
 * Software  is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS
 * OR IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN
 * NO EVENT  SHALL THE AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY
 * CLAIM,  DAMAGES OR  OTHER LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>

#include "kitepp.hpp"

namespace kc = kiteconnect;

void onConnect(kc::ticker* ws) {
    std::cout << "connected.. subscribing now..\n";
    ws->setMode("full", { 408065, 2953217 });
};

unsigned int tickCount = 0;
void onTicks(kc::ticker* ws, const std::vector<kc::tick>& ticks) {
    tickCount++;
    for (const auto& i : ticks) {
        std::cout << "instrument token: " << i.instrumentToken
                  << " last price: " << i.lastPrice << "\n";
    };
    if (tickCount >= 100) { ws->stop(); };
};

void onOrderUpdate(kc::ticker* ws, const kc::postback& postback) {
    std::cout << "Order update: trading symbol: " << postback.tradingSymbol
              << " status: " << postback.status
              << " filled quantity: " << postback.filledQuantity << "\n";
};

void onMessage(kc::ticker* ws, const std::string& message) {
    std::cout << "Received message: " << message << "\n";
}

void onError(kc::ticker* ws, int code, const std::string& message) {
    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kc::ticker* ws) { std::cout << "Couldn't connect..\n"; };

void onTryReconnect(kc::ticker* ws, unsigned int attemptCount) {
    std::cout << "Trying to reconnect.. Attempt " << attemptCount << "\n";
};

void onReconnectFail(kc::ticker* ws) { std::cout << "Failed to reconnect!\n"; };

void onClose(kc::ticker* ws, int code, const std::string& message) {
    std::cout << "Closed the connection.. code: " << code
              << " message: " << message << "\n";
};

int main(int argc, char const* argv[]) {

    fmt::print("fmt version is {}\n",FMT_VERSION);

    kc::ticker Ticker(std::getenv("KITE_API_KEY"), 5, true, 5);

    Ticker.setAccessToken(std::getenv("KITE_ACCESS_TOKEN"));
    Ticker.onConnect = onConnect;
    Ticker.onTicks = onTicks;
    Ticker.onOrderUpdate = onOrderUpdate;
    Ticker.onMessage = onMessage;
    Ticker.onError = onError;
    Ticker.onConnectError = onConnectError;
    Ticker.onTryReconnect = onTryReconnect;
    Ticker.onReconnectFail = onReconnectFail;
    Ticker.onClose = onClose;

    Ticker.connect();
    Ticker.run();
    Ticker.stop();

    return 0;
};
