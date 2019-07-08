#pragma once

#include <Encoder.h> /* https://github.com/PaulStoffregen/Encoder */
#include "../interface/IEncoder.h"

#define TRANSITIONS_PER_POS 4

class MotorEncoder : public IEncoder
{
public:
    MotorEncoder(uint8_t pina, uint8_t pinb, long pulses_per_rev) : _pulses_per_rev(pulses_per_rev)
    {
        _encoder = new Encoder(pina, pinb);
    }

    long position(void)
    {
        return _encoder->read() / TRANSITIONS_PER_POS;
    }

    void reset(void)
    {
        _encoder->write(0);
    }

private:
    Encoder* _encoder;
    long _pulses_per_rev;
};