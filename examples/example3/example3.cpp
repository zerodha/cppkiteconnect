#include <iostream>

#include "kitepp/kitews.hpp"

void onConnect(kitepp::kiteWS* ws) {

    // ws->subscribe({ 408065 });
    ws->setMode("full", { 408065, 2953217 });

    std::cout << "connected..\n";
};

// unsigned int tickCount = 0;
void onTicks(kitepp::kiteWS* ws, const std::vector<kitepp::tick>& ticks) {

    std::cout << "onTicks called\n";

    // tickCount++;
    for (const auto& i : ticks) {

        std::cout << "instrument token: " << i.instrumentToken << " last price: " << i.lastPrice << "\n";
    };

    // if (tickCount >= 100) { ws->stop(); };
};

void onOrderUpdate(kitepp::kiteWS* ws, const kitepp::postback& postback) {

    std::cout << "trading symbol: " << postback.tradingSymbol << " status: " << postback.status
              << " filled quantity: " << postback.filledQuantity << "\n";
};

void onMessage(kitepp::kiteWS* ws, const std::string& message) { std::cout << "Received message: " << message << "\n"; }

void onError(kitepp::kiteWS* ws, int code, const std::string& message) {

    std::cout << "Error! Code: " << code << " message: " << message << "\n";
};

void onConnectError(kitepp::kiteWS* ws) { std::cout << "Couldn't connect..\n"; };

void onTryReconnect(kitepp::kiteWS* ws, unsigned int attemptCount) {

    std::cout << "Trying to reconnect.. Attempt " << attemptCount << "\n";
};

void onReconnectFail(kitepp::kiteWS* ws) { std::cout << "Failed to reconnect!\n"; }

void onClose(kitepp::kiteWS* ws, int code, const std::string& message) {
    std::cout << "onClose: Closed the connection.. code: " << code << " message: " << message << "\n";
}

int main(int argc, char const* argv[]) {

    kitepp::kiteWS kWS("a9gg2394xe9sqmjc", 5, true, 5);

    kWS.setAccessToken("bALpnlJSC6CHgK6fZ54lu2V0UYCoytmP");
    kWS.onConnect = onConnect;
    kWS.onTicks = onTicks;
    kWS.onOrderUpdate = onOrderUpdate;
    kWS.onMessage = onMessage;
    kWS.onError = onError;
    kWS.onConnectError = onConnectError;
    kWS.onTryReconnect = onTryReconnect;
    kWS.onReconnectFail = onReconnectFail;
    kWS.onClose = onClose;

    kWS.connect();

    kWS.run();

    kWS.stop();

    //!

    /*uWS::Hub hub;
    uWS::Group<uWS::CLIENT>* hubGroup;
    std::unique_ptr<uS::Async> hubTrigger;

    hubGroup = hub.createGroup<uWS::CLIENT>();

    uWS::WebSocket<uWS::CLIENT>* wsPtr = nullptr;

    hubGroup->onConnection([&](uWS::WebSocket<uWS::CLIENT>* ws, uWS::HttpRequest req) {
        wsPtr = ws;
        std::cout << "connect, sending HELLO" << std::endl;
        std::string msg = "HELLO";
        ws->send(msg.data(), msg.size(), uWS::OpCode::TEXT);
    });

    hubGroup->onMessage([](uWS::WebSocket<uWS::CLIENT>* ws, char* message, size_t length, uWS::OpCode opCode) {
        std::cout << "Got reply: " << std::string(message, length) << std::endl;
        std::string msg = "HELLO";
        ws->send(msg.data(), msg.size(), uWS::OpCode::TEXT);
        // ws->terminate();
    });

    hubGroup->onDisconnection([&](uWS::WebSocket<uWS::CLIENT>* ws, int code, char* message, size_t length) {
        wsPtr = nullptr;
        std::cout << "Disconnect." << std::endl;
    });

    hubGroup->onError([&](void*) {
        if (wsPtr) { wsPtr->close(); };
        std::cout << "onError called\n";
        std::cout << "Calling connect()\n";

        // hubGroup = hub.createGroup<uWS::CLIENT>();
        // hubGroup->onMessage([](uWS::WebSocket<uWS::CLIENT>* ws, char* message, size_t length, uWS::OpCode opCode) {
        //     std::cout << "New callback..\n";
        // });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    hub.connect("wss://echo.websocket.org", nullptr, {}, 5000, hubGroup);
});

hub.connect("wss://echo.websocket.org", nullptr, {}, 5000, hubGroup);

hub.run();
*/

    //!

    // return 0;
};
