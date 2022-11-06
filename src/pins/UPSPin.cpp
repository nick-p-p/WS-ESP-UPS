#include "UPSPin.h"

UPSPin::UPSPin( int pin, int onState, String pinName  )
{
  this->pin = pin;
  lastChangedMillis = millis();
  this->onState = onState;
  offState = (onState == HIGH ? LOW : HIGH);
  name = pinName;
}

boolean UPSPin::isOn()
{
  return (pinState == onState);
}

boolean UPSPin::isOff()
{
  return (pinState != onState);
}

boolean UPSPin::isOnFor(int seconds)
{
  return isOn() && ((millis() - lastChangedMillis) >= (unsigned long)(seconds * 1000));
}

boolean UPSPin::isOffFor(int seconds)
{
  return isOff() && ((millis() - lastChangedMillis) >= (unsigned long)(seconds * 1000));
}

void UPSPin::resetTimer()
{
   lastChangedMillis = millis();
}
