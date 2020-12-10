#include <iostream>

#include <chrono>
#include <thread>

#include "kitepp.hpp"

/*

#include <map>
#include <string>

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

#include <nlohmann/json.hpp>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using jsonn = nlohmann::json;


pplx::task<http_response> makeTaskReq(http_client& client, method mtd, json::value const& jvalue){


   return (mtd == methods::GET || mtd == methods::HEAD) ?
      client.request(mtd, U("/api/users/2")) :
      client.request(mtd, U("/api/users/2"), jvalue);


};

void sendReq(http_client& client, method mtd, json::value const& jvalue){


    makeTaskReq(client, mtd, jvalue)
    .then([](http_response res){

        if(res.status_code()==status_codes::OK){

            res.extract_string()
            .then([](std::string res){

                auto j = jsonn::parse(res);

                std::cout<<j.dump(4);

            });


        }else{

            //return pplx::task_from_result(json::value());

        }

    })
    .wait();



}


int main(int argc, char* argv[])
{


    http_client client(U("https://reqres.in/"));

    sendReq(client, methods::GET, json::value());

    std::cout<<"done";




}



*/


int main(int argc, char const* argv[]) {

    kitepp::kite Kite("a9gg2394xe9sqmjc");
    Kite.setAccessToken("YI8oUa4Y8dN7Q7tAd4GLEKsvJIaitgCp");

    kitepp::njson margins, profile, misOrd, bracketOrd, coverOrd, modOrd, cancelOrd, orders, ordHistory, trades, ordTrades, exitOrd, placegtt,
        getGtts, getGtt, gttMod, delGtt, holdings, positions;

    try {

        // profile = Kite.profile();
        // margins = Kite.margins("equity");
        // misOrd = Kite.placeOrder("regular", "NSE", "BHEL", "SELL", "1", "MIS", "MARKET", "34.90", "DAY");
        // bracketOrd = Kite.placeOrder("bo", "NSE", "ACC", "BUY", "1", "MIS", "LIMIT", "1665.95", "DAY", "", "2", "2", "");
        // coverOrd = Kite.placeOrder("co", "NSE", "BHEL", "BUY", "1", "MIS", "LIMIT", "33", "DAY", "32", "", "", "");
        // modOrd = Kite.modifyOrder("regular", "151220000000000", "", "2", "", "MARKET", "", "DAY");
        // modOrd = Kite.modifyOrder("co", "201207001896036", "", "", "34.8", "LIMIT", "33", "DAY");
        // cancelOrd = Kite.cancelOrder("regular", "151220000000000", "123");
        // exitOrd = Kite.exitOrder("co", "201207001972149");
        // orders = Kite.orders();
        // ordHistory = Kite.orderHistory("201207001459491");
        // trades = Kite.trades();
        // ordTrades = Kite.orderTrades("201207001459491");

        /*
        auto gttOrds = njson::array();
        gttOrds.push_back({

        {"transaction_type", "BUY"},
        {"quantity", 1},
        {"order_type", "LIMIT"},
        {"product", "CNC"},
        {"price", 19100},

        });
        placegtt = Kite.placeGTT("single", "NESTLEIND", "NSE", {19000}, "17596.95", gttOrds);
        */

        /*
        auto gttOrds = njson::array();
        gttOrds.push_back({

        {"transaction_type", "BUY"},
        {"quantity", 1},
        {"order_type", "LIMIT"},
        {"product", "CNC"},
        {"price", 19100},

        });
        gttMod = Kite.modifyGTT("21105917", "single", "NESTLEIND", "NSE", {20000}, "17596.95", gttOrds);
        */

        // getGtts = Kite.getGTTs();
        // getGtt = Kite.getGTT("21105917");
        // delGtt = Kite.deleteGTT("21105917");

        // std::cout<<Kite.holdings().dump(4)<<std::endl;
        // std::cout<<Kite.positions().dump(4)<<std::endl;
        // std::cout<<Kite.convertPosition("NSE", "INFY", "BUY", "overnight", "3", "NRML", "MIS").dump(4)<<std::endl;
        // std::cout<<Kite.quote({"NSE:INFY", "NSE:NIFTY+50", "BSE:RELIANCE"}).dump(4)<<std::endl;
        // std::cout<<Kite.ohlc({"NSE:INFY", "NSE:NIFTY+50", "BSE:RELIANCE"}).dump(4)<<std::endl;
        // std::cout<<Kite.ltp({"NSE:INFY", "NSE:NIFTY+50", "BSE:RELIANCE"}).dump(4)<<std::endl;
        // std::cout<<Kite.historicalData("5633", "2020-11-01+09:30:00", "2020-12-01+10:30:00", "minute").dump(4)<<std::endl;
        // std::cout<<Kite.placeMFOrder("INF174K01LS2", "BUY", "", "10000").dump(4)<<std::endl;
        // std::cout<<Kite.MFOrder("724c8a20-5bad-45f6-bb42-c8fbc532695a").dump(4)<<std::endl;
        // std::cout<<Kite.MFOrders().dump(4)<<std::endl;
        // std::cout<<Kite.cancelMFOrder("724c8a20-5bad-45f6-bb42-c8fbc532695a").dump(4)<<std::endl;
        // std::cout<<Kite.placeMFSIP("INF174K01LS2", "10000", "-1", "monthly", "50000", "1").dump(4)<<std::endl;
        // std::cout<<Kite.modifyMFSIP("665078328772975", "5000", "paused", "10", "monthly", "5").dump(4)<<std::endl;
        // std::cout<<Kite.SIP("665078328772975").dump(4)<<std::endl;
        // std::cout<<Kite.cancelMFSIP("665078328772975").dump(4)<<std::endl;
        // std::cout<<Kite.SIPs().dump(4)<<std::endl;
        // std::cout<<Kite.MFHoldings().dump(4)<<std::endl;
        // std::cout<<Kite.generateSession("5HQbkFLlVvrFtZUtOXcwDxG4UmJhhu1G", "wqk7riwa1gunamv89zx111qaen0iwe2i").dump(4)<<std::endl;
        // std::cout<<Kite.invalidateSession().dump(4)<<std::endl;
        // std::cout<<Kite.instruments("NSE")<<std::endl;
        std::cout << Kite.MFInstruments() << std::endl;


        /*
        auto ords = njson::array();
        ords.push_back({

        {"exchange", "NSE"},
        {"tradingsymbol", "INFY"},
        {"transaction_type", "BUY"},
        {"variety", "regular"},
        {"product", "CNC"},
        {"order_type", "MARKET"},
        {"quantity", 1},
        {"price", 0},
        {"trigger_price", 0}

        });

        std::cout<<Kite.orderMargins(ords).dump(4)<<std::endl;
        */


    } catch (kitepp::kiteppException& e) { std::cerr << FMT("{0},{1},{2}\n", e.what(), e.code(), e.message()); } catch (kitepp::libException& e) {

        std::cerr << FMT("{0}\n", e.what());
    };

    // std::cout<<"availible cash is "<<margins["data"]["commodity"]["available"]["cash"]<<std::endl; //margins()
    // std::cout<<"availible cash is "<<margins["data"]["available"]["cash"]<<std::endl; //margins(segment)
    // std::cout<<misOrd.dump(4)<<std::endl;
    // std::cout<<modOrd.dump(4)<<std::endl;
    // std::cout<<cancelOrd.dump(4)<<std::endl;
    // std::cout<<exitOrd.dump(4)<<std::endl;
    // std::cout<<orders.dump(4)<<std::endl;
    // std::cout<<ordHistory.dump(4)<<std::endl;
    // std::cout<<trades.dump(4)<<std::endl;
    // std::cout<<ordTrades.dump(4)<<std::endl;
    // std::cout<<placegtt.dump(4)<<std::endl;
    // std::cout<<getGtts.dump(4)<<std::endl;
    // std::cout<<getGtt.dump(4)<<std::endl;
    // std::cout<<gttMod.dump(4)<<std::endl;
    // std::cout<<delGtt.dump(4)<<std::endl;


    return 0;
}
