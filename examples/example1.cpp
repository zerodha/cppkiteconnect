/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
*/

#include <cstdlib>
#include <iostream>
#include <kitepp.hpp>

namespace kc = kiteconnect;

int main() {
    try {
        kc::kite Kite(std::getenv("KITE_API_KEY"));
        std::string apiSecret = std::getenv("KITE_API_SECRET");

        std::cout << "login URL: " << Kite.loginURL() << '\n';
        std::cout << "login with this URL and obtain the request token\n";

        std::string reqToken;
        std::cout << "enter obtained request token: ";
        std::cin >> reqToken;

        std::string accessToken = Kite.generateSession(reqToken, apiSecret).tokens.accessToken;
        Kite.setAccessToken(accessToken);
        std::cout << "access token is " << Kite.getAccessToken() << '\n';

        kc::userProfile profile = Kite.profile();
        std::cout << "name: " << profile.userName << "\n";
        std::cout << "email: " << profile.email << "\n";

        // clang-format off
} catch (kc::kiteppException& e) {
    std::cerr << e.what() << ", " << e.code() << ", " << e.message() << '\n';
} catch (kc::libException& e) {
     std::cerr << e.what() << '\n';
}
catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
};
    // clang-format on

    return 0;
};
