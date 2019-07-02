#pragma once

#include "Arduino.h"
#include "../interface/IInputSource.h"

#define BC_PWM_MIN_FREQ 50
#define BC_PWM_MAX_PERIOD (1000000 / BC_PWM_MIN_FREQ)

class InputSourcePWM : public IInputSource 
{
public:
    InputSourcePWM(uint8_t pin) : _pin(pin), _pulse_high(0), _pulse_low(0)
    {
        _last_received = 0;
        pinMode(_pin, INPUT);
    }

    void monitor(void)
    {
        unsigned long tmp1 = pulseIn(_pin, HIGH, 2 * BC_PWM_MAX_PERIOD);
        unsigned long tmp2 = pulseIn(_pin, LOW, 2 * BC_PWM_MAX_PERIOD);

        if (tmp1 != 0 && tmp2 != 0)
        {
            _pulse_high = tmp1;
            _pulse_low = tmp2;
            _last_received = millis();
        }
    }

    unsigned int setpoint(unsigned int current_setpoint)
    {
        return map(duty_cycle(), 0, 1000, 0, 255);
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
    uint8_t _pin;
    uint32_t _pulse_low;
    uint32_t _pulse_high;
};