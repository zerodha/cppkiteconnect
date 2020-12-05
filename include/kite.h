#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <utility> //pair<>
#include <algorithm> //for_each
#include <iostream> //debug

#include "config.hpp"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <nlohmann/json.hpp>

#include "kiteppexceptions.hpp"

namespace http = web::http;
using std::string;
using njson = nlohmann::json;


class kite{


public:

//member variables

string apiKey = "";
string accessToken = "";








//constructors and destructor

kite(const string& apikey, const string& accesstoken): apiKey(apikey), accessToken(accesstoken), _httpClient(U("https://api.kite.trade/")){




};







//methods


njson margins(){

    return _sendReq(http::methods::GET, _routes.at("user.margins"));

};







private:

//member variables

const string _kiteVersion = "3";
const std::unordered_map<string, string> _routes={

        {"api.token", "/session/token"},
        {"api.token.invalidate", "/session/token"},
        {"api.token.renew", "/session/refresh_token"},
        {"user.profile", "/user/profile"},
        {"user.margins", "/user/margins"},
        {"user.margins.segment", "/user/margins/{segment}"},

        {"orders", "/orders"},
        {"trades", "/trades"},

        {"order.info", "/orders/{order_id}"},
        {"order.place", "/orders/{variety}"},
        {"order.modify", "/orders/{variety}/{order_id}"},
        {"order.cancel", "/orders/{variety}/{order_id}"},
        {"order.trades", "/orders/{order_id}/trades"},

        {"portfolio.positions", "/portfolio/positions"},
        {"portfolio.holdings", "/portfolio/holdings"},
        {"portfolio.positions.convert", "/portfolio/positions"},

        //MF api endpoints
        {"mf.orders", "/mf/orders"},
        {"mf.order.info", "/mf/orders/{order_id}"},
        {"mf.order.place", "/mf/orders"},
        {"mf.order.cancel", "/mf/orders/{order_id}"},

        {"mf.sips", "/mf/sips"},
        {"mf.sip.info", "/mf/sips/{sip_id}"},
        {"mf.sip.place", "/mf/sips"},
        {"mf.sip.modify", "/mf/sips/{sip_id}"},
        {"mf.sip.cancel", "/mf/sips/{sip_id}"},

        {"mf.holdings", "/mf/holdings"},
        {"mf.instruments", "/mf/instruments"},

        //market endpoints

        {"market.instruments.all", "/instruments"},
        {"market.instruments", "/instruments/{exchange}"},
        {"market.margins", "/margins/{segment}"},
        {"market.historical", "/instruments/historical/{instrument_token}/{interval}"},
        {"market.trigger_range", "/instruments/trigger_range/{transaction_type}"},

        {"market.quote", "/quote"},
        {"market.quote.ohlc", "/quote/ohlc"},
        {"market.quote.ltp", "/quote/ltp"},

        //GTT endpoints
        {"gtt", "/gtt/triggers"},
        {"gtt.place", "/gtt/triggers"},
        {"gtt.info", "/gtt/triggers/{trigger_id}"},
        {"gtt.modify", "/gtt/triggers/{trigger_id}"},
        {"gtt.delete", "/gtt/triggers/{trigger_id}"},

        //Margin computation endpoints
        {"order.margins", "/margins/orders"}

};

http::client::http_client _httpClient;


//methods

string _getAuth() const{

    return FMT("token {0}:{1}", apiKey, accessToken);

};

njson _sendReq(http::method mtd, const string& endpoint, const std::vector<std::pair<string, string>>& headers = {}, const string& body=""){

    //create request

    http::http_request req(mtd);

    req.set_request_uri(U(endpoint));
    req.headers().add(U("Authorization"), U(_getAuth()));
    req.headers().add(U("X-Kite-Version"), U(_kiteVersion));

    std::for_each(headers.begin(), headers.end(), [&](std::pair<string, string> header){

        req.headers().add(U(header.first), U(header.second));

    });

    if ((mtd!=http::methods::GET && mtd!=http::methods::HEAD) && body != ""){

        req.set_body(U(body));

    };


    //send request, get response and parse json

    http::http_response res = _httpClient.request(req).get();
    string dataRcvd = res.extract_string().get();

    if(!dataRcvd.empty()){

        njson data;

        try
        {
            
            data = njson::parse(dataRcvd);

        }
        catch(const std::exception& e)
        {
            
            throw libException(FMT("{0} was thrown while parsing json (_sendReq-njson::parse)", e.what()).c_str());

        };

        if(res.status_code() == http::status_codes::OK){
        
            //TODO can return njson["data"] & save users a step if it is established that every ok request has `data` field. It seems it is
            return data;

        }else{

            int code = 0;
            string excpStr= "", message="";

            try{

                code = res.status_code();
                excpStr = data["error_type"];
                message = data["message"];

            }catch(const std::exception& e){

                throw libException(FMT("{0} was thrown while extracting code, excpStr and message (_sendReq)", e.what()).c_str());

            };

            throwException(excpStr, code, message);

        };


    }else{

        return njson();

    };

    return njson();
    
};



};