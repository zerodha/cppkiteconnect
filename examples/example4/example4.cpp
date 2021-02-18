/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
*/

#include <iostream>

#include "kitepp.hpp"

namespace kc = kiteconnect;

void onConnect(kc::kiteWS* ws) {
    std::cout << "connected.. Subscribing now..\n";
    ws->setMode("full", { 408065, 2953217 });
};

unsigned int tickCount = 0;
void onTicks(kc::kiteWS* ws, const std::vector<kc::tick>& ticks) {
    tickCount++;

    for (const auto& i : ticks) {
        std::cout << "instrument token: " << i.instrumentToken << " last price: " << i.lastPrice << "\n";
    };

    if (tickCount >= 100) { ws->stop(); };
};

void onOrderUpdate(kc::kiteWS* ws, const kc::postback& postback) {
    std::cout << "Order update: trading symbol: " << postback.tradingSymbol << " status: " << postback.status
              << " filled quantity: " << postback.filledQuantity << "\n";
};

void onMessage(kc::kiteWS* ws, const std::string& message) { std::cout << "Received message: " << message << "\n"; }

void onError(kc::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kc::kiteWS* ws) { std::cout << "Couldn't connect..\n"; };

void onTryReconnect(kc::kiteWS* ws, unsigned int attemptCount) {
    std::cout << "Trying to reconnect.. Attempt " << attemptCount << "\n";
};

void onReconnectFail(kc::kiteWS* ws) { std::cout << "Failed to reconnect!\n"; };

void onClose(kc::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Closed the connection.. code: " << code << " message: " << message << "\n";
};

int main(int argc, char const* argv[]) {

    // kc::kiteWS kWS("---APIKEY---", 5, true, 5);
    kc::kiteWS kWS(std::getenv("KITE_API_KEY"), 5, true, 5);

    // kWS.setAccessToken("---ACCESSTOKEN---");
    kWS.setAccessToken(std::getenv("KITE_ACCESS_TOKEN"));

    kWS.onConnect = onConnect;
    kWS.onTicks = onTicks;
    kWS.onOrderUpdate = onOrderUpdate;
    kWS.onMessage = onMessage;
    kWS.onError = onError;
    kWS.onConnectError = onConnectError;
    kWS.onTryReconnect = onTryReconnect;
    kWS.onReconnectFail = onReconnectFail;
    kWS.onClose = onClose;

    kWS.connect();

    kWS.run();

    kWS.stop();

    return 0;
};
