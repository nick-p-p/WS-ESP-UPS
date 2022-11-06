#pragma once
#include "interfaces/Display.h"
#include <vector>

class AllDisplays
{

public:
    AllDisplays(/* args */);

    /**
     * @brief register a specific implementation to receive update messages. Each implementation
     *      should call this once
     * 
     * @param display - pointer to the implementing object
     */ 
    static void registerDisplay(Display * display);

    /**
     * @brief notify each display of the state message
     * 
     * @param msg - the message to propogate
     */
    static void notify(StateMessage & msg);

private:

    static std::vector<Display *> allDisplays;
};
