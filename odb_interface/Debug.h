#ifndef __DEBUG_H
#define __DEBUG_H

//-- Required headers ---------------------------------------------------------
#include <stdint.h>
#include <AltSoftSerial.h>


//-- Public defines -----------------------------------------------------------
#define DEBUG


//-- Public data types ---------------------------------------------------------
class DebugSerial : public Print{
public:
    DebugSerial(){}

    void init();

    virtual size_t write(uint8_t c);
    
private:
    AltSoftSerial serial;
};


//-- Public data --------------------------------------------------------------
extern DebugSerial Debug;

#endif
