/*
 *   Copyright (c) 2021 Bhumit Attarde

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <iostream>
#include <kitepp.hpp>

namespace kc = kiteconnect;

int main() {

    std::cout << "Running..\n";

    try {

        kc::kite Kite(std::getenv("KITE_API_KEY"));
        // kc::kite Kite("---apikey---");

        std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";

        std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;
        
        // std::cout << "Enter API secret: ";
        // std::cin >> apiSecret;
        apiSecret = std::getenv("KITE_API_SECRET");

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
