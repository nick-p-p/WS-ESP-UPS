#pragma once

#include "Arduino.h"
#include "UPSPin.h"

/**
 * @brief handles polling a pin and knowing how long it has been in the current state
 *
 * with added functionality specifically for outputs
 */
class UPSOutputPin : public UPSPin
{

public:
  /**
   * @brief basic constructor
   *
   * @param onState HIGH or LOW, which ever logically represents boolean on
   * @param name for debugging only
   */
  UPSOutputPin(int pin, int onState, String pinName);

  /**
   * @brief initialize pin
   *
   * @param on what state to initially set the pin to
   */
  void begin(bool on);

  /**
   * @param gets called frequently to check/update the state of a pin.
   */
  virtual void loop();

  /**
   * @brief set the pin on or off according to whether on is HIGH or LOW
   */
  void set(bool on);

  /**
   * @brief set the pin on  according to whether on is HIGH or LOW
   */
  void on();

  /**
   * @brief set the pin off according to whether on is HIGH or LOW
   */
  void off();
};
