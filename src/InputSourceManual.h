
#pragma once

#include "../interface/IInputSource.h"

class InputSourceManual : public IInputSource
{
public:
    InputSourceManual(uint8_t pin_close, uint8_t pin_open) : _pin_open(pin_close), _pin_close(pin_open)
    {
        _last_received = 0;
        pinMode(pin_close, INPUT);
        pinMode(pin_open, INPUT);
    }

    void monitor(void)
    {

    }

    unsigned int setpoint(unsigned int current_setpoint)
    {
        return 666;
    }

private:
    uint8_t _pin_close;
    uint8_t _pin_open;
};