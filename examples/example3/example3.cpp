#include <iostream>

#include "kitepp/kitews.hpp"

void onConnect(kitepp::kiteWS* ws) {

    ws->subscribe({ 408065 });
    ws->setMode("quote", { 408065 });

    std::cout << "connected..\n";
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

    std::cout << "trading symbol: " << postback.tradingSymbol << " status: " << postback.status
              << " filled quantity: " << postback.filledQuantity << "\n";
};

void onMessage(kitepp::kiteWS* ws, const std::string& message) { std::cout << "Received message: " << message << "\n"; }

void onError(kitepp::kiteWS* ws, int code, const std::string& message) {

    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onWSError(kitepp::kiteWS* ws) { std::cout << "Couldn't connect..\n"; };

void onTryReconnect(kitepp::kiteWS* ws, unsigned int attemptCount) {

    std::cout << "Trying to reconnect.. Attempt " << attemptCount << "\n";
};

void onReconnectFail(kitepp::kiteWS* ws) { std::cout << "Failed to reconnect!\n"; }

void onClose(kitepp::kiteWS* ws, int code, const std::string& message) {
    std::cout << "Closed the connection.. code: " << code << " message: " << message << "\n";
}

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("a9gg2394xe9sqmjc", 5000, true, 5);

    kWS.setAccessToken("XLtDccOJ7p3NeMOGcGd2hMAvIjuP7GXS");
    kWS.onConnect = onConnect;
    kWS.onTicks = onTicks;
    kWS.onOrderUpdate = onOrderUpdate;
    kWS.onMessage = onMessage;
    kWS.onError = onError;
    kWS.onWSError = onWSError;
    kWS.onTryReconnect = onTryReconnect;
    kWS.onReconnectFail = onReconnectFail;
    kWS.onClose = onClose;

    kWS.connect();

    kWS.run();

    kWS.stop();

    /*std::vector<char> bytes = { static_cast<char>(0xdd), static_cast<char>(0xcc), static_cast<char>(0xbb),
        static_cast<char>(0xaa) };

    int value = _getNum2<int>(bytes, 0, 3);

    std::cout << value << "\n";
    std::cout << std::hex << value << '\n';*/

    // return 0;
};
