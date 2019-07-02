#pragma once

class IControlStrategy
{
public:
    /*
     Returns a value between -250 .. 250
     */
    virtual long control(long process_pos, long setpoint) = 0;
};