#pragma once

#include "StateMessage.h"


/**
 * Abstract display class to handle propogating various state updates to any 
 * hardware specific implementations
 */ 
class Display
{

public:
    /**
     * @brief ctor
     */ 
    Display(/* args */);

    virtual void accept(StateMessage & msg) = 0;
    
};
