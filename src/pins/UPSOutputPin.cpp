#include "UPSOutputPin.h"
#include "Arduino.h"


UPSOutputPin::UPSOutputPin(int pin, int onState, String pinName) 
  : UPSPin(pin, onState, pinName)
{
}


void UPSOutputPin::begin(bool on)
{
  pinMode(pin, OUTPUT);
  set(on);
}

// check for changes
void UPSOutputPin::set(bool on)
{
  bool currentState = pinState == onState;
  if (currentState == on)
  {
    return;
  }

  Serial.print(name);
  Serial.print(" set to: ");
  Serial.println((on) ? "On" : "Off");

  int newState = (on ? onState : offState);
  digitalWrite(pin, newState);

  pinState = newState;
  lastChangedMillis = millis();
}

void UPSOutputPin::on()
{
  set(true);
}

void UPSOutputPin::off()
{
  set(false);
}

void UPSOutputPin::loop()
{
}
