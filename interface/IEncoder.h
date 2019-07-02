#pragma once

class IEncoder
{
public:
    virtual long position(void) = 0;
    virtual void reset(void) = 0;
};