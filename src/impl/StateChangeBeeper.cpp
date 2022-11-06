#include "StateChangeBeeper.h"

StateChangeBeeper::StateChangeBeeper(int pinNo, StateMessageType stateMessageType)
    : beeperPin(pinNo, HIGH, "beeper")
{
    this->stateMessageType = stateMessageType;
}

void StateChangeBeeper::begin()
{
    beeperPin.begin(false);
    AllDisplays::registerDisplay(this);
}

void StateChangeBeeper::accept(StateMessage &msg)
{
    if (msg.getStateMessageType() == stateMessageType)
    {
        if (msg.getState() != 0)
        {
            // on - single long beep
            beeperPin.on();
            delay(750);
            beeperPin.off();
        }
        else
        {
            // off - double short beep
            beeperPin.on();
            delay(250);
            beeperPin.off();
            delay(250);
            beeperPin.on();
            delay(250);
        }
    }
}