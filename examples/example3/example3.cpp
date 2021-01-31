#include <chrono>
#include <iostream>
#include <thread>

#include "kitepp/kitews.hpp"

void onConnect(kitepp::kiteWS* ws) {

    ws->subscribe({ 408065 });
    // ws->setMode("quote", { 408065 });

    std::cout << "connected..\n";
};

void onTicks(kitepp::kiteWS* ws, const std::vector<kitepp::tick>& ticks) {

    for (const auto& i : ticks) {

        std::cout << "instrument token: " << i.instrumentToken << " last price: " << i.lastPrice << "\n";
    };
}

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("a9gg2394xe9sqmjc");

    kWS.setAccessToken("gbVToEHu8lmusVSjGaEy9A0Kk7Wzt2BB");
    kWS.onConnect = onConnect;
    kWS.onTicks = onTicks;

    kWS.connect();

    kWS.run();

    /*std::vector<char> bytes = { static_cast<char>(0xdd), static_cast<char>(0xcc), static_cast<char>(0xbb),
        static_cast<char>(0xaa) };

    int value = _getNum2<int>(bytes, 0, 3);

    std::cout << value << "\n";
    std::cout << std::hex << value << '\n';*/

    // return 0;
};
