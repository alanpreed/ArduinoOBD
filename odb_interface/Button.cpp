#include <PinChangeInterruptBoards.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include "Button.h"
#include "Debug.h"

Button::Button(uint8_t pin, ButtonCB callback): 
                pin(pin), callback(callback)
{
  pinMode(pin, INPUT_PULLUP);
}

void Button::enable()
{
  if(!enabled)
  {
    enabled = true;
    
    // On the UNO, only pins 2 and 3 can use external interrupts.  All others must
    // use port change interrupts.
    if(pin == 2 || pin == 3)
    {
      attachInterrupt(digitalPinToInterrupt(pin), callback, FALLING);  
    }
    else
    {
      attachPCINT(digitalPinToPinChangeInterrupt(pin), callback, FALLING);
    }
  }
}

void Button::disable()
{
  if(enabled)
  {
    enabled = false;
    
    // On the UNO, only pins 2 and 3 can use external interrupts.  All others must
    // use port change interrupts.
    if(pin == 2 || pin == 3)
    {
       detachInterrupt(digitalPinToInterrupt(pin));  
    }
    else
    {
      detachPCINT(digitalPinToPinChangeInterrupt(pin));
    }
  }
}

