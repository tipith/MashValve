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
    ValveMotor(IEncoder &encoder, IMotorDriver &hbridge, IInputSource* inputs[], unsigned int input_count, IControlStrategy &control, ILimitSwitch &limit) : _encoder(encoder), _hbridge(hbridge), _control(control), _limit(limit)
    {
        for (int i = 0; i < (sizeof(_inputs) / sizeof(_inputs[0])); ++i)
        {
            _inputs[i] = (i < input_count) ? inputs[i] : nullptr;
        }

        pinMode(LED_BUILTIN, OUTPUT);
    }

    void calibrate(void)
    {
        Serial.println("calibrating");

        /* Drive motor until limit switch triggers */
        _hbridge.set_dc(-255);
        while (!_limit.set());
        _hbridge.set_dc(0);

        /* Set known position */
        _encoder.reset();
    }

    void do_work(void)
    {
        IControlStrategy::ControlRange crange = _control.range();
        long pos = _encoder.position();
        IInputSource::InputRange irange = {crange.min, crange.max, pos, crange.max - pos, pos};

        for (auto in : _inputs)
        {
            if (in) 
            {
                in->monitor(irange);
            }
        }

        _hbridge.set_dc(_calculate_control());

        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }

    void print_state(void)
    {
        static char text[128] = {0};

        Serial.println("-----------------------------------");

        for (auto in : _inputs)
        {
            if (in)
            {
                snprintf(text, sizeof(text) - 1, "input %s:  age=%lu ms set=%i\n",
                        in->type(), in->millis_since_latest(),
                        in->has_setpoint() ? in->setpoint() : -1);
                Serial.print(text);
            }
        }

        snprintf(text, sizeof(text) - 1, "tacho:   pos=%li\n", _encoder.position());
        Serial.print(text);

        snprintf(text, sizeof(text) - 1, "driver:  dutycycle=%i\n", _hbridge.get_dc());
        Serial.print(text);

        IInputSource* in = _get_input_source();
        snprintf(text, sizeof(text) - 1, "control: control=%li src=%s\n",
                 _calculate_control(), in ? in->type() : "none");
        Serial.print(text);
    }

private:
    IInputSource *_get_input_source(void)
    {
        for (auto in : _inputs)
        {
            if (in && in->has_setpoint())
            {
                return in;
            }
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
    IInputSource* _inputs[2];
    IControlStrategy &_control;
    ILimitSwitch &_limit;
    bool ledState = true;
};
