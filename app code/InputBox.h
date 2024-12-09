#ifndef PROJECTST_INPUTBOX_H
#define PROJECTST_INPUTBOX_H

#include "GUIComponent.h"
#include <string>

class InputBox : public GUIComponent {
public:
    InputBox(const std::string& id, const std::string& initialValue = "");
    void setValue(const std::string& value);
    std::string getValue() const;
    void handleInput(char input);
    void draw() const override;

private:
    std::string value;
};

#endif //PROJECTST_INPUTBOX_H