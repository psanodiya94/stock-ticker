#include "StockData_test.h"

TEST_F(StockDataTest, DefaultValues) {
    StockData data;
    EXPECT_EQ(data.symbol, "");
    EXPECT_EQ(data.price, 0.0);
    EXPECT_EQ(data.change, 0.0);
    EXPECT_EQ(data.changePercent, 0.0);
    EXPECT_EQ(data.timestamp, 0);
    EXPECT_TRUE(data.history.empty());
}
