
#pragma once

#include "../interface/IControlStrategy.h"

class ControlStrategyBasic : public IControlStrategy
{
public:
    ControlStrategyBasic(long positions) : poscount(positions)
    {
    }

    long control(long process_pos, long setpoint)
    {
        long err = setpoint - process_pos;
        err = constrain(err, -poscount, poscount);
        return map(err, -poscount, poscount, -250, 250);
    }

private:
    long poscount;
};