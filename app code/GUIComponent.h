#ifndef PROJECTST_GUICOMPONENT_H
#define PROJECTST_GUICOMPONENT_H

#include <string>

class GUIComponent {
public:
    GUIComponent(const std::string& id) : id(id) {}
    virtual ~GUIComponent() {}

    std::string getId() const { return id; }
    virtual void draw() const = 0;  // virtual function for drawing the component

private:
    std::string id;
};

#endif //PROJECTST_GUICOMPONENT_H