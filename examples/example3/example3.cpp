#include <chrono>
#include <iostream>
#include <thread>

#include "kitepp/kitews.hpp"

void onConnect(kitepp::kiteWS* ws) {

    ws->subscribe({ 408065 });
    // ws->setMode("quote", { 408065 });

    std::cout << "connected..\n";
};

void onMessage(kitepp::kiteWS* ws, char* message, size_t length) {

    std::cout << "message received..\n";
    std::cout << std::string(message, length) << std::endl;
};

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("a9gg2394xe9sqmjc");

    kWS.setAccessToken("Dze0745S2uL7QW1Ae25FdfK50ax5my5W");
    kWS.onConnect = onConnect;
    kWS.onMessage = onMessage;

    kWS.connect();

    kWS.run();

    /*std::vector<char> bytes = { static_cast<char>(0xdd), static_cast<char>(0xcc), static_cast<char>(0xbb),
        static_cast<char>(0xaa) };

    int value = _getNum2<int>(bytes, 0, 3);

    std::cout << value << "\n";
    std::cout << std::hex << value << '\n';*/

    // return 0;
};
