#include "UPSToggleInputPin.h"
UPSToggleInputPin::UPSToggleInputPin(int pin, boolean pullup, int onState, String pinName)
    : UPSPin(pin, onState, pinName)
{
    if (pullup)
    {
        pinMode(pin, INPUT_PULLUP);
    }
    else
    {
        pinMode(pin, INPUT);
    }
    actualPinState = digitalRead(pin);
}

// check for changes
void UPSToggleInputPin::loop()
{
    int newState = digitalRead(pin);
    if (newState != actualPinState)
    {
        actualPinState = newState;
        actualLastChangedMillis = millis();
    }

    if (actualPinState = onState) // pin is on
    {
        if (releasedSincelastPress && ((millis() - actualLastChangedMillis) >= (TOGGLE_AFTER_MILLIS)))
        {
            // first time we crossed TOGGLE_AFTER_MILLIS
            pinState = (pinState == onState) ? offState : onState;
            Serial.print(name);
            Serial.print(" is ");
            Serial.println((pinState == onState) ? "On" : "Off");
            lastChangedMillis = millis();
            releasedSincelastPress = false;
        }
    }
    else
    {
        // off
        releasedSincelastPress = true;
    }
}
