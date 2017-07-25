//-- Required headers ---------------------------------------------------------
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include "Block.h"
#include "KW1281.h"
#include "Debug.h"
#include "Error.h"
#include "Display.h"


//-- Public defines -----------------------------------------------------------
#define CONNECT_BUTTON 4
#define PLUS_BUTTON 2
#define MINUS_BUTTON 3

#define OBD_RX 0
#define OBD_TX 1


//-- Public data types --------------------------------------------------------
typedef enum {
  DISCONNECTED,
  CONNECTING,
  RECEIVE_HEADER,
  RECEIVE_DATA,
  ERROR,
} State; 


//-- Public data --------------------------------------------------------------
KW1281 obd(OBD_RX,OBD_TX);
Display display;
State current_state;
Error current_error;
uint8_t current_group;
volatile uint8_t next_group;
volatile bool connect_button_pressed;
Block rx_block;
Block tx_block;


//-- Interrupt service routines -----------------------------------------------
void isr_connect_button(void)
{
  Debug.println("Connect");
  connect_button_pressed = true;
}

void isr_plus_button(void)
{
  Debug.println("Plus");
  next_group++;
}

void isr_minus_button(void)
{
  Debug.println("Minus");
  next_group--;
}


//-- Public functions ---------------------------------------------------------
void setup() 
{ 
  Debug.init();
  display.init();
  display.clear();
  Debug.println("Arduino OBD Interface");

  current_state = DISCONNECTED;
  current_error = SUCCESS;
  current_group = 1;
  next_group = 0;
  connect_button_pressed = false;
  
  // Set up buttons
  pinMode(CONNECT_BUTTON, INPUT_PULLUP);
  pinMode(PLUS_BUTTON, INPUT_PULLUP);
  pinMode(MINUS_BUTTON, INPUT_PULLUP);
  
  attachPCINT(digitalPinToPinChangeInterrupt(CONNECT_BUTTON), isr_connect_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(PLUS_BUTTON), isr_plus_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(MINUS_BUTTON), isr_minus_button, FALLING);
}

void loop() {
  switch(current_state)
  {
    case DISCONNECTED:
      display.show_disconnected();
    
      if(connect_button_pressed)
      {
        current_state = CONNECTING;
        display.clear();
        connect_button_pressed = false;
      }
      break;

    case CONNECTING:
      display.show_connecting();
      
      if(obd.connect(0x01, 9600))
      {
        current_state = RECEIVE_HEADER;
        display.clear();
      }
      else{
        Debug.println("ERROR: Connection failed");
        current_error = CONNECT_ERROR;
        current_state = ERROR;
        display.clear();
      }
      break;

    case RECEIVE_HEADER:
      display.show_header();
      tx_block.len = 4;
      tx_block.title = GROUP_REQUEST;
      tx_block.data[0] = current_group;

      if(!obd.send_block(tx_block))
      {
        Debug.println("ERROR: Initial group request failed");
        current_error = TX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else if(!obd.receive_block(rx_block))
      {
        Debug.println("ERROR: Didn't receive header");
        current_error = RX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else if(rx_block.title != HEADER)
      {
        Debug.println("ERROR: Wrong block type");
        Debug.println(rx_block.title, HEX);
        current_error = RX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else
      {
        current_state = RECEIVE_DATA;
        display.clear();
      }
      break;

    case RECEIVE_DATA:
      if(!obd.send_block(tx_block))
      {
        Debug.println("ERROR: Group request failed");
        current_error = TX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else if(!obd.receive_block(rx_block) || rx_block.title != GROUP_DATA)
      {
        Debug.println("ERROR: Failed to receive data");
        current_error = RX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else
      {
        display.clear();
        display.show_group(current_group, rx_block.data[0], rx_block.data[1],
                                            rx_block.data[2], rx_block.data[3]);
      }
      break;  

    case ERROR:
      display.show_error(current_error);
      break;
  }

//  current_group = next_group;
}
