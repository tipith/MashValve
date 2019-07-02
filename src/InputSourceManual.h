
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
    InputSourceManual(uint8_t pin_close, uint8_t pin_open, unsigned long positions) : _positions(positions), _old_diff(0), _pin_open(pin_close), _pin_close(pin_open)
    {
        diff = 0;
        _last_received = 0;
        pinMode(pin_close, INPUT_PULLUP);
        pinMode(pin_open, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(pin_close), on_close_button_pressed, FALLING);
        attachInterrupt(digitalPinToInterrupt(pin_open), on_open_button_pressed, FALLING);
    }

    void monitor(void)
    {
        if (_old_diff != diff)
        {
            _old_diff = diff;
            _last_received = millis();
        }
    }

    long setpoint(long current_setpoint)
    {
        return diff + current_setpoint;
    }

private:
    unsigned long _positions;
    int _old_diff;
    uint8_t _pin_close;
    uint8_t _pin_open;
};