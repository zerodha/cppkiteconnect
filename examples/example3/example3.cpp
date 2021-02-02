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
}

void onClose(kitepp::kiteWS* ws, int code, const std::string& message) { std::cout << "Closed the connection..\n"; }

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("a9gg2394xe9sqmjc");

    kWS.setAccessToken("7QK8YYPyFH9PmixW4vPNWF2tME3LTNZq");
    kWS.onConnect = onConnect;
    kWS.onTicks = onTicks;
    kWS.onClose = onClose;

    kWS.connect();

    kWS.run();

    /*std::vector<char> bytes = { static_cast<char>(0xdd), static_cast<char>(0xcc), static_cast<char>(0xbb),
        static_cast<char>(0xaa) };

    int value = _getNum2<int>(bytes, 0, 3);

    std::cout << value << "\n";
    std::cout << std::hex << value << '\n';*/

    // return 0;
};
