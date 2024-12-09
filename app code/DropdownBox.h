#ifndef PROJECTST_DROPDOWNBOX_H
#define PROJECTST_DROPDOWNBOX_H

#include "GUIComponent.h"
#include <string>
#include <vector>

class DropdownBox : public GUIComponent {
public:
    DropdownBox(const std::string& id, const std::vector<std::string>& options);
    void addOption(const std::string& option);
    void removeOption(const std::string& option);
    std::string getSelectedOption() const;
    void setSelectedOption(const std::string& option);
    void draw() const override;

private:
    std::vector<std::string> options;
    std::string selectedOption;
};

#endif //PROJECTST_DROPDOWNBOX_H