#include <string>
#include <utility>
#include <vector>

#include <gmock/gmock-actions.h>
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock-generated-function-mockers.h>
#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../kitepp.hpp"
#include "../utils.hpp"

using std::string;
using ::testing::ByMove;
using ::testing::Return;
using ::testing::StrictMock;
namespace kc = kiteconnect;
namespace utils = kc::internal::utils;

TEST(kiteTest, placeGttTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/gtt_place_order.json");
    constexpr double LAST_PRICE = 798.0;
    const string TRIGGER_TYPE = "single";
    const string SYMBOL = "INFY";
    const string EXCHANGE = "NSE";
    const std::vector<double> TRIGGER_VALUES = { 702.0 };
    constexpr int GTT_PARAM1_QUANTITY = 1;
    constexpr double GTT_PARAM1_PRICE = 702.5;
    const string GTT_PARAM1_TRANSACTON_TYPE = "BUY";
    const string GTT_PARAM1_ORDER_TYPE = "LIMIT";
    const string GTT_PARAM1_PRODUCT = "CNC";
    constexpr int EXPECTED_TRIGGER_ID = 123;
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::POST, "/gtt/triggers" },
            utils::http::Params {
                { "type", "single" },
                { "condition",
                    R"({"exchange":"NSE","tradingsymbol":"INFY","trigger_values":[702.0],"last_price":798.0})" },
                { "orders",
                    R"([{"exchange":"NSE","tradingsymbol":"INFY","transaction_type":"BUY","quantity":1,"order_type":"LIMIT","product":"CNC","price":702.5}])" },
            },
            utils::FmtArgs {}))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    // clang-format off
    const int TRIGGER_ID = Kite.placeGtt(kc::placeGttParams()
                                    .LastPrice(LAST_PRICE)
                                    .TriggerType(TRIGGER_TYPE)
                                    .Symbol(SYMBOL)
                                    .Exchange(EXCHANGE)
                                    .TriggerValues(TRIGGER_VALUES)
                                    .GttParamsList({
                                        kc::gttParams()
                                        .Quantity(GTT_PARAM1_QUANTITY)
                                        .Price(GTT_PARAM1_PRICE)
                                        .TransactionType(GTT_PARAM1_TRANSACTON_TYPE)
                                        .OrderType(GTT_PARAM1_ORDER_TYPE)
                                        .Product(GTT_PARAM1_PRODUCT)
                                    }));
    // clang-format on

    EXPECT_EQ(TRIGGER_ID, EXPECTED_TRIGGER_ID);
}

TEST(kiteTest, getGTTsTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/gtt_get_orders.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/gtt/triggers" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::GTT> Triggers = Kite.triggers();

    ASSERT_EQ(Triggers.size(), 2);
    kc::GTT order1 = Triggers[0];
    EXPECT_EQ(order1.ID, 112127);
    EXPECT_EQ(order1.userID, "XX0000");
    EXPECT_EQ(order1.type, "single");
    EXPECT_EQ(order1.createdAt, "2019-09-12 13:25:16");
    EXPECT_EQ(order1.updatedAt, "2019-09-12 13:25:16");
    EXPECT_EQ(order1.expiresAt, "2020-09-12 13:25:16");
    EXPECT_EQ(order1.status, "active");
    EXPECT_EQ(order1.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order1.condition.lastPrice, 798);
    EXPECT_EQ(order1.condition.tradingsymbol, "INFY");
    EXPECT_DOUBLE_EQ(order1.condition.triggerValues[0], 702);
    EXPECT_EQ(order1.orders[0].exchange, "NSE");
    EXPECT_EQ(order1.orders[0].tradingSymbol, "INFY");
    EXPECT_EQ(order1.orders[0].product, "CNC");
    EXPECT_EQ(order1.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order1.orders[0].transactionType, "BUY");
    EXPECT_EQ(order1.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order1.orders[0].price, 702.5);

    kc::GTT order2 = Triggers[1];
    EXPECT_EQ(order2.ID, 105099);
    EXPECT_EQ(order2.userID, "XX0000");
    EXPECT_EQ(order2.type, "two-leg");
    EXPECT_EQ(order2.createdAt, "2019-09-09 15:13:22");
    EXPECT_EQ(order2.updatedAt, "2019-09-09 15:15:08");
    EXPECT_EQ(order2.expiresAt, "2020-01-01 12:00:00");
    EXPECT_EQ(order2.status, "triggered");
    EXPECT_EQ(order2.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(order2.condition.lastPrice, 102.6);
    EXPECT_EQ(order2.condition.tradingsymbol, "RAIN");
    EXPECT_THAT(order2.condition.triggerValues, ::testing::ElementsAre(102.0, 103.7));
    EXPECT_EQ(order2.orders[0].tradingSymbol, "RAIN");
    EXPECT_EQ(order2.orders[0].product, "CNC");
    EXPECT_EQ(order2.orders[0].orderType, "LIMIT");
    EXPECT_EQ(order2.orders[0].transactionType, "SELL");
    EXPECT_EQ(order2.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(order2.orders[0].price, 1);
    EXPECT_EQ(order2.orders[1].tradingSymbol, "RAIN");
    EXPECT_EQ(order2.orders[1].product, "CNC");
    EXPECT_EQ(order2.orders[1].orderType, "LIMIT");
    EXPECT_EQ(order2.orders[1].transactionType, "SELL");
    EXPECT_EQ(order2.orders[1].quantity, 1);
    EXPECT_DOUBLE_EQ(order2.orders[1].price, 1);
}

TEST(kiteTest, getGTTTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/gtt_get_order.json");
    constexpr int TRIGGER_ID = 123;
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET, "/gtt/triggers/{0}" },
                          utils::http::Params {}, utils::FmtArgs { std::to_string(TRIGGER_ID) }))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const kc::GTT trigger = Kite.getGtt(TRIGGER_ID);
    EXPECT_EQ(trigger.ID, 123);
    EXPECT_EQ(trigger.userID, "XX0000");
    EXPECT_EQ(trigger.type, "two-leg");
    EXPECT_EQ(trigger.createdAt, "2019-09-09 15:13:22");
    EXPECT_EQ(trigger.updatedAt, "2019-09-09 15:15:08");
    EXPECT_EQ(trigger.expiresAt, "2020-01-01 12:00:00");
    EXPECT_EQ(trigger.status, "triggered");
    EXPECT_EQ(trigger.condition.exchange, "NSE");
    EXPECT_DOUBLE_EQ(trigger.condition.lastPrice, 102.6);
    EXPECT_EQ(trigger.condition.tradingsymbol, "RAIN");
    EXPECT_THAT(trigger.condition.triggerValues, ::testing::ElementsAre(102.0, 103.7));
    EXPECT_EQ(trigger.orders[0].tradingSymbol, "RAIN");
    EXPECT_EQ(trigger.orders[0].product, "CNC");
    EXPECT_EQ(trigger.orders[0].orderType, "LIMIT");
    EXPECT_EQ(trigger.orders[0].transactionType, "SELL");
    EXPECT_EQ(trigger.orders[0].quantity, 1);
    EXPECT_DOUBLE_EQ(trigger.orders[0].price, 1);
}

TEST(kiteTest, modifyGttTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/gtt_modify_order.json");
    constexpr double LAST_PRICE = 798.0;
    const string TRIGGER_TYPE = "single";
    const string SYMBOL = "INFY";
    const string EXCHANGE = "NSE";
    const std::vector<double> TRIGGER_VALUES = { 702.0 };
    constexpr int GTT_PARAM1_QUANTITY = 2;
    constexpr double GTT_PARAM1_PRICE = 702.5;
    const string GTT_PARAM1_TRANSACTON_TYPE = "BUY";
    const string GTT_PARAM1_ORDER_TYPE = "LIMIT";
    const string GTT_PARAM1_PRODUCT = "CNC";
    constexpr int TRIGGER_ID = 123;
    constexpr int EXPECTED_TRIGGER_ID = 123;
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite,
        sendReq(utils::http::endpoint { utils::http::METHOD::PUT, "/gtt/triggers/{0}" },
            utils::http::Params {
                { "type", "single" },
                { "condition",
                    R"({"exchange":"NSE","tradingsymbol":"INFY","trigger_values":[702.0],"last_price":798.0})" },
                { "orders",
                    R"([{"exchange":"NSE","tradingsymbol":"INFY","transaction_type":"BUY","quantity":2,"order_type":"LIMIT","product":"CNC","price":702.5}])" },
            },
            utils::FmtArgs { std::to_string(TRIGGER_ID) }))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const int RECEIVED_TRIGGER_ID = Kite.modifyGtt(kc::modifyGttParams()
                                                       .TriggerId(TRIGGER_ID)
                                                       .LastPrice(LAST_PRICE)
                                                       .TriggerType(TRIGGER_TYPE)
                                                       .Symbol(SYMBOL)
                                                       .Exchange(EXCHANGE)
                                                       .TriggerValues(TRIGGER_VALUES)
                                                       .GttParamsList({ kc::gttParams()
                                                                            .Quantity(GTT_PARAM1_QUANTITY)
                                                                            .Price(GTT_PARAM1_PRICE)
                                                                            .TransactionType(GTT_PARAM1_TRANSACTON_TYPE)
                                                                            .OrderType(GTT_PARAM1_ORDER_TYPE)
                                                                            .Product(GTT_PARAM1_PRODUCT) }));

    EXPECT_EQ(RECEIVED_TRIGGER_ID, EXPECTED_TRIGGER_ID);
}

TEST(kiteTest, deleteGTTTest) {
    const string JSON = kc::test::readFile("../tests/mock_responses/gtt_delete_order.json");
    constexpr int TRIGGER_ID = 123;
    constexpr int EXPECTED_TRIGGER_ID = 123;
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::DEL, "/gtt/triggers/{0}" },
                          utils::http::Params {}, utils::FmtArgs { std::to_string(TRIGGER_ID) }))
        .Times(1)
        .WillOnce(Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const int RECEIVED_TRIGGER_ID = Kite.deleteGtt(TRIGGER_ID);

    EXPECT_EQ(RECEIVED_TRIGGER_ID, EXPECTED_TRIGGER_ID);
}
