#pragma once

#include <stdio.h>
#include "Arduino.h"
#include "../interface/IEncoder.h"
#include "../interface/IMotorDriver.h"
#include "../interface/IInputSource.h"
#include "../interface/IControlStrategy.h"

class ValveMotor
{
public:
    ValveMotor(IEncoder &encoder, IMotorDriver &hbridge, IInputSource &input_prio, IInputSource &input, IControlStrategy &control) : _encoder(encoder), _hbridge(hbridge), _input_prio(input_prio), _input(input), _control(control)
    {
    }

    void calibrate(void)
    {
        /* Drive motor until limit switch triggers */

        /* Set known position */
        _encoder.reset();
    }

    void test_ramp()
    {
        static unsigned long next_run = 0;
        static bool cwDirection = true;
        static int pwm = 1;

        if (next_run < millis())
        {
            if (cwDirection && pwm++ > 100)
            {
                cwDirection = false;
            }
            else if (!cwDirection && pwm-- < -100)
            {
                cwDirection = true;
            }

            _hbridge.set_dc(pwm);
            next_run = millis() + 50;
        }
    }

    void do_work(void)
    {
        _input_prio.monitor();
        _input.monitor();
        test_ramp();
    }

    void print_state(void)
    {
        static char text[128] = {0};

        Serial.println("-----------------------------------");

        snprintf(text, sizeof(text) - 1, "input1:  age=%lu ms set=%u\n",
                 _input_prio.millis_since_latest(), 
                 _input_prio.has_setpoint() ? _input_prio.setpoint(0) : -1);
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "input2:  age=%lu ms set=%u\n",
                 _input.millis_since_latest(),
                 _input.has_setpoint() ? _input.setpoint(0) : -1);
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "tacho:   pos=%li\n", _encoder.position());
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "driver:  dutycycle=%i\n", _hbridge.get_dc());
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "control: error=%i\n", _get_error());
        Serial.print(text);
    }

private:

    int _get_error(void)
    {
        if (_input.has_setpoint())
        {
            return _control.get_error(_encoder.position(), _input.setpoint(0));
        }

        return 0;
    }

    IEncoder &_encoder;
    IMotorDriver &_hbridge;
    IInputSource &_input_prio;
    IInputSource &_input;
    IControlStrategy &_control;
};
