#pragma once

#include "../interface/IEncoder.h"
#include "../interface/IMotorDriver.h"
#include "../interface/IInputSource.h"
#include "../interface/IControlStrategy.h"
#include "../interface/ILimitSwitch.h"
#include "ValveMotor.h"


class ValveMotorBuilder
{
public:
    ValveMotorBuilder(void)
    {
    }

    ValveMotorBuilder &withDriver(IMotorDriver &driver)
    {
        _driver = &driver;
        return *this;
    }

    ValveMotorBuilder &withEncoder(IEncoder &encoder)
    {
        _encoder = &encoder;
        return *this;
    }

    ValveMotorBuilder &addInput(IInputSource &input)
    {
        unsigned int count = _count_of_inputs();
        if (count < (sizeof(_inputs) / sizeof(_inputs[0])))
            _inputs[count] = &input;
        return *this;
    }

    ValveMotorBuilder &withController(IControlStrategy &controller)
    {
        _controller = &controller;
        return *this;
    }

    ValveMotorBuilder &withLimitSwitch(ILimitSwitch &limit)
    {
        _limit = &limit;
        return *this;
    }

    ValveMotor &build(void)
    {
        ValveMotor *motor = new ValveMotor(*_encoder,
                                           *_driver, 
                                           _inputs, 
                                           _count_of_inputs(),
                                           *_controller,
                                           *_limit);
        return *motor;
    }

private:
    unsigned int _count_of_inputs(void)
    {
        unsigned int count = 0;
        for (auto in : _inputs)
        {
            count += (in != nullptr) ? 1 : 0;
        }
        return count;
    }

    IMotorDriver *_driver;
    IEncoder *_encoder;
    IInputSource *_inputs[3] = {nullptr};
    IControlStrategy *_controller;
    ILimitSwitch *_limit;
};