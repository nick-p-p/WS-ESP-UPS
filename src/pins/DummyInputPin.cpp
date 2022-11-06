#include "DummyInputPin.h"

DummyInputPin::DummyInputPin(bool alwaysReturn, String name)
    : UPSPin(-1, HIGH, name)
{
    this->alwaysReturn = alwaysReturn;
    loop();
}

void DummyInputPin::loop()
{
    this->pinState = alwaysReturn ? HIGH : LOW;
}