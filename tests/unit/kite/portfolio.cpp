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

TEST(kiteTest, holdingsTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/holdings.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/portfolio/holdings" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const std::vector<kc::holding> HOLDINGS = Kite.holdings();

    ASSERT_EQ(HOLDINGS.size(), 19);
    kc::holding holding1 = HOLDINGS[0];
    EXPECT_EQ(holding1.tradingsymbol, "AXTEL");
    EXPECT_EQ(holding1.exchange, "BSE");
    EXPECT_EQ(holding1.instrumentToken, 134105604);
    EXPECT_EQ(holding1.ISIN, "INE767C01012");
    EXPECT_EQ(holding1.product, "CNC");
    EXPECT_DOUBLE_EQ(holding1.price, 0);
    EXPECT_EQ(holding1.quantity, 1);
    EXPECT_EQ(holding1.t1Quantity, 0);
    EXPECT_EQ(holding1.realisedQuantity, 1);
    EXPECT_EQ(holding1.collateralQuantity, 0);
    EXPECT_EQ(holding1.collateralType, "");
    EXPECT_DOUBLE_EQ(holding1.averagePrice, 261.5);
    EXPECT_DOUBLE_EQ(holding1.lastPrice, 313);
    EXPECT_DOUBLE_EQ(holding1.closePrice, 308.8);
    EXPECT_DOUBLE_EQ(holding1.PnL, 51.5);
    EXPECT_DOUBLE_EQ(holding1.dayChange, 4.1999999999999886);
    EXPECT_DOUBLE_EQ(holding1.dayChangePercentage, 1.3601036269430016);
}

TEST(kiteTest, getPositionsTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_responses/positions.json");
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::GET,
                                  "/portfolio/positions" },
                          utils::http::Params {}, utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const kc::positions POSITIONS = Kite.getPositions();

    ASSERT_EQ(POSITIONS.net.size(), 3);
    ASSERT_EQ(POSITIONS.day.size(), 3);
    kc::position netPosition1 = POSITIONS.net[0];
    EXPECT_EQ(netPosition1.tradingsymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(netPosition1.exchange, "MCX");
    EXPECT_EQ(netPosition1.instrumentToken, 53496327);
    EXPECT_EQ(netPosition1.product, "NRML");
    EXPECT_EQ(netPosition1.quantity, 1);
    EXPECT_EQ(netPosition1.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.multiplier, 1000);
    EXPECT_DOUBLE_EQ(netPosition1.averagePrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.closePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.lastPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.value, -161050);
    EXPECT_DOUBLE_EQ(netPosition1.PnL, 0);
    EXPECT_DOUBLE_EQ(netPosition1.M2M, 0);
    EXPECT_DOUBLE_EQ(netPosition1.unrealised, 0);
    EXPECT_DOUBLE_EQ(netPosition1.realised, 0);
    EXPECT_EQ(netPosition1.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition1.buyPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.buyValue, 161050);
    EXPECT_DOUBLE_EQ(netPosition1.buyM2MValue, 161050);
    EXPECT_EQ(netPosition1.sellQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellPrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellValue, 0);
    EXPECT_DOUBLE_EQ(netPosition1.sellM2MValue, 0);
    EXPECT_EQ(netPosition1.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition1.dayBuyPrice, 161.05);
    EXPECT_DOUBLE_EQ(netPosition1.dayBuyValue, 161050);
    EXPECT_EQ(netPosition1.daySellQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition1.daySellPrice, 0);
    EXPECT_DOUBLE_EQ(netPosition1.daySellValue, 0);

    kc::position netPosition2 = POSITIONS.net[1];
    EXPECT_EQ(netPosition2.tradingsymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(netPosition2.exchange, "MCX");
    EXPECT_EQ(netPosition2.instrumentToken, 53505799);
    EXPECT_EQ(netPosition2.product, "NRML");
    EXPECT_EQ(netPosition2.quantity, 0);
    EXPECT_EQ(netPosition2.overnightQuantity, 3);
    EXPECT_DOUBLE_EQ(netPosition2.multiplier, 1);
    EXPECT_DOUBLE_EQ(netPosition2.averagePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition2.closePrice, 23232);
    EXPECT_DOUBLE_EQ(netPosition2.lastPrice, 23355);
    EXPECT_DOUBLE_EQ(netPosition2.value, 801);
    EXPECT_DOUBLE_EQ(netPosition2.PnL, 801);
    EXPECT_DOUBLE_EQ(netPosition2.M2M, 276);
    EXPECT_DOUBLE_EQ(netPosition2.unrealised, 801);
    EXPECT_DOUBLE_EQ(netPosition2.realised, 0);
    EXPECT_EQ(netPosition2.buyQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.buyPrice, 23139.75);
    EXPECT_DOUBLE_EQ(netPosition2.buyValue, 92559);
    EXPECT_DOUBLE_EQ(netPosition2.buyM2MValue, 93084);
    EXPECT_EQ(netPosition2.sellQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.sellPrice, 23340);
    EXPECT_DOUBLE_EQ(netPosition2.sellValue, 93360);
    EXPECT_DOUBLE_EQ(netPosition2.sellM2MValue, 93360);
    EXPECT_EQ(netPosition2.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition2.dayBuyPrice, 23388);
    EXPECT_DOUBLE_EQ(netPosition2.dayBuyValue, 23388);
    EXPECT_EQ(netPosition2.daySellQuantity, 4);
    EXPECT_DOUBLE_EQ(netPosition2.daySellPrice, 23340);
    EXPECT_DOUBLE_EQ(netPosition2.daySellValue, 93360);

    kc::position netPosition3 = POSITIONS.net[2];
    EXPECT_EQ(netPosition3.tradingsymbol, "SBIN");
    EXPECT_EQ(netPosition3.exchange, "NSE");
    EXPECT_EQ(netPosition3.instrumentToken, 779521);
    EXPECT_EQ(netPosition3.product, "CO");
    EXPECT_EQ(netPosition3.quantity, 0);
    EXPECT_EQ(netPosition3.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(netPosition3.multiplier, 1);
    EXPECT_DOUBLE_EQ(netPosition3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition3.closePrice, 0);
    EXPECT_DOUBLE_EQ(netPosition3.lastPrice, 308.4);
    EXPECT_DOUBLE_EQ(netPosition3.value, -2);
    EXPECT_DOUBLE_EQ(netPosition3.PnL, -2);
    EXPECT_DOUBLE_EQ(netPosition3.M2M, -2);
    EXPECT_DOUBLE_EQ(netPosition3.unrealised, -2);
    EXPECT_DOUBLE_EQ(netPosition3.realised, 0);
    EXPECT_EQ(netPosition3.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.buyPrice, 311);
    EXPECT_DOUBLE_EQ(netPosition3.buyValue, 311);
    EXPECT_DOUBLE_EQ(netPosition3.buyM2MValue, 311);
    EXPECT_EQ(netPosition3.sellQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.sellPrice, 309);
    EXPECT_DOUBLE_EQ(netPosition3.sellValue, 309);
    EXPECT_DOUBLE_EQ(netPosition3.sellM2MValue, 309);
    EXPECT_EQ(netPosition3.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.dayBuyPrice, 311);
    EXPECT_DOUBLE_EQ(netPosition3.dayBuyValue, 311);
    EXPECT_EQ(netPosition3.daySellQuantity, 1);
    EXPECT_DOUBLE_EQ(netPosition3.daySellPrice, 309);
    EXPECT_DOUBLE_EQ(netPosition3.daySellValue, 309);

    kc::position dayPosition1 = POSITIONS.day[0];
    EXPECT_EQ(dayPosition1.tradingsymbol, "GOLDGUINEA17DECFUT");
    EXPECT_EQ(dayPosition1.exchange, "MCX");
    EXPECT_EQ(dayPosition1.instrumentToken, 53505799);
    EXPECT_EQ(dayPosition1.product, "NRML");
    EXPECT_EQ(dayPosition1.quantity, -3);
    EXPECT_EQ(dayPosition1.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition1.multiplier, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.averagePrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.closePrice, 23232);
    EXPECT_DOUBLE_EQ(dayPosition1.lastPrice, 23355);
    EXPECT_DOUBLE_EQ(dayPosition1.value, 69972);
    EXPECT_DOUBLE_EQ(dayPosition1.PnL, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.M2M, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.unrealised, -93);
    EXPECT_DOUBLE_EQ(dayPosition1.realised, 0);
    EXPECT_EQ(dayPosition1.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.buyPrice, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.buyValue, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.buyM2MValue, 23388);
    EXPECT_EQ(dayPosition1.sellQuantity, 4);
    EXPECT_DOUBLE_EQ(dayPosition1.sellPrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.sellValue, 93360);
    EXPECT_DOUBLE_EQ(dayPosition1.sellM2MValue, 93360);
    EXPECT_EQ(dayPosition1.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition1.dayBuyPrice, 23388);
    EXPECT_DOUBLE_EQ(dayPosition1.dayBuyValue, 23388);
    EXPECT_EQ(dayPosition1.daySellQuantity, 4);
    EXPECT_DOUBLE_EQ(dayPosition1.daySellPrice, 23340);
    EXPECT_DOUBLE_EQ(dayPosition1.daySellValue, 93360);

    kc::position dayPosition2 = POSITIONS.day[1];
    EXPECT_EQ(dayPosition2.tradingsymbol, "LEADMINI17DECFUT");
    EXPECT_EQ(dayPosition2.exchange, "MCX");
    EXPECT_EQ(dayPosition2.instrumentToken, 53496327);
    EXPECT_EQ(dayPosition2.product, "NRML");
    EXPECT_EQ(dayPosition2.quantity, 1);
    EXPECT_EQ(dayPosition2.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.multiplier, 1000);
    EXPECT_DOUBLE_EQ(dayPosition2.averagePrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.closePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.lastPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.value, -161050);
    EXPECT_DOUBLE_EQ(dayPosition2.PnL, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.M2M, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.unrealised, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.realised, 0);
    EXPECT_EQ(dayPosition2.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition2.buyPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.buyValue, 161050);
    EXPECT_DOUBLE_EQ(dayPosition2.buyM2MValue, 161050);
    EXPECT_EQ(dayPosition2.sellQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellPrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellValue, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.sellM2MValue, 0);
    EXPECT_EQ(dayPosition2.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition2.dayBuyPrice, 161.05);
    EXPECT_DOUBLE_EQ(dayPosition2.dayBuyValue, 161050);
    EXPECT_EQ(dayPosition2.daySellQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.daySellPrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition2.daySellValue, 0);

    kc::position dayPosition3 = POSITIONS.day[2];
    EXPECT_EQ(dayPosition3.tradingsymbol, "SBIN");
    EXPECT_EQ(dayPosition3.exchange, "NSE");
    EXPECT_EQ(dayPosition3.instrumentToken, 779521);
    EXPECT_EQ(dayPosition3.product, "CO");
    EXPECT_EQ(dayPosition3.quantity, 0);
    EXPECT_EQ(dayPosition3.overnightQuantity, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.multiplier, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.averagePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.closePrice, 0);
    EXPECT_DOUBLE_EQ(dayPosition3.lastPrice, 308.4);
    EXPECT_DOUBLE_EQ(dayPosition3.value, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.PnL, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.M2M, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.unrealised, -2);
    EXPECT_DOUBLE_EQ(dayPosition3.realised, 0);
    EXPECT_EQ(dayPosition3.buyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.buyPrice, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.buyValue, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.buyM2MValue, 311);
    EXPECT_EQ(dayPosition3.sellQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.sellPrice, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.sellValue, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.sellM2MValue, 309);
    EXPECT_EQ(dayPosition3.dayBuyQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.dayBuyPrice, 311);
    EXPECT_DOUBLE_EQ(dayPosition3.dayBuyValue, 311);
    EXPECT_EQ(dayPosition3.daySellQuantity, 1);
    EXPECT_DOUBLE_EQ(dayPosition3.daySellPrice, 309);
    EXPECT_DOUBLE_EQ(dayPosition3.daySellValue, 309);
}

TEST(kiteTest, convertPositionTest) {
    const string JSON =
        kc::test::readFile("../tests/mock_custom/convert_position.json");
    constexpr int QUNATITY = 11;
    const string EXCHANGE = "BSE";
    const string SYMBOL = "INFY";
    const string TRANSACTION_TYPE = "SELL";
    const string POSITION_TYPE = "overnight";
    const string OLD_PRODUCT = "NRML";
    const string NEW_PRODUCT = "MIS";
    constexpr bool EXPECTED_RESULT = true;
    StrictMock<kc::test::mockKite> Kite;
    EXPECT_CALL(Kite, sendReq(utils::http::endpoint { utils::http::METHOD::PUT,
                                  "/portfolio/positions" },
                          utils::http::Params {
                              { "quantity", std::to_string(QUNATITY) },
                              { "tradingsymbol", SYMBOL },
                              { "transaction_type", TRANSACTION_TYPE },
                              { "position_type", POSITION_TYPE },
                              { "exchange", EXCHANGE },
                              { "old_product", OLD_PRODUCT },
                              { "new_product", NEW_PRODUCT },
                          },
                          utils::FmtArgs {}))
        .Times(1)
        .WillOnce(
            Return(ByMove(utils::http::response(utils::http::code::OK, JSON))));

    const bool RESULT =
        Kite.convertPosition(kc::convertPositionParams()
                                 .Quantity(QUNATITY)
                                 .Exchange(EXCHANGE)
                                 .Symbol(SYMBOL)
                                 .TransactionType(TRANSACTION_TYPE)
                                 .PositionType(POSITION_TYPE)
                                 .OldProduct(OLD_PRODUCT)
                                 .NewProduct(NEW_PRODUCT));

    EXPECT_EQ(RESULT, EXPECTED_RESULT);
}
