#include <iostream>
#include <kite.h>

#include <thread>
#include <chrono>


/*

#include <map>
#include <string>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

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





int main(int argc, char const *argv[])
{

    kite Kite("a9gg2394xe9sqmjc", "w0cx403CwKLv3VOEAFro8z2x8XSD1put");

    njson margins, profile, misOrd, bracketOrd, modOrd, cancelOrd, orders, ordHistory, trades, ordTrades;

    try{
        
        //profile = Kite.profile();
        //margins = Kite.margins("equity");
        //FIXME misOrd = Kite.placeOrder("regular", "NSE", "ACC", "BUY", "1", "MIS", "MARKET", "", "DAY");
        //FIXME bracketOrd = Kite.placeOrder("bo", "NSE", "ACC", "BUY", "1", "MIS", "LIMIT", "1665.95", "DAY", "", "2", "2", "");
        //FIXME modOrd = Kite.modifyOrder("regular", "151220000000000", "", "2", "", "MARKET", "", "DAY");
        //FIXME cancelOrd = Kite.cancelOrder("regular", "151220000000000", "123");
        //FIXME orders = Kite.orders();
        //FIXME ordHistory = Kite.orderHistory("151220000000000");
        trades = Kite.trades();
        ordTrades = Kite.orderTrades("123456789");

    }catch(kiteppException& e){

        std::cerr<<FMT("{0},{1},{2}\n", e.what(), e.code(), e.message());

    };

    bool d = (margins.empty())? true:false;
    

    //?std::cout<<"availible cash is "<<margins["data"]["commodity"]["available"]["cash"]<<std::endl; //margins()
    std::cout<<"availible cash is "<<margins["data"]["available"]["cash"]<<std::endl; //margins(segment)
    //FIXME std::cout<<misOrd.dump(4)<<std::endl;
    //FIXME std::cout<<bracketOrd.dump(4)<<std::endl;
    //FIXME std::cout<<modOrd.dump(4)<<std::endl;
    //FIXME std::cout<<cancelOrd.dump(4)<<std::endl;
    //FIXME std::cout<<orders.dump(4)<<std::endl;
    //FIXME std::cout<<ordHistory.dump(4)<<std::endl;
    std::cout<<trades.dump(4)<<std::endl;
    std::cout<<ordTrades.dump(4)<<std::endl;




    






    return 0;

}

