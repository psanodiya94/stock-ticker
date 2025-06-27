#ifndef STOCK_DATA_H
#define STOCK_DATA_H

#include <string>
#include <vector>

/**
 * @file StockData.h
 * @brief Defines the StockData structure for holding stock information.
 */

/**
 * @struct StockData
 * @brief Represents stock information for a single symbol.
 */
struct StockData {
    std::string symbol;      ///< Stock symbol
    double price = 0.0;     ///< Current price
    double change = 0.0;    ///< Price change
    double changePercent = 0.0; ///< Percentage change
    int timestamp = 0;      ///< Last update timestamp
    std::vector<double> history; ///< Price history for charting
};

/**
 * @brief Alias for a list of stock symbols to watch.
 */
typedef std::vector<std::string> Watchlist;

#endif // STOCK_DATA_H
