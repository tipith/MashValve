
#pragma once

#include "../interface/IInputSource.h"

class InputSourceManualPoller : public IInputSource
{
public:
    InputSourceManualPoller(uint8_t pin_close, uint8_t pin_open, unsigned long active_for_ms) : _setpoint(0), _pin_open(pin_close), _pin_close(pin_open)
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
            if (has_setpoint() == false)
            {
                _latched_range = range;
                _setpoint = _latched_range.pos;
            }
            _last_received = millis();
            _setpoint += 100 * ((button_open == LOW) - (button_close == LOW));
            _setpoint = constrain(_setpoint, -_latched_range.min, _latched_range.max);
        }
    }

    long setpoint(void)
    {
        return _setpoint;    
    }

    bool has_setpoint(void) override
    {
        return (_last_received != 0) && (millis_since_latest() < _active_for_ms);
    }

    const char* type(void)
    {
        return "manual poller";
    }

private:
    unsigned long _active_for_ms;
    int _setpoint;
    uint8_t _pin_close;
    uint8_t _pin_open;
    InputRange _latched_range;
};