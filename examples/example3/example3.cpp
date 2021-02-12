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
