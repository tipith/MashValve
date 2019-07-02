#pragma once

#include <Encoder.h> /* https://github.com/PaulStoffregen/Encoder */
#include "../interface/IEncoder.h"

class MotorEncoder : public IEncoder
{
public:
    MotorEncoder(uint8_t pina, uint8_t pinb)
    {
        _encoder = new Encoder(pina, pinb);
    }

    long position(void)
    {
        return _encoder->read();
    }

    void reset(void)
    {
        _encoder->write(0);
    }

private:
    Encoder* _encoder;
};