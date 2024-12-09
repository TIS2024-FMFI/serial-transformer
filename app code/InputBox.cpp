#include "InputBox.h"
#include <iostream>

InputBox::InputBox(const std::string& id, const std::string& initialValue) :
                        GUIComponent(id), value(initialValue) {}

void InputBox::setValue(const std::string& value) {
    this->value = value;
}

std::string InputBox::getValue() const {
    return value;
}

void InputBox::draw() const {
    // TODO !!!
    // draw input box
    std::cout << "Drawing input box with value: " << value << std::endl;
}

void InputBox::handleInput(char input) {
    // TODO !!!
}