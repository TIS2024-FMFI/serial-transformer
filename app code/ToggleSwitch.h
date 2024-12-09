#ifndef PROJECTST_TOGGLESWITCH_H
#define PROJECTST_TOGGLESWITCH_H

#include "GUIComponent.h"

class ToggleSwitch : public GUIComponent {
public:
    ToggleSwitch(const std::string& id, bool initialState);
    void setState(bool state);
    bool getState() const;
    void onToggle() const;
    void draw() const override;

private:
    bool state;
};

#endif //PROJECTST_TOGGLESWITCH_H