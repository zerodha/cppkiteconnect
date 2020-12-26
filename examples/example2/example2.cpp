#include <iostream>
#include <string>
#include <vector>

//! [adding kitepp header]
#include "kitepp.hpp"
#include "kitepp/responses.hpp"
//! [adding kitepp header]


int main() {


    std::cout << "RUNNING\n";

    //! Content is commented out on purpose so you can enable the parts you want

    try {

        //! [initializing kite]
        // kitepp::kite Kite("---apikey---");
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

        // std::string accessToken = Kite.generateSession(reqToken, apiSecret)["data"]["access_token"];
        std::string accessToken = Kite.generateSession(reqToken, apiSecret).tokens.accessToken;

        std::cout << "access token is " << accessToken << "\n";

        */
        //! [obtaining access token]

        //! [settting access token]
        // Kite.setAccessToken(accessToken);
        Kite.setAccessToken("zg9yIIBQnQa7COd74ncNX4LzfMAefrWL");
        //! [settting access token]

        //! [invalidate session]
        // std::cout << "Session invalidate response: " << Kite.invalidateSession().dump(4) << "\n";
        //! [invalidate session]

        //! [get user profile]
        /*kitepp::userProfile profile = Kite.profile();
        std::cout << "email is :" << profile.email << "\n";
        std::cout << "Order types are: \n";
        for (const std::string& type : profile.orderTypes) { std::cout << type << ", "; };*/
        //! [get user profile]

        //! [get margins]
        /*kitepp::allMargins Margins = Kite.getMargins();
        std::cout << "Available cash for equity segment is " << Margins.equity.available.cash << "\n";

        kitepp::margins eqMargins = Kite.getMargins("equity");
        std::cout << "adhoc margin for equity segment is " << eqMargins.available.adHocMargin << "\n";*/
        //! [get margins]

        //! [placing an order]
        /*std::string ordID = Kite.placeOrder("co", "NSE", "BHEL", "BUY", 1, "MIS", "LIMIT", 33.5, "DAY", 32, kitepp::kite::DEFAULTDOUBLE,
        std::string ordID2 = Kite.placeOrder("regular", "NSE", "BHEL", "SELL", 1, "MIS", "MARKET", 33.60, "DAY");
            kitepp::kite::DEFAULTDOUBLE, kitepp::kite::DEFAULTDOUBLE);
        if (!ordID.empty()) {

            std::cout << "order ID: " << ordID << "\n";
        } else {

            std::cout << "failed to place order"
                      << "\n";
        };*/
        //! [placing an order]

        //! [modifying an order]
        // std::cout << "modify order response: " << Kite.modifyOrder("regular", "151220000000000", "", "2", "", "MARKET", "", "DAY").dump(4) << "\n";

        /*std::string modOrdID = Kite.modifyOrder("regular", "201223002459577", "", 2, Kite.DEFAULTDOUBLE, "MARKET", Kite.DEFAULTDOUBLE, "DAY");
        if (!modOrdID.empty()) {

            std::cout << "order ID: " << modOrdID << "\n";
        } else {

            std::cout << "failed to modify order"
                      << "\n";
        };*/
        //! [modifying an order]

        //! [cancelling an order]
        /*std::string canOrdID = Kite.cancelOrder("regular", "201223002513052");

        if (!canOrdID.empty()) {

            std::cout << "order ID: " << canOrdID << "\n";
        } else {

            std::cout << "failed to cancel order"
                      << "\n";
        };*/
        //! [cancelling an order]

        //! [exiting an order]
        /*std::string exitOrdID = Kite.exitOrder("regular", "201223002546330", "201223002546328");

        if (!exitOrdID.empty()) {

            std::cout << "order ID: " << exitOrdID << "\n";
        } else {

            std::cout << "failed to exit order"
                      << "\n";
        };*/
        //! [exiting an order]

        //! [get orders]
        /*std::vector<kitepp::order> orderVec = Kite.orders();
        for (const auto& i : orderVec) { std::cout << "Order ID: " << i.orderID << " & status: " << i.status << "\n"; }*/
        //! [get orders]

        //! [get order history]
        /*std::vector<kitepp::order> orderHistoryVec = Kite.orderHistory("201223002380513");
        for (const auto& i : orderHistoryVec) { std::cout << "status: " << i.status << " & filled quantity: " << i.filledQuantity << "\n"; };*/
        //! [get order history]

        //! [get trades]
        /*std::vector<kitepp::trade> tradesVec = Kite.trades();
        for (const auto& i : tradesVec) { std::cout << "trade ID: " << i.tradeID << " & filled at: " << i.fillTimestamp << "\n"; };*/
        //! [get trades]

        //! [get order trades]
        /*std::vector<kitepp::trade> tradesVec = Kite.orderTrades("201223002511160");
        for (const auto& i : tradesVec) { std::cout << "trade ID: " << i.tradeID << " & filled at: " << i.fillTimestamp << "\n"; };*/
        //! [get order trades]

        //! [placing a gtt]
        /*kitepp::GTTParams gttparam("BUY", 1, "LIMIT", "CNC", 21100);
        std::vector<kitepp::GTTParams> gttparams = { gttparam };
        // gttparams.emplace_back("BUY", 10, "LIMIT", "CNC", 21105);
        // int trigID = Kite.placeGTT("two-leg", "NESTLEIND", "NSE", { 15000, 20007 }, 18580.0, gttparams);

        int trigID = Kite.placeGTT("single", "NESTLEIND", "NSE", { 20000 }, 18580.0, gttparams);

        if (!Kite.isValid(trigID)) {

            std::cout << "trigger ID: " << trigID << "\n";
        } else {

            std::cout << "failed to place gtt"
                      << "\n";
        };*/
        //! [placing a gtt]

        //! [modifying a gtt]
        /*kitepp::GTTParams gttparam("BUY", 1, "LIMIT", "CNC", 21100);
        std::vector<kitepp::GTTParams> gttparams = { gttparam };

        int modTrigID = Kite.modifyGTT(23246584, "single", "NESTLEIND", "NSE", { 23500 }, 18580.0, gttparams);

        if (!Kite.isValid(modTrigID)) {

            std::cout << "trigger ID: " << modTrigID << "\n";
        } else {

            std::cout << "failed to modify gtt"
                      << "\n";
        };*/
        //! [modifying a gtt]

        //! [get gtts]
        /*std::vector<kitepp::GTT> gttVec = Kite.getGTTs();
        for (const auto& gtt : gttVec) { std::cout << "ID: " << gtt.ID << " & created at: " << gtt.createdAt << "\n"; };*/
        //! [get gtts]

        //! [get gtt info]
        /*kitepp::GTT gtt = Kite.getGTT(23271899);
        std::cout << "Created at: " << gtt.createdAt << " & first trigger value: " << gtt.condition.triggerValues[0] << "\n";*/
        //! [get gtt info]

        //! [delete a gtt]
        /*int trigID = Kite.deleteGTT(23271889);

        if (!Kite.isValid(trigID)) {

            std::cout << "trigger ID: " << trigID << "\n";
        } else {

            std::cout << "failed to delete gtt"
                      << "\n";
        };*/
        //! [delete a gtt]

        //! [get holdings]
        /*std::vector<kitepp::holding> holdings = Kite.holdings();
        for (const auto& i : holdings) { std::cout << "trading symbol: " << i.tradingsymbol << " & average price: " << i.averagePrice << "\n"; };*/
        //! [get holdings]

        //! [get positions]
        /*kitepp::positions pos = Kite.getPositions();
        if (!pos.day.empty()) { std::cout << "tradingsymbol: " << pos.day[0].tradingsymbol << " & quantity: " << pos.day[0].quantity << "\n"; };*/
        //! [get positions]

        //! [convert position]
        if (Kite.convertPosition("NSE", "INFY", "BUY", "overnight", 3, "NRML", "MIS")) {

            std::cout << "Successfully converted the position\n";
        } else {

            std::cout << "Failed to convert the position\n";
        };
        //! [convert position]

        //! [get quote]
        // std::cout << "get quote response: " << Kite.quote({ "NSE:INFY", "NSE:NIFTY+50", "NSE:RELIANCE" }).dump(4) << std::endl;
        //! [get quote]

        //! [get ohlc]
        // std::cout << "get ohlc response: " << Kite.ohlc({ "NSE:INFY", "NSE:NIFTY+50", "NSE:RELIANCE" }).dump(4) << std::endl;
        //! [get ohlc]

        //! [get ltp]
        // std::cout << "get ltp response: " << Kite.ltp({ "NSE:INFY", "NSE:NIFTY+50", "NSE:RELIANCE" }).dump(4) << std::endl;
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
