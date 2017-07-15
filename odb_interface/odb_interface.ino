#include <PinChangeInterruptBoards.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include "Block.h"
#include "KW1281.h"
#include "Debug.h"

// Pin change interrupt button
#define CONNECT_BUTTON 4
#define PLUS_BUTTON 2
#define MINUS_BUTTON 3

#define OBD_RX 0
#define OBD_TX 1


KW1281 obd(OBD_RX,OBD_TX);


typedef enum {
  DISCONNECTED,
  CONNECTED,
  ERROR,
} State; 

volatile bool connect_button_pressed = false;

void isr_connect_button(void)
{
  Debug.println("Connect");
  connect_button_pressed = true;
}

void isr_plus_button(void)
{
  Debug.println("Plus");
}

void isr_minus_button(void)
{
  Debug.println("Minus");
}

void setup() 
{ 
  Debug.init();
  Debug.println("Arduino OBD Interface");
  
  // // Set up buttons
  pinMode(CONNECT_BUTTON, INPUT_PULLUP);
  pinMode(PLUS_BUTTON, INPUT_PULLUP);
  pinMode(MINUS_BUTTON, INPUT_PULLUP);
  
  attachPCINT(digitalPinToPinChangeInterrupt(CONNECT_BUTTON), isr_connect_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(PLUS_BUTTON), isr_plus_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(MINUS_BUTTON), isr_minus_button, FALLING);
  
  while(!connect_button_pressed)
  {
    
  }
  
  Debug.println("Connecting!");

  while(1);
  // bool result = obd.connect(0x01, 9600);

  // if(result)
  // {
  //   debug.println("Connection succeeded");
  // }
  // else
  // {
  //   debug.println("Connection failed");
  //   while(1);
  // }
}

void loop() {
  // Block ack;
  // ack.len = 3;
  // ack.title = ACK;

  // Block rx_block;
  
  // if(!obd.receive_block(rx_block))
  // {
  //   Debug.print("Ack receive failed");
  //   while(1);
  // }
  
  // if(!obd.send_block(ack))
  // {
  //   Debug.print("Ack send failed");
  //   while(1);
  // }  
}
