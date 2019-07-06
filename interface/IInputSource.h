#pragma once

class IInputSource
{
public:
    typedef struct {
        long min;
        long max;
        long to_max;
        long to_min;
        long pos;
    } InputRange;

    virtual void monitor(InputRange& range) = 0;
    virtual long setpoint(void) = 0;
    virtual const char* type(void) = 0;

    unsigned long millis_since_latest(void)
    {
        return millis() - _last_received;
    }

    virtual bool has_setpoint(void)
    {
        return _last_received != 0;
    }

protected:
    unsigned long _last_received;
};