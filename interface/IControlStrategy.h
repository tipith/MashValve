#pragma once

class IControlStrategy
{
public:
    typedef struct {
        long min;
        long max;
    } ControlRange ;

    /*
     Returns a value between -250 .. 250
     */
    virtual long control(long process_pos, long setpoint) = 0;

    virtual ControlRange range(void)
    {
        ControlRange range = {0, _poscount};
        return range;
    }

protected:
    long _poscount;
};