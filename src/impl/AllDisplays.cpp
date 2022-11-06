#include "AllDisplays.h"

std::vector<Display*> AllDisplays::allDisplays;

AllDisplays::AllDisplays(){}

void AllDisplays::registerDisplay(Display * display)
{
    allDisplays.push_back(display);
}

void AllDisplays::notify(StateMessage & msg)
{
    for (Display * displayptr : allDisplays)
    {
        displayptr->accept(msg);
    }
}