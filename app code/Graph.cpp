#include "Graph.h"
#include <iostream>
#include <algorithm>

Graph::Graph(const std::string& id) : GUIComponent(id) {}

void Graph::addDataPoint(float x, float y) {
    dataPoints.emplace_back(x, y);
}

void Graph::removeDataPoint(float x, float y) {
    dataPoints.erase(std::remove(dataPoints.begin(), dataPoints.end(),
                                 std::make_pair(x, y)), dataPoints.end());
}

void Graph::updateDataPoint(float oldX, float oldY, float newX, float newY) {
    auto it = std::find(dataPoints.begin(), dataPoints.end(),
                        std::make_pair(oldX, oldY));
    if (it != dataPoints.end()) {
        *it = {newX, newY};
    }
}

void Graph::draw() const {
    // TODO !!!
    // draw graph with data points
}