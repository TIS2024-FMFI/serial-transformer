#include "GUI.h"
#include <iostream>

GUI::GUI() {}

void GUI::addButton(const std::string& id, const std::string& label) {
    components[id] = new Button(id, label);
}

void GUI::addToggleSwitch(const std::string& id, bool initialState) {
    components[id] = new ToggleSwitch(id, initialState);
}

void GUI::addDropdownBox(const std::string& id, const std::vector<std::string>& options) {
    components[id] = new DropdownBox(id, options);
}

void GUI::addInputBox(const std::string& id, const std::string& initialValue) {
    components[id] = new InputBox(id, initialValue);
}

void GUI::addGraph(const std::string& id) {
    components[id] = new Graph(id);
}

Button* GUI::getButton(const std::string& id) const {
    return dynamic_cast<Button*>(components.at(id));
}

ToggleSwitch* GUI::getToggleSwitch(const std::string& id) const {
    return dynamic_cast<ToggleSwitch*>(components.at(id));
}

DropdownBox* GUI::getDropdownBox(const std::string& id) const {
    return dynamic_cast<DropdownBox*>(components.at(id));
}

InputBox* GUI::getInputBox(const std::string& id) const {
    return dynamic_cast<InputBox*>(components.at(id));
}

Graph *GUI::getGraph(const std::string &id) const {
    return dynamic_cast<Graph*>(components.at(id));
}

void GUI::draw() const {
    // TODO !!!
}

void GUI::handleInput(const std::string& id, char input) {
    // TODO !!!
}