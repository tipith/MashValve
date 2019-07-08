
#pragma once

#include "../interface/IInputSource.h"

#define GRACE_PERIOD_AFTER_DISABLED 2000
#define STEPSIZE_DIVISOR 20

class InputSourceManualPoller : public IInputSource
{
public:
    InputSourceManualPoller(uint8_t pin_close, uint8_t pin_open) : _pin_open(pin_close), _pin_close(pin_open)
    {
        pinMode(_pin_close, INPUT_PULLUP);
        pinMode(_pin_open, INPUT_PULLUP);
    }

    void monitor(InputRange &range)
    {
        int bclose = digitalRead(_pin_close);
        int bopen = digitalRead(_pin_open);
        if ((bopen == LOW || bclose == LOW) && _next_input_allowed < millis())
        {
            // if both buttons pressed, disable manual mode, and enter a grace period
            if (bopen == LOW && bclose == LOW)
            {
                _last_received = 0;
                _next_input_allowed = millis() + GRACE_PERIOD_AFTER_DISABLED;
            }
            else
            {
                _last_received = millis();
                _next_input_allowed = 0;
            }

            long stepsize = (range.max - range.min) / STEPSIZE_DIVISOR;
            long direction = (bopen == LOW) - (bclose == LOW);
            _setpoint = range.pos + stepsize * direction;
            _setpoint = constrain(_setpoint, range.min, range.max);
        }
    }

    long setpoint(void)
    {
        return _setpoint;
    }

    bool has_setpoint(void) override
    {
        return (_last_received != 0); // && (millis_since_latest() < _active_for_ms);
    }

    const char *type(void)
    {
        return "manual poller";
    }

private:
    unsigned long _next_input_allowed = 0;
    int _setpoint = 0;
    uint8_t _pin_close;
    uint8_t _pin_open;
};