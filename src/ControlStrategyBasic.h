
#pragma once

#include "../interface/IControlStrategy.h"

class ControlStrategyBasic : public IControlStrategy
{
public:
    ControlStrategyBasic()
    {

    }

    unsigned long control(bool cw)
    {

    }

    int get_error(int current, int setpoint)
    {
        return setpoint - current;
    }


private:

};