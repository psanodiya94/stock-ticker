#include "StockApiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <algorithm>

StockApiClient::StockApiClient(QObject* parent) 
    : QObject(parent) {
    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, &StockApiClient::onReplyFinished);
}

StockApiClient::~StockApiClient() = default;

void StockApiClient::fetchStockData(const std::string& symbol) {
    QString url = QString("https://query1.finance.yahoo.com/v8/finance/chart/%1?interval=1m&range=1d")
                  .arg(QString::fromStdString(symbol));
    
    QNetworkRequest request(url);
    m_networkManager.get(request);
}

void StockApiClient::setWatchlist(const Watchlist& watchlist) {
    m_watchlist = watchlist;
}

void StockApiClient::startUpdates(int intervalMs) {
    m_updateTimer.setInterval(intervalMs);
    connect(&m_updateTimer, &QTimer::timeout, this, &StockApiClient::onTimeout);
    m_updateTimer.start();
    // Immediately fetch first set of data
    onTimeout();
}

void StockApiClient::stopUpdates() {
    m_updateTimer.stop();
}

void StockApiClient::onReplyFinished(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return;
    }
    
    QByteArray response = reply->readAll();
    std::string symbol = reply->url().toString().split('/').last().toStdString();
    parseResponse(response, symbol);
    
    reply->deleteLater();
}

void StockApiClient::onTimeout() {
    for (const auto& symbol : m_watchlist) {
        fetchStockData(symbol);
    }
}

void StockApiClient::parseResponse(const QByteArray& response, const std::string& symbol) {
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull()) {
        emit errorOccurred("Invalid JSON response");
        return;
    }

    QJsonObject chart = doc.object()["chart"].toObject();
    QJsonArray resultArray = chart["result"].toArray();
    if (resultArray.isEmpty()) {
        emit errorOccurred("No result in response");
        return;
    }
    QJsonObject result = resultArray.first().toObject();
    QJsonObject meta = result["meta"].toObject();
    QJsonArray quoteArray = result["indicators"].toObject()["quote"].toArray();
    if (quoteArray.isEmpty()) {
        emit errorOccurred("No quote data in response");
        return;
    }
    QJsonObject indicators = quoteArray.first().toObject();
    QJsonArray closePrices = indicators["close"].toArray();

    StockData data;
    data.symbol = symbol;
    data.price = meta["regularMarketPrice"].toDouble();
    data.change = meta["regularMarketChange"].toDouble();
    data.changePercent = meta["regularMarketChangePercent"].toDouble();
    data.timestamp = meta["regularMarketTime"].toInt();

    // Get last 10 close prices for chart
    int startIdx = std::max<int>(0, closePrices.size() - 10);
    for (int i = startIdx; i < closePrices.size(); ++i) {
        if (!closePrices[i].isNull())
            data.history.push_back(closePrices[i].toDouble());
    }

    emit stockDataUpdated(data);
}
