#ifndef PROJECTST_GUI_H
#define PROJECTST_GUI_H

#include "Button.h"
#include "ToggleSwitch.h"
#include "DropdownBox.h"
#include "InputBox.h"
#include "Graph.h"
#include <unordered_map>
#include <string>

class GUI {
public:
    GUI();
    void addButton(const std::string& id, const std::string& label);
    void addToggleSwitch(const std::string& id, bool initialState);
    void addDropdownBox(const std::string& id, const std::vector<std::string>& options);
    void addInputBox(const std::string& id, const std::string& initialValue = "");
    void addGraph(const std::string& id);

    Button* getButton(const std::string& id) const;
    ToggleSwitch* getToggleSwitch(const std::string& id) const;
    DropdownBox* getDropdownBox(const std::string& id) const;
    InputBox* getInputBox(const std::string& id) const;
    Graph* getGraph(const std::string& id) const;

    void draw() const;  // draw all components
    void handleInput(const std::string& id, char input);  // handle user input for a specific component

private:
    std::unordered_map<std::string, GUIComponent*> components;
};

#endif //PROJECTST_GUI_H