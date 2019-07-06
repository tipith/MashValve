
#pragma once

#include "../interface/IControlStrategy.h"

class ControlStrategyBasic : public IControlStrategy
{
public:
    ControlStrategyBasic(long positions)
    {
        _poscount = positions;
    }

    long control(long process_pos, long setpoint)
    {
        long err = setpoint - process_pos;
        err = constrain(err, -_poscount, _poscount);
        return map(err, -_poscount, _poscount, -250, 250);
    }
};