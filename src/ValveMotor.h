#pragma once

#include <stdio.h>
#include "Arduino.h"
#include "../interface/IEncoder.h"
#include "../interface/IMotorDriver.h"
#include "../interface/IInputSource.h"
#include "../interface/IControlStrategy.h"
#include "../interface/ILimitSwitch.h"

class ValveMotor
{
public:
    ValveMotor(IEncoder &encoder, IMotorDriver &hbridge, IInputSource &input_prio, IInputSource &input, IControlStrategy &control, ILimitSwitch &limit) : _encoder(encoder), _hbridge(hbridge), _input_prio(input_prio), _input(input), _control(control), _limit(limit)
    {
        pinMode(LED_BUILTIN, OUTPUT);
    }

    void calibrate(void)
    {
        /* Drive motor until limit switch triggers */
        _hbridge.set_dc(-100);
        while (!_limit.set());
        _hbridge.set_dc(0);

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
        IControlStrategy::ControlRange crange = _control.range();
        long pos = _encoder.position();
        IInputSource::InputRange irange = {crange.min, crange.max, pos, crange.max - pos};
        _input_prio.monitor(irange);
        _input.monitor(irange);
        _hbridge.set_dc(_calculate_control());

        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }

    void print_state(void)
    {
        static char text[128] = {0};

        Serial.println("-----------------------------------");
        snprintf(text, sizeof(text) - 1, "input1:  age=%lu ms set=%i\n",
                 _input_prio.millis_since_latest(),
                 _input_prio.has_setpoint() ? _input_prio.setpoint() : -1);
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "input2:  age=%lu ms set=%i\n",
                 _input.millis_since_latest(),
                 _input.has_setpoint() ? _input.setpoint() : -1);
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "tacho:   pos=%li\n", _encoder.position());
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "driver:  dutycycle=%i\n", _hbridge.get_dc());
        Serial.print(text);

        IInputSource *in = _get_input_source();
        snprintf(text, sizeof(text) - 1, "control: control=%li src=%s\n",
                 _calculate_control(), in ? in->type() : "none");
        Serial.print(text);
    }

private:
    IInputSource *_get_input_source(void)
    {

        if (_input_prio.has_setpoint())
        {
            return &_input_prio;
        }
        else if (_input.has_setpoint())
        {
            return &_input;
        }
        return nullptr;
    }

    long _calculate_control(void)
    {
        IInputSource *in = _get_input_source();

        /* stay put if no input has been received */
        long setpoint = in ? in->setpoint() : _encoder.position();

        return _control.control(_encoder.position(), setpoint);
    }

    IEncoder &_encoder;
    IMotorDriver &_hbridge;
    IInputSource &_input_prio;
    IInputSource &_input;
    IControlStrategy &_control;
    ILimitSwitch &_limit;
    bool ledState = true;
};
