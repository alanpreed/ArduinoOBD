#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>
#include <AltSoftSerial.h>

#define DEBUG

class DebugSerial : public Print{
public:
    DebugSerial(){}

    void init();

    virtual size_t write(uint8_t c);
    
private:
    AltSoftSerial serial;
};

extern DebugSerial Debug;

#endif
