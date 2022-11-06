#pragma once

#include <Arduino.h>
#include "interfaces/Display.h"
#include "impl/AllDisplays.h"
#include "pins/UPSOutputPin.h"

/**
 * @brief display for a beeper controlled by a gpio
 * 
 * it listens for a specified state type and beeps once for value != 0 and twice otherwise
 * 
 */
class StateChangeBeeper : public Display
{

public:
    /**
     * @brief ctor
     *
     * @param pinNo - numner of the GPIO to use for the beeper
     * @param stateMessageType - whoch message type to respond to
     */
    StateChangeBeeper(int pinNo, StateMessageType stateMessageType);

    /**
     * @brief initialize
     */ 
    virtual void begin();

    /**
     * @brief process messages looking for user switch changes
     */ 
    virtual void accept(StateMessage &msg);

private:
    UPSOutputPin beeperPin;
    StateMessageType stateMessageType;
};
