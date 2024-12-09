#ifndef PROJECTST_GRAPH_H
#define PROJECTST_GRAPH_H

#include "GUIComponent.h"
#include <vector>

class Graph : public GUIComponent {
public:
    Graph(const std::string& id);
    void addDataPoint(float x, float y);
    void removeDataPoint(float x, float y);
    void updateDataPoint(float oldX, float oldY, float newX, float newY);  // Method to update data point
    void draw() const override;

private:
    std::vector<std::pair<float, float>> dataPoints;
};

#endif //PROJECTST_GRAPH_H