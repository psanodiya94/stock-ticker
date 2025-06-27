#include "StockAsciiChart_test.h"
#include "StockData.h"

TEST_F(StockAsciiChartTest, RenderEmptyHistory) {
    StockData data;
    std::string chart = StockAsciiChart::render(data.history);
    EXPECT_TRUE(chart.empty() || chart.find("*") == std::string::npos);
}
