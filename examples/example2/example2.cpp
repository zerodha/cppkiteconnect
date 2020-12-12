#include <iostream>
#include <string>

//! [adding kitepp header]
#include "kitepp.hpp"
//! [adding kitepp header]


int main() {


    std::cout << "RUNNING\n";

    try {

        //! [initializing kite]
        kitepp::kite Kite("a9gg2394xe9sqmjc");
        //! [initializing kite]

        //! [obtaining login url]
        // std::cout << "Login URL: " << Kite.loginURL() << "\nLogin with this URL and obtain the request token.\n";
        //! [obtaining login url]

        //! [obtaining access token]
        /*
        std::string apiSecret;
        std::string reqToken;

        std::cout << "Enter obtained request token: ";
        std::cin >> reqToken;
        std::cout << "Enter API secret: ";
        std::cin >> apiSecret;

        std::string accessToken = Kite.generateSession(reqToken, apiSecret)["data"]["access_token"];
        std::cout << "access token is " << accessToken << "\n";
        */
        //! [obtaining access token]

        //! [settting access token]
        // Kite.setAccessToken(accessToken);
        Kite.setAccessToken("p21mTu38rZlNpzV8KLqXlpTJH3Rgimzb"); // FIXME comment this later on
        //! [settting access token]

        //! [invalidate session]
        // std::cout << "Session invalidate response: " << Kite.invalidateSession().dump(4) << "\n";
        //! [invalidate session]

        //! [get user profile]
        // std::cout << "Profile: " << Kite.profile().dump(4) << "\n";
        //! [get user profile]

        //! [get margins]
        // std::cout << "Margins: " << Kite.margins().dump(4) << "\n";
        // std::cout << "Margins: " << Kite.margins("equity").dump(4) << "\n";
        //! [get margins]

        //! [placing an order]
        /*
        std::cout << "Place order response: " << Kite.placeOrder("regular", "NSE", "BHEL", "SELL", "1", "MIS", "MARKET", "34.90", "DAY").dump(4)
                  << "\n";
        std::cout << "Place order response: "
                  << Kite.placeOrder("co", "NSE", "BHEL", "BUY", "1", "MIS", "LIMIT", "33", "DAY", "32", "", "", "").dump(4) << "\n";
        */
        //! [placing an order]

        //! [modifying an order]
        // std::cout << "modify order response: " << Kite.modifyOrder("regular", "151220000000000", "", "2", "", "MARKET", "", "DAY").dump(4) << "\n";
        //! [modifying an order]

        //! [cancelling an order]
        // std::cout << "cancel order response: " << Kite.cancelOrder("regular", "151220000000000", "123").dump(4) << "\n";
        //! [cancelling an order]

        //! [exiting an order]
        // std::cout << "exit order response: " << Kite.exitOrder("co", "201207001972149").dump(4) << "\n";
        //! [exiting an order]

        //! [get orders]
        // std::cout << "orders response: " << Kite.orders().dump(4) << "\n";
        //! [get orders]

        //! [get order history]
        // std::cout << "order history response: " << Kite.orderHistory("201207001459491").dump(4) << "\n";
        //! [get order history]

        //! [get trades]
        // std::cout << "trades response: " << Kite.trades().dump(4) << "\n";
        //! [get trades]

        //! [get order trades]
        // std::cout << "order trades response: " << Kite.orderTrades("201207001459491").dump(4) << "\n";
        //! [get order trades]

        //! [placing a gtt]
        /*
        auto gttOrds = kitepp::njson::array();

        gttOrds.push_back({

            { "transaction_type", "BUY" },
            { "quantity", 1 },
            { "order_type", "LIMIT" },
            { "product", "CNC" },
            { "price", 21100 },

        });

        std::cout << "place gtt response: " << Kite.placeGTT("single", "NESTLEIND", "NSE", { 20000 }, "17596.95", gttOrds).dump(4) << "\n";
        */
        //! [placing a gtt]

        //! [modifying a gtt]
        /*
        auto gttOrds = kitepp::njson::array();

        gttOrds.push_back({

            { "transaction_type", "BUY" },
            { "quantity", 1 },
            { "order_type", "LIMIT" },
            { "product", "CNC" },
            { "price", 22100 },

        });

        std::cout << "modify gtt response: " << Kite.modifyGTT("21764342", "single", "NESTLEIND", "NSE", { 22000 }, "17596.95", gttOrds).dump(4)
                  << "\n";
        */
        //! [modifying a gtt]

        //! [get gtts]
        // std::cout << "get gtts response: " << Kite.getGTTs().dump(4) << "\n";
        //! [get gtts]

        //! [get gtt info]
        // std::cout << "get gtt response: " << Kite.getGTT("21764342").dump(4) << "\n";
        //! [get gtt info]

        //! [delete a gtt]
        // std::cout << "delete gtt response: " << Kite.deleteGTT("21764342").dump(4) << "\n";
        //! [delete a gtt]

        //! [get holdings]
        // std::cout << "get holdings response: " << Kite.holdings().dump(4) << "\n";
        //! [get holdings]

        //! [get positions]
        // std::cout << "get positions response: " << Kite.positions().dump(4) << std::endl;
        //! [get positions]

        //! [convert position]
        /*
        std::cout << "convert position response: " << Kite.convertPosition("NSE", "INFY", "BUY", "overnight", "3", "NRML", "MIS").dump(4)
                  << std::endl;
        */
        //! [convert position]

        //! [get quote]
        // std::cout << "get quote response: " << Kite.quote({ "NSE:INFY", "NSE:NIFTY+50", "BSE:RELIANCE" }).dump(4) << std::endl;
        //! [get quote]

        //! [get ohlc]
        // std::cout << "get ohlc response: " << Kite.ohlc({ "NSE:INFY", "NSE:NIFTY+50", "BSE:RELIANCE" }).dump(4) << std::endl;
        //! [get ohlc]

        //! [get ltp]
        // std::cout << "get ltp response: " << Kite.ltp({ "NSE:INFY", "NSE:NIFTY+50", "BSE:RELIANCE" }).dump(4) << std::endl;
        //! [get ltp]

        //! [get historical data]
        /*
        std::cout << "get historical data response: " << Kite.historicalData("5633", "2020-11-01+09:30:00", "2020-12-01+10:30:00", "minute").dump(4)
                  << std::endl;
        */
        //! [get historical data]

        //! [place mf order]
        // std::cout << "place mf response: " << Kite.placeMFOrder("INF174K01LS2", "BUY", "", "10000").dump(4) << std::endl;
        //! [place mf order]

        //! [get mf order info]
        // std::cout << "get mf order response: " << Kite.MFOrder("e05f2303-1717-4c54-834b-01003df8dc8a").dump(4) << std::endl;
        //! [get mf order info]

        //! [get mf orders]
        // std::cout << "get mf orders response: " << Kite.MFOrders().dump(4) << std::endl;
        //! [get mf orders]

        //! [cancel a mf order]
        // std::cout << "cancel mf order response: " << Kite.cancelMFOrder("e05f2303-1717-4c54-834b-01003df8dc8a").dump(4) << std::endl;
        //! [cancel a mf order]

        //! [place mf sip order]
        // std::cout << "place mf sip order response: " << Kite.placeMFSIP("INF174K01LS2", "10000", "-1", "monthly", "50000", "1").dump(4) <<
        // std::endl;
        //! [place mf sip order]

        //! [modify mf sip order]
        /*
        std::cout << "modify mf sip order response: " << Kite.modifyMFSIP("825802497812153", "5000", "paused", "10", "monthly", "5").dump(4)
                  << std::endl;
        */
        //! [modify mf sip order]

        //! [get sip info]
        // std::cout << "get sip resposne: " << Kite.SIP("825802497812153").dump(4) << std::endl;
        //! [get sip info]

        //! [cancel mf sip]
        // std::cout << "cancel mf sip response" << Kite.cancelMFSIP("825802497812153").dump(4) << std::endl;
        //! [cancel mf sip]

        //! [get sips]
        // std::cout << "get sips response: " << Kite.SIPs().dump(4) << std::endl;
        //! [get sips]

        //! [get mf holdings]
        // std::cout << "get mf holdings response: " << Kite.MFHoldings().dump(4) << std::endl;
        //! [get mf holdings]

        //! [get instruments]
        // std::cout << "get instruments response: " << Kite.instruments("NSE") << std::endl;
        //! [get instruments]

        //! [get mf instruments]
        // std::cout << "get mf instruments response: " << Kite.MFInstruments() << std::endl;
        //! [get mf instruments]

        //! [dealing with kitepp exceptions]
    } catch (kitepp::kiteppException& e) {
        std::cerr << e.what() << ", " << e.code() << ", " << e.message() << "\n";
    } catch (kitepp::libException& e) {
        std::cerr << e.what() << "\n";
    }
    //! [dealing with kitepp exceptions]

    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    };

    return 0;
}
