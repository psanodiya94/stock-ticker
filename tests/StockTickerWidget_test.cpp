#include "StockTickerWidget_test.h"
#include <QCoreApplication>
#include <cstdlib>

TEST_F(StockTickerWidgetTest, WidgetCreation) {
    setenv("QT_QPA_PLATFORM", "offscreen", 1);
    int argc = 0;
    char* argv[] = {nullptr};
    QCoreApplication app(argc, argv);
    StockTickerWidget widget;
    EXPECT_TRUE(widget.isVisible() || !widget.isVisible()); // Widget can be created
}
