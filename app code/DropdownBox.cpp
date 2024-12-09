#include "DropdownBox.h"
#include <iostream>
#include <algorithm>

DropdownBox::DropdownBox(const std::string& id, const std::vector<std::string>& options)
        : GUIComponent(id), options(options), selectedOption(options.empty() ? "" : options[0]) {}

void DropdownBox::addOption(const std::string& option) {
    // TODO !!!
}

void DropdownBox::removeOption(const std::string& option) {
    // TODO !!!
}

std::string DropdownBox::getSelectedOption() const {
    return selectedOption;
}

void DropdownBox::setSelectedOption(const std::string& option) {
    if (std::find(options.begin(), options.end(), option) != options.end()) {
        selectedOption = option;
    }
}

void DropdownBox::draw() const {
    // TODO !!!
    // draw dropdown box
    std::cout << "Drawing dropdown box with selected option: " << selectedOption << std::endl;
}