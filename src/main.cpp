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

    kite Kite("a9gg2394xe9sqmjc", "0HW0h8CsizDNmjSFjQ3F2bXhq6yVW56F");

    njson margins;

    try{
        
        margins = Kite.margins();

    }catch(kiteppException& e){

        std::cerr<<FMT("{0},{1},{2}\n", e.what(), e.code(), e.message());

    };

    std::cout<<"availible cash is "<<margins["data"]["commodity"]["available"]["cash"]<<std::endl;




    return 0;

}

