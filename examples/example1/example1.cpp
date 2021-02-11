#include <iostream>
#include <kitepp.hpp>

namespace kc = kiteconnect;

int main() {

    std::cout << "Running..\n";

    try {

        kc::kite Kite("---apikey---");

        std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";

        std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;
        std::cout << "Enter API secret: ";
        std::cin >> apiSecret;

        std::string accessToken = Kite.generateSession(reqToken, apiSecret).tokens.accessToken;
        std::cout << "access token is " << accessToken << "\n";

        Kite.setAccessToken(accessToken);

        kc::userProfile profile = Kite.profile();
        std::cout << "email is :" << profile.email << "\n";
        std::cout << "Order types are: \n";
        for (const std::string& type : profile.orderTypes) { std::cout << type << ", "; };

        // clang-format off
} catch (kc::kiteppException& e) {

    std::cerr << e.what() << ", " << e.code() << ", " << e.message() << "\n";

} catch (kc::libException& e) {

     std::cerr << e.what() << "\n";
}
catch (std::exception& e) {

    std::cerr << e.what() << std::endl;

};
    // clang-format on

    return 0;
};
