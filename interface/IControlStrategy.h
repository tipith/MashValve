#pragma once

class IControlStrategy
{
public:
    virtual unsigned long control(bool cw) = 0;
    virtual int get_error(int current, int setpoint) = 0;
};