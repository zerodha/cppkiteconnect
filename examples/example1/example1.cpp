#include <iostream>
#include <kitepp.hpp>

int main() {

    std::cout << "Running..\n";

    try {

        // Before starting, it is highly recommended that you go through [REST API's documentation]() at least once to get yourself familier with what
        // inputs are expected by API and what outputs are returned. kitepp's sole job is to provide C++ interface around REST API endpoints and
        // hence, explaining details about expected fields is outside the scope of this documentation


        // Whole of the kitepp library is under `kitepp` namespace.
        // You need to initialize the kite object with your API key. You should create one instance of `kite` per API key. All REST endpoints are
        // covered by `kite` class
        kitepp::kite Kite("---apikey---");

        // Login flow starts from here. See https://kite.trade/docs/connect/v3/user/#login-flow for more info.

        // First step is to get login URL using `loginURL()` method
        // Put the login URL in your browser and login.
        std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";
        // You should have a request token now.

        // we will now store the request token you obtained and API secret in (separate) variables. We will need both of them for getting access token
        std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;
        std::cout << "Enter API secret: ";
        std::cin >> apiSecret;

        // to get access token, other user session details, and more importantly, generate a user session, you need to call
        // generateSession("---request token---"", "---apiSecret---") method with the request token you obtained and your API's secret key.
        kitepp::njson sessionDetails = Kite.generateSession(reqToken, apiSecret);

        /*You must be wondering what `kitepp::njson` is. `kitepp::njson` is a extremely reliable, user friendly and fairly efficient C++ `json` data
           type created by [N Lohmann]() using magic of templates. Reason for renaming it to `njson` is name clash with cpprestsdk. Here on, we shall
           refer to `kitepp::njson` as `njson`. Most methods of the `kite` class return njson response. njson is extremely easy to
           work with. Here's an example:

            //create an njson object

            kitepp::njson ex = {
                "active" : false,
                "list1" : ["A", "B", "C"],
                "objList" : [
                        {
                        "key1" : "value1",
                        "key2" : [ 0, 1 ]
                                        }
                                ]

            };

            //pretty print an njson object

            std::cout<<ex.dump(4); //4 is the desired indent

            //access values

            bool flag = ex["active"]
            //or
            bool flag = ex.at("active")

            //Notice how you don't have to tell compiler the *type*

            For detailed use of the njson object, visit [github link of the project](). Their README has ample documentation.

         */

        // let's pretty print the response received and store the access token
        std::cout << "User session details received: " << sessionDetails.dump(4) << "\n";
        std::string accessToken = sessionDetails["data"]["access_token"];
        std::cout << "access token is " << accessToken << "\n";

        // After successfully obtaining access token, you need to set it using `setAccessToken()` method
        Kite.setAccessToken(accessToken);
        std::cout << "access token set\n";

        // we're done with the login flow. Now you can use methods you need.

        // let's try and get our equity segment's available cash balance. The method used for getting user margins is `margins("---segment---")`. If
        // no segment is provided, margins for all segments are returned. You'll find this pattern repeated throughout the library.
        kitepp::njson Margins = Kite.margins("equity");
        std::cout << Margins.dump(4) << "\n";
        std::cout << "Available cash balance of equity segment is: " << Margins["data"]["available"]["cash"] << std::endl;

        // All methods of kite class and their purpose is documented [here]().

        // kitepp throws two kinds of exceptions: exceptions derived from`kitepp::kiteppException` and `kitepp::libException`. `kiteppException`s are
        // exceptions returned by kite API and you can extract HTTP code, message from them. You should always try to catch them both during every
        // call to `kite` class. Both of these exception types are documented [here]().
        // clang-format off
    } catch (kitepp::kiteppException& e) {
    
        std::cerr << e.what() << ", " << e.code() << ", " << e.message() << "\n";

    } catch (kitepp::libException& e) {
        
         std::cerr << e.what() << "\n"; 
    } 
    catch (std::exception& e) {

        std::cerr << e.what() << std::endl;
    
    };
    //clang-format on


    return 0;
};
