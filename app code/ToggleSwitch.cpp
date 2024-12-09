#include "ToggleSwitch.h"
#include <iostream>

ToggleSwitch::ToggleSwitch(const std::string& id, bool initialState) : GUIComponent(id), state(initialState) {}

void ToggleSwitch::setState(bool state) {
    this->state = state;
}

bool ToggleSwitch::getState() const {
    return state;
}

void ToggleSwitch::onToggle() const {
    // TODO !!!
    // implementation of on toggle event
}

void ToggleSwitch::draw() const {
    // TODO !!!
    // draw toggle switch
    std::cout << "Drawing toggle switch with state: " << (state ? "ON" : "OFF") << std::endl;
}