#include "Debug.h"

#define DEBUG_BAUD 9600
#define DEBUG_TX 8
#define DEBUG_RX 9

void DebugSerial::init(void)
{
#ifdef DEBUG
    pinMode(DEBUG_RX, INPUT_PULLUP);
    pinMode(DEBUG_TX, OUTPUT);
    serial.begin(DEBUG_BAUD);
#endif
}

size_t DebugSerial::write(uint8_t c)
{
#ifdef DEBUG
    serial.write(c);
#endif
}

DebugSerial Debug;
