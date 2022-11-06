#include "UserSwitchDisplay.h"

UserSwitchDisplay::UserSwitchDisplay(int pinNo)
    : displayPin(pinNo, HIGH, "userSwitch")
{
}

void UserSwitchDisplay::begin()
{
    displayPin.begin(false);
    AllDisplays::registerDisplay(this);
}

void UserSwitchDisplay::accept(StateMessage & msg) 
{
    if (msg.getStateMessageType() == UserSwitch)
    {
        displayPin.set( msg.getState() != 0);
    }
}