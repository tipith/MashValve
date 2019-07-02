
#pragma once

#include "../interface/IInputSource.h"

class InputSourceManualPoller : public IInputSource
{
public:
    InputSourceManualPoller(uint8_t pin_close, uint8_t pin_open, unsigned long active_for_ms) : _diff(0), _pin_open(pin_close), _pin_close(pin_open)
    {
        _active_for_ms = active_for_ms;
        _last_received = 0;
        pinMode(_pin_close, INPUT_PULLUP);
        pinMode(_pin_open, INPUT_PULLUP);
    }

    void monitor(InputRange& range)
    {
        int button_close = digitalRead(_pin_close);
        int button_open = digitalRead(_pin_open);
        if (button_close == LOW || button_open == LOW)
        {
            if (!has_setpoint())
                _latched_range = range;
            _last_received = millis();
            _diff += 10 * ((button_open == LOW) - (button_close == LOW));
            _diff = constrain(_diff, -_latched_range.to_min, _latched_range.to_max);
        }
    }

    long setpoint(void)
    {
        return _diff;    
    }

    bool has_setpoint(void) override
    {
        bool active = (_last_received != 0) && (millis_since_latest() < _active_for_ms);
        if (!active)
            _diff = 0;
        return active;
    }

    const char* type(void)
    {
        return "manual poller";
    }

private:
    unsigned long _active_for_ms;
    int _diff;
    uint8_t _pin_close;
    uint8_t _pin_open;
    InputRange _latched_range;
};