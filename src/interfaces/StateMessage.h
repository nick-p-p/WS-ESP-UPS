enum StateMessageType
{
    /**
     * @brief External Power is on or off
     *      on is indicated by state != 0
     */
    ExternalPower,

    /**
     * @brief Change in state of Pi
     *      new state is in state (int of PiState)
     */
    Pi,

    /**
     * @brief Change in battery charge
     *      state is percentage
     */
    BatteryPct,

    /**
     * @brief Change in battery voltage
     *      value is percentage
     */
    BatteryVolts,

    /**
     * @brief Change in time using battery as power source
     *      state is seconds
     */
    BatteryTime,


    /**
     * @brief Change in time limit for battery as power source
     *      state is seconds
     */    
    BatteryTimeLimit,

    /**
     * @brief user switch physical or soft has changed
     *      on is indicated by state != 0
     */
    UserSwitch
};

/**
 * @brief Generic messsage format for updating any type of display or indicator of changes
 */
class StateMessage
{
public:
    StateMessage(StateMessageType stateMessageType, int state, float value);
    StateMessageType getStateMessageType();
    int getState();
    float getValue();

private:
    StateMessageType stateMessageType;
    int state;
    float value;
};