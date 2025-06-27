#ifndef STOCK_API_CLIENT_H
#define STOCK_API_CLIENT_H

/**
 * @file StockApiClient.h
 * @brief Provides the StockApiClient class for fetching and updating stock data from an API.
 */

#include "StockData.h"
#include <QObject>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

/**
 * @class StockApiClient
 * @brief Handles fetching stock data and periodic updates from a remote API.
 */
class StockApiClient : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Constructs a StockApiClient object.
     * @param parent The parent QObject.
     */
    explicit StockApiClient(QObject* parent = nullptr);
    virtual ~StockApiClient();
    
    /**
     * @brief Fetches stock data for a given symbol.
     * @param symbol The stock symbol to fetch data for.
     */
    virtual void fetchStockData(const std::string& symbol);
    /**
     * @brief Sets the watchlist of stock symbols.
     * @param watchlist The list of stock symbols to watch.
     */
    virtual void setWatchlist(const Watchlist& watchlist);
    /**
     * @brief Starts periodic updates for the watchlist.
     * @param intervalMs Update interval in milliseconds (default 5000).
     */
    virtual void startUpdates(int intervalMs = 5000);
    /**
     * @brief Stops periodic updates.
     */
    virtual void stopUpdates();
    
signals:
    /**
     * @brief Emitted when stock data is updated.
     * @param data The updated stock data.
     */
    void stockDataUpdated(const StockData& data);
    /**
     * @brief Emitted when an error occurs.
     * @param message The error message.
     */
    void errorOccurred(const QString& message);
    
private slots:
    /**
     * @brief Handles finished network replies.
     * @param reply The finished QNetworkReply.
     */
    void onReplyFinished(QNetworkReply* reply);
    /**
     * @brief Handles update timer timeouts.
     */
    void onTimeout();
    
private:
    QNetworkAccessManager m_networkManager; ///< Manages network requests.
    QTimer m_updateTimer; ///< Timer for periodic updates.
    Watchlist m_watchlist; ///< List of stock symbols to watch.
    
    /**
     * @brief Parses the API response and emits stockDataUpdated.
     * @param response The raw API response.
     * @param symbol The stock symbol.
     */
    void parseResponse(const QByteArray& response, const std::string& symbol);
};

#endif // STOCK_API_CLIENT_H
