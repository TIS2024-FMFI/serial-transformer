#ifndef PROJECTST_BUTTON_H
#define PROJECTST_BUTTON_H

#include "GUIComponent.h"
#include <string>

class Button : public GUIComponent {
public:
    Button(const std::string& id, const std::string& label);
    void setLabel(const std::string& Label);
    std::string getLabel() const;
    void onClick() const;
    void draw() const override;

private:
    std::string label;
};

#endif //PROJECTST_BUTTON_H