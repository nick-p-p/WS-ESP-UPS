#include "StateMessage.h"

StateMessage::StateMessage(StateMessageType stateMessageType, int state, float value)
{
    this->stateMessageType = stateMessageType;
    this->state = state;
    this->value = value;
}

StateMessageType StateMessage::getStateMessageType() { return stateMessageType; }

int StateMessage::getState() { return state; }

float StateMessage::getValue() { return value; }