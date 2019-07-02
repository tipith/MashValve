#pragma once

class IInputSource
{
public:
    virtual void monitor(void) = 0;
    virtual unsigned int setpoint(unsigned int current_setpoint) = 0;

    unsigned long millis_since_latest(void)
    {
        return millis() - _last_received;
    }

    bool has_setpoint(void)
    {
        return _last_received != 0;
    }

protected:
    unsigned long _last_received;
};