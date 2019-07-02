#pragma once

#include "../interface/IEncoder.h"
#include "../interface/IMotorDriver.h"
#include "../interface/IInputSource.h"
#include "../interface/IControlStrategy.h"
#include "../interface/ILimitSwitch.h"
#include "ValveMotor.h"

enum InputPriority
{
    PRIO_HIGH,
    PRIO_LOW,
    PRIO_NUM
};

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

    ValveMotorBuilder &usingInput(IInputSource &input, enum InputPriority prio)
    {
        switch (prio)
        {
        case PRIO_HIGH:
            _input_prio_high = &input;
            break;
        case PRIO_LOW:
            _input_prio_low = &input;
            break;
        default:
            break;
        }
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
                                           *_input_prio_high, 
                                           *_input_prio_low,
                                           *_controller,
                                           *_limit);
        return *motor;
    }

private:
    IMotorDriver *_driver;
    IEncoder *_encoder;
    IInputSource *_input_prio_high;
    IInputSource *_input_prio_low;
    IControlStrategy *_controller;
    ILimitSwitch *_limit;
};