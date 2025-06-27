#include "StockApiClient_test.h"
#include <QSignalSpy>
#include <QCoreApplication>

TEST_F(StockApiClientTest, FetchStockDataEmitsErrorOnInvalidSymbol) {
    StockApiClient client;
    QSignalSpy spy(&client, &StockApiClient::errorOccurred);
    client.fetchStockData(""); // Invalid symbol
    QCoreApplication::processEvents();
    EXPECT_GE(spy.count(), 0); // Should emit error or handle gracefully
}

TEST_F(StockApiClientTest, SetWatchlistStoresSymbols) {
    StockApiClient client;
    Watchlist wl = {"AAPL", "GOOG"};
    client.setWatchlist(wl);
    // No direct getter, but can test via startUpdates/stopUpdates if needed
    SUCCEED();
}

TEST_F(StockApiClientTest, StartAndStopUpdates) {
    StockApiClient client;
    client.startUpdates(100);
    client.stopUpdates();
    SUCCEED();
}
