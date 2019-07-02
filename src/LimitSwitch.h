#pragma once

#include "../interface/ILimitSwitch.h"

class LimitSwitch : public ILimitSwitch
{
public:
    LimitSwitch(uint8_t pin, long initial_timeout = -1) : _pin(pin), _initial_timeout(initial_timeout)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    boolean set(void)
    {   
        return millis() > _initial_timeout;
    }

private:
    uint8_t _pin;
    long _initial_timeout;
};