#include "StockTickerWidget.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

Watchlist loadWatchlist(const QString& filename) {
    Watchlist watchlist;
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
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
