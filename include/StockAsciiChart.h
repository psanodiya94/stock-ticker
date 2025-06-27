#ifndef STOCK_ASCII_CHART_H
#define STOCK_ASCII_CHART_H

#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

/**
 * @file StockAsciiChart.h
 * @brief Provides ASCII chart rendering for stock price history.
 */

/**
 * @class StockAsciiChart
 * @brief Utility class for rendering stock price history as ASCII charts.
 */
class StockAsciiChart {
public:
    /**
     * @brief Renders a price history as an ASCII chart.
     * @param history The price history to render.
     * @return A string containing the ASCII chart.
     */
    static std::string render(const std::vector<double>& history);
};

#endif // STOCK_ASCII_CHART_H
