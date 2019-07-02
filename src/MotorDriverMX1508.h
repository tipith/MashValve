#pragma once

#include <MX1508.h> /* https://github.com/Saeterncj/MX1508 */
#include "../interface/IMotorDriver.h"

class MotorDriverMX1508 : public IMotorDriver 
{
public:
    MotorDriverMX1508(uint8_t pin1, uint8_t pin2)
    {
        _hbridge = new MX1508(pin1, pin2);
    }

    unsigned long set_dir(bool cw)
    {

    }

    unsigned long set_dc(unsigned long dutycycle)
    {
        _hbridge->motorGo(dutycycle);
    }

    int get_dc(void)
    {
        return _hbridge->getPWM();
    }


private:
    MX1508* _hbridge;
};