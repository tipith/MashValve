#pragma once

#include "Arduino.h"
#include "../interface/IInputSource.h"

#define BC_PWM_MIN_FREQ 50
#define BC_PWM_MAX_PERIOD (1000000 / BC_PWM_MIN_FREQ)

class InputSourcePWM : public IInputSource 
{
public:
    InputSourcePWM(uint8_t pin)
    {
        pinMode(_pin, INPUT);
    }

    void monitor(InputRange& range)
    {
        unsigned long tmp1 = pulseIn(_pin, HIGH, 2 * BC_PWM_MAX_PERIOD);
        unsigned long tmp2 = pulseIn(_pin, LOW, 2 * BC_PWM_MAX_PERIOD);

        if (tmp1 != 0 && tmp2 != 0)
        {
            _pulse_high = tmp1;
            _pulse_low = tmp2;
            _last_received = millis();
            _setpoint = map(duty_cycle(), 0, 1000, range.min, range.max);
        }
    }

    long setpoint(void)
    {
        return _setpoint;
    }

    const char* type(void)
    {
        return "pwm";
    }

    uint32_t inline period(void)
    {
        // try to prevent division by zero
        return (_pulse_high + _pulse_low) ? (_pulse_high + _pulse_low) : 1;
    }

    uint16_t frequency(void)
    {
        return 1000000 / period();
    }

    uint32_t duty_cycle(void)
    {
        return 1000 * _pulse_high / period();
    }

private:
    long _setpoint = 0;
    uint8_t _pin = 0;
    uint32_t _pulse_low = 0;
    uint32_t _pulse_high = 0;
};