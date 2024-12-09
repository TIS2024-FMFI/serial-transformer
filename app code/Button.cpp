#include "Button.h"
#include <iostream>

Button::Button(const std::string& id, const std::string& label) : GUIComponent(id), label(label) {}

void Button::setLabel(const std::string& Label) {
    this->label = Label;
}

std::string Button::getLabel() const {
    return label;
}

void Button::onClick() const {
    // TODO !!!
    // implementation of on click event
}

void Button::draw() const {
    // draw button
    std::cout << "Drawing button with label: " << label << std::endl;
}