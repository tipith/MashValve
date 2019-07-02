#pragma once

class IMotorDriver
{
public:
    virtual unsigned long set_dir(bool cw) = 0;
    virtual unsigned long set_dc(unsigned long dutycycle) = 0;
    virtual int get_dc(void) = 0;
};