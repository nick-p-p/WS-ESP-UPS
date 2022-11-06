#pragma once

#include <Arduino.h>
#include "interfaces/Display.h"
#include "impl/AllDisplays.h"
#include "pins/UPSOutputPin.h"

/**
 * @brief display for soft user switch to an LED
 */
class UserSwitchDisplay : public Display
{

public:
    /**
     * @brief ctor
     *
     * @param pinNo - numner of teh GPIO to use for the led
     */
    UserSwitchDisplay(int pinNo);

    /**
     * @brief initialize
     */ 
    virtual void begin();

    /**
     * @brief process messages looking for user switch changes
     */ 
    virtual void accept(StateMessage &msg);

private:
    UPSOutputPin displayPin;
};
