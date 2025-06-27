#include "StockAsciiChart.h"
#include <sstream>

std::string StockAsciiChart::render(const std::vector<double>& history) {
    if (history.empty()) return "";
    double minVal = *std::min_element(history.begin(), history.end());
    double maxVal = *std::max_element(history.begin(), history.end());
    int width = 50;
    int height = 5;
    std::vector<std::string> chart(height, std::string(width, ' '));
    for (size_t i = 0; i < history.size() && i < (size_t)width; ++i) {
        int y = (int)((history[i] - minVal) / (maxVal - minVal + 1e-9) * (height - 1));
        chart[height - 1 - y][i] = '*';
    }
    std::ostringstream oss;
    for (const auto& row : chart) {
        oss << row << '\n';
    }
    return oss.str();
}
