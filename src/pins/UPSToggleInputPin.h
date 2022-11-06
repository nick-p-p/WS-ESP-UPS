#pragma once

#include "Arduino.h"
#include "UPSPin.h"

#define TOGGLE_AFTER_MILLIS 1200

/**
 * @brief a variation on theme which is logically toggled on and off each time the pin is on
 *  for TOGGLE_AFTER_MILLIS milliseconds. The inherited pinState is changed when toggling occurs
 *  isOnFor and isOffFor represent the time since last toggle.
 *
 */
class UPSToggleInputPin : public UPSPin
{

public:
    /**
     * @brief basic constructor
     *
     * @param pullup - set to true to enable internal pullups for the pin
     * @param onState HIGH or LOW, which ever logically represents boolean on
     * @param name for debugging only
     */
    UPSToggleInputPin(int pin, boolean pullup, int onState, String pinName);

    /**
     * @param gets called frequently to check/update the state of a pin.
     */
    virtual void loop();

private:
    /**
     * @brief used to prevent constant toggling if the button is held down.
     *  must be reset to true by the pin being off before another toggle.
     */ 
    bool releasedSincelastPress = false;

    /**
     * @brief tracks state of the underlying pin rather than the toggle
     */
    int actualPinState;

    /**
     * @brief tracks time of changes to the underlying pin rather than the toggle
     */
    long actualLastChangedMillis = 0;
};
