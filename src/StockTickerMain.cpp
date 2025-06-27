#include "StockTickerWidget.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <cstdlib>

Watchlist loadWatchlist(const QString& filename) {
    Watchlist watchlist;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        // Try environment variable if file not found
        const char* envPath = std::getenv("STOCK_TICKER_WATCHLIST_PATH");
        if (envPath) {
            QFile envFile(QString::fromLocal8Bit(envPath));
            if (envFile.open(QIODevice::ReadOnly)) {
                QByteArray data = envFile.readAll();
                QJsonDocument doc = QJsonDocument::fromJson(data);
                if (doc.isArray()) {
                    QJsonArray array = doc.array();
                    for (const auto& item : array) {
                        watchlist.push_back(item.toString().toStdString());
                    }
                }
                return watchlist;
            }
        }
        qWarning("Couldn't open watchlist file.");
        return watchlist;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (const auto& item : array) {
            watchlist.push_back(item.toString().toStdString());
        }
    }
    
    return watchlist;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    StockTickerWidget widget;
    widget.resize(800, 600);
    widget.setWindowTitle("Stock Ticker");
    
    // Load watchlist from file
    Watchlist watchlist = loadWatchlist("data/watchlist.json");
    if (watchlist.empty()) {
        // Default watchlist if file not found
        watchlist = {"AAPL", "MSFT", "GOOGL", "AMZN", "TSLA"};
    }
    
    widget.setWatchlist(watchlist);
    widget.show();
    
    return app.exec();
}
