
#pragma once

#include "../interface/IControlStrategy.h"
#include <PID_v1.h>

#define PWM_LIMIT 255

class ControlStrategyPID : public IControlStrategy
{
public:
    ControlStrategyPID(double Kp, double Ki, double Kd, long positions) : _setpoint(0), _input(0), _output(0), _Kp(Kp), _Ki(Ki), _Kd(Kd)
    {
        _poscount = positions;
        _pid = new PID(&_input, &_output, &_setpoint, _Kp, _Ki, _Kd, DIRECT);
        _pid->SetMode(AUTOMATIC);
        _pid->SetOutputLimits(-PWM_LIMIT, PWM_LIMIT);
        _pid->SetSampleTime(200);
    }

    long control(long process_pos, long setpoint)
    {
        _input = process_pos;
        _setpoint = setpoint;
        _pid->Compute();
        return (long)_output;
    }

private:
    PID* _pid;

    double _setpoint;
    double _input;
    double _output;

    double _Kp;
    double _Ki;
    double _Kd;
};