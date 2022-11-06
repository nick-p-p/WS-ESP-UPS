#include "UPSInputPin.h"
UPSInputPin::UPSInputPin( int pin, boolean pullup, int onState, String pinName  ) : UPSPin ( pin,  onState,  pinName  )
{
   if (pullup)
  {
    pinMode(pin, INPUT_PULLUP);
  }
  else
  {
    pinMode(pin, INPUT);
  }
}

// check for changes
void UPSInputPin::loop()
{
  int newState = digitalRead(pin);
  if (newState != pinState)
  {
    Serial.print(name);
    Serial.print(" is ");
    Serial.println((newState == onState) ? "On" : "Off");
    pinState = newState;
    lastChangedMillis = millis();
  }
}

