#pragma once
#include "UPSPin.h"
/**
 * @brief Dummy pin for where an input is optional
 * can be set to return always on or always off
 */ 
class DummyInputPin : public UPSPin
{

public:
    /**
     * @brief constructor
     * 
     * @param alwaysReturn - the value this pin should always return
     */ 
    DummyInputPin(bool alwaysReturn, String name);

    /**
     * @brief gets called frequently to check/update the state of a pin.
     */ 
    virtual void loop();

private:
    bool alwaysReturn = true;
};
