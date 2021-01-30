#include <iostream>

#include "kitepp/kitews.hpp"

void onMessage(kitepp::kiteWS* ws, char* message, size_t length) {

    std::cout << std::string(message, length) << std::endl;
};

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("apikey");
    kWS.setAccessToken("accessToken");
    kWS.onMessage = onMessage;

    kWS.connect();
    kWS.run();

    return 0;
};
