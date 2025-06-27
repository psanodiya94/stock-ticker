#include "StockTickerWidget.h"
#include "StockAsciiChart.h"
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <sstream>
#include <iomanip>

StockTickerWidget::StockTickerWidget(QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setupToolbar();
    m_table = new QTableWidget(this);
    setupTable();
    mainLayout->addWidget(m_toolbar);
    mainLayout->addWidget(m_table);
    setupStatusBar();
    mainLayout->addWidget(m_statusBar);
    setLayout(mainLayout);
    connect(&m_apiClient, &StockApiClient::stockDataUpdated,
            this, &StockTickerWidget::updateStockData);
    connect(&m_apiClient, &StockApiClient::errorOccurred,
            this, &StockTickerWidget::handleError);
}

StockTickerWidget::~StockTickerWidget() = default;

void StockTickerWidget::setupToolbar() {
    m_toolbar = new QToolBar(this);
    QAction* refreshAction = m_toolbar->addAction("Refresh");
    connect(refreshAction, &QAction::triggered, this, &StockTickerWidget::refreshData);
}

void StockTickerWidget::setupTable() {
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Symbol", "Price", "Change", "Chart"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setShowGrid(true);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet("QTableWidget { font-size: 14px; } QHeaderView::section { background: #222; color: #fff; }");
}

void StockTickerWidget::setupStatusBar() {
    m_statusBar = new QStatusBar(this);
    m_statusBar->showMessage("Ready");
}

void StockTickerWidget::setWatchlist(const Watchlist& watchlist) {
    m_apiClient.setWatchlist(watchlist);
    m_apiClient.startUpdates(5000);
}

void StockTickerWidget::updateStockData(const StockData& data) {
    m_stockData[data.symbol] = data;
    updateDisplay();
}

void StockTickerWidget::handleError(const QString& message) {
    m_statusBar->showMessage("Error: " + message, 5000);
}

void StockTickerWidget::updateDisplay() {
    m_table->setRowCount(static_cast<int>(m_stockData.size()));
    int row = 0;
    for (const auto& [symbol, data] : m_stockData) {
        QTableWidgetItem* symbolItem = new QTableWidgetItem(QString::fromStdString(symbol));
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(data.price, 'f', 2));
        QTableWidgetItem* changeItem = new QTableWidgetItem(QString::number(data.change, 'f', 2) + " (" + QString::number(data.changePercent, 'f', 2) + "%)");
        // Color for change
        if (data.change >= 0) {
            changeItem->setForeground(QBrush(QColor("#00c853")));
        } else {
            changeItem->setForeground(QBrush(QColor("#d50000")));
        }
        // ASCII chart as text
        QString chart = data.history.empty() ? "" : QString::fromStdString(StockAsciiChart::render(data.history));
        QTableWidgetItem* chartItem = new QTableWidgetItem(chart);
        m_table->setItem(row, 0, symbolItem);
        m_table->setItem(row, 1, priceItem);
        m_table->setItem(row, 2, changeItem);
        m_table->setItem(row, 3, chartItem);
        row++;
    }
}

void StockTickerWidget::refreshData() {
    m_apiClient.startUpdates(0); // Immediate update
    m_statusBar->showMessage("Refreshing...");
}

QString StockTickerWidget::formatStockInfo(const StockData& data) const {
    // Not used in new table-based UI, but kept for reference
    std::stringstream ss;
    ss << std::left << std::setw(6) << data.symbol << " "
       << std::fixed << std::setprecision(2) << data.price << " ";
    if (data.change >= 0) {
        ss << "+" << data.change << " (+" << data.changePercent << "%) ▲";
    } else {
        ss << data.change << " (" << data.changePercent << "%) ▼";
    }
    if (!data.history.empty()) {
        ss << "\n" << StockAsciiChart::render(data.history);
    }
    return QString::fromStdString(ss.str());
}
