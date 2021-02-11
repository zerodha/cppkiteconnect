#include <iostream>

#include "kitepp.hpp"

void onConnect(kitepp::kiteWS* ws) {
    std::cout << "connected.. Subscribing now..\n";
    ws->setMode("full", { 408065, 2953217 });
};

unsigned int tickCount = 0;
void onTicks(kitepp::kiteWS* ws, const std::vector<kitepp::tick>& ticks) {
    tickCount++;

    for (const auto& i : ticks) {
        std::cout << "instrument token: " << i.instrumentToken << " last price: " << i.lastPrice << "\n";
    };

    if (tickCount >= 100) { ws->stop(); };
};

void onOrderUpdate(kitepp::kiteWS* ws, const kitepp::postback& postback) {
    std::cout << "Order update: trading symbol: " << postback.tradingSymbol << " status: " << postback.status
              << " filled quantity: " << postback.filledQuantity << "\n";
};

void onMessage(kitepp::kiteWS* ws, const std::string& message) { std::cout << "Received message: " << message << "\n"; }

void onError(kitepp::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kitepp::kiteWS* ws) { std::cout << "Couldn't connect..\n"; };

void onTryReconnect(kitepp::kiteWS* ws, unsigned int attemptCount) {
    std::cout << "Trying to reconnect.. Attempt " << attemptCount << "\n";
};

void onReconnectFail(kitepp::kiteWS* ws) { std::cout << "Failed to reconnect!\n"; };

void onClose(kitepp::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Closed the connection.. code: " << code << " message: " << message << "\n";
};

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("---APIKEY---", 5, true, 5);

    kWS.setAccessToken("---ACCESSTOKEN---");

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
