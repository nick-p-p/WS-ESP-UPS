#pragma once

#include "Arduino.h"
/**
 * @brief handles polling a pin and knowing how long it has been in the current state
 */ 
class UPSPin
{

public:
    /**
     * @brief basic constructor
     * 
     * @param onState HIGH or LOW, which ever logically represents boolean on
     * @param name for debugging only
     */
    UPSPin(int pin, int onState, String pinName);

    /**
     * @brief gets called frequently to check/update the state of a pin.
     */ 
    virtual void loop() = 0;

    /**
     * @brief true if the pin is logically on
     */ 
    boolean isOn();

    /**
     * @brief true if the pin is logically off
     */ 
    boolean isOff();

    /**
     * @brief true if the pin is logically on and has been in that state for the suppleid seconds
     */ 
    boolean isOnFor(int seconds);

    /**
     * @brief true if the pin is logically on and has been in that state for the suppleid seconds
     */ 
    boolean isOffFor(int seconds);

    /**
     * @brief resets the counter of isOnFor and isOffFor
     */
    void resetTimer();

protected:
    // which pin we are checking
    int pin;

    // state HIGH / LOW that reflects on
    int onState;

    // state HIGH / LOW that reflects on
    int offState;

    // state HIGH / LOW that  the pin is in now
    int pinState;

    // millis for last time pin state changed
    long lastChangedMillis = 0;

    // name just for diagnostic purposes
    String name;
};