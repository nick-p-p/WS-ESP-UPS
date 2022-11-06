#pragma once

#include "Arduino.h"
#include "UPSPin.h"

/**
 * @brief handles polling a pin and knowing how long it has been in the current state
 * 
 * with added functionality specifically for inputs
 */ 
class UPSInputPin : public UPSPin 
{

public:

    /**
     * @brief basic constructor
     * 
     * @param pullup - set to true to enable internal pullups for the pin
     * @param onState HIGH or LOW, which ever logically represents boolean on
     * @param name for debugging only
     */
  UPSInputPin( int pin, boolean pullup, int onState, String pinName );

    /**
     * @param gets called frequently to check/update the state of a pin.
     */ 
  virtual void loop();

  
};



