#ifndef STOCK_TICKER_WIDGET_H
#define STOCK_TICKER_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QHBoxLayout>
#include <QTimer>
#include <unordered_map>
#include <map>
#include "StockData.h"
#include "StockApiClient.h"

/**
 * @file StockTickerWidget.h
 * @brief Declares the StockTickerWidget class for displaying stock data in a GUI.
 */

/**
 * @class StockTickerWidget
 * @brief Main widget for displaying stock data and charts.
 */
class StockTickerWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructs a StockTickerWidget.
     * @param parent The parent widget.
     */
    explicit StockTickerWidget(QWidget* parent = nullptr);
    ~StockTickerWidget();

    /**
     * @brief Sets the watchlist of stock symbols.
     * @param watchlist The list of stock symbols to watch.
     */
    void setWatchlist(const Watchlist& watchlist);

private slots:
    /**
     * @brief Updates the display when new stock data arrives.
     * @param data The updated stock data.
     */
    void updateStockData(const StockData& data);
    /**
     * @brief Handles errors from the API client.
     * @param message The error message.
     */
    void handleError(const QString& message);
    /**
     * @brief Updates the display widget.
     */
    void updateDisplay();
    /**
     * @brief Refreshes the stock data.
     */
    void refreshData();

private:
    StockApiClient m_apiClient; ///< API client for fetching stock data.
    QTableWidget* m_table; ///< Table for stock info.
    QToolBar* m_toolbar; ///< Toolbar for actions.
    QStatusBar* m_statusBar; ///< Status bar for messages.
    Watchlist m_watchlist; ///< List of stock symbols to watch.
    std::map<std::string, StockData> m_stockData; ///< Map of symbol to StockData.

    /**
     * @brief Formats stock info for display.
     * @param data The stock data.
     * @return Formatted string.
     */
    QString formatStockInfo(const StockData& data) const;

    /**
     * @brief Sets up the table for displaying stock information.
     */
    void setupTable();
    /**
     * @brief Sets up the toolbar for actions.
     */
    void setupToolbar();
    /**
     * @brief Sets up the status bar for messages.
     */
    void setupStatusBar();
};

#endif // STOCK_TICKER_WIDGET_H