#ifndef STOCK_API_CLIENT_TEST_H
#define STOCK_API_CLIENT_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "StockApiClient.h"

// Mock classes and test fixture declarations for StockApiClient
class MockStockApiClient : public StockApiClient {
public:
    using StockApiClient::StockApiClient;
    MOCK_METHOD(void, fetchStockData, (const std::string& symbol), (override));
    MOCK_METHOD(void, setWatchlist, (const Watchlist& watchlist), (override));
    MOCK_METHOD(void, startUpdates, (int intervalMs), (override));
    MOCK_METHOD(void, stopUpdates, (), (override));
};

class StockApiClientTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

#endif // STOCK_API_CLIENT_TEST_H
