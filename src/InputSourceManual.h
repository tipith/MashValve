
#pragma once

#include "../interface/IInputSource.h"

/* singleton due to attaching interrupts */
static int diff = 0;

static void on_close_button_pressed(void)
{
    diff--;
}

static void on_open_button_pressed(void)
{
    diff++;
}

class InputSourceManual : public IInputSource
{
public:
    InputSourceManual(uint8_t pin_close, uint8_t pin_open) : _pin_open(pin_close), _pin_close(pin_open)
    {
        diff = 0;
        pinMode(pin_close, INPUT_PULLUP);
        pinMode(pin_open, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(pin_close), on_close_button_pressed, FALLING);
        attachInterrupt(digitalPinToInterrupt(pin_open), on_open_button_pressed, FALLING);
    }

    void monitor(InputRange& range)
    {
        if (_old_diff != diff)
        {
            _old_diff = diff;
            _last_received = millis();
            _setpoint = constrain(_old_diff, -range.to_min, range.to_max);
        }
    }

    long setpoint(void)
    {
        return _setpoint;
    }

    const char* type(void)
    {
        return "manual";
    }

private:
    long _setpoint = 0;
    int _old_diff; = 0
    uint8_t _pin_close;
    uint8_t _pin_open;
};