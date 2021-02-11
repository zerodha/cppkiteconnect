#include <iostream>

#include "kitepp.hpp"

void onConnect(kitepp::kiteWS* ws) {
    std::cout << "connected.. Subscribing now..\n";
    ws->setMode("full", { 408065, 2953217 });
};

void onTicks(kitepp::kiteWS* ws, const std::vector<kitepp::tick>& ticks) {
    for (const auto& i : ticks) {
        std::cout << "instrument token: " << i.instrumentToken << " last price: " << i.lastPrice << "\n";
    };
};

void onError(kitepp::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kitepp::kiteWS* ws) { std::cout << "Couldn't connect..\n"; };

void onClose(kitepp::kiteWS* ws, int code, const std::string& message) {

    std::cout << "Closed the connection.. code: " << code << " message: " << message << "\n";
}

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("a9gg2394xe9sqmjc", 5, true, 5);

    kWS.setAccessToken("6QORGRaXV3YEDm0YH4ZYh55kuDJYmsw4");

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
