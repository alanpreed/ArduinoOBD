#include "Block.h"
#include "KW1281.h"
#include "Debug.h"
#include "Error.h"
#include "Display.h"
#include "Button.h"

// External connections
#define CONNECT_BUTTON 4
#define PLUS_BUTTON 2
#define MINUS_BUTTON 3
#define OBD_RX 0
#define OBD_TX 1

#define FIRST_GROUP 1
#define LAST_GROUP 21

typedef enum {
  DISCONNECTED,
  CONNECTING,
  RECEIVE_HEADER,
  RECEIVE_DATA,
  ERROR,
} State; 

// Button callbacks
void isr_connect_button(void);
void isr_plus_button(void);
void isr_minus_button(void);

KW1281 obd(OBD_RX,OBD_TX);
Display display;
State current_state;
Error current_error;

uint8_t current_group;
volatile uint8_t next_group;
volatile bool connect_button_pressed;

Block rx_block;
Block tx_block;

Button connect(CONNECT_BUTTON, isr_connect_button);
Button plus(PLUS_BUTTON, isr_plus_button);
Button minus(MINUS_BUTTON, isr_minus_button);

void setup() 
{ 
  Debug.init();
  Debug.println("Arduino OBD Interface");
  display.init();
  display.clear();

  current_state = DISCONNECTED;
  current_error = SUCCESS;
  current_group = FIRST_GROUP;
  next_group = current_group;
  connect_button_pressed = false;
  
  connect.enable();
  plus.enable();
  minus.enable();
}

void loop() {
  current_group = next_group;

  switch(current_state)
  {
    case DISCONNECTED:
      display.show_disconnected(current_group);
    
      if(connect_button_pressed)
      {
        current_state = CONNECTING;
        display.clear();
        connect_button_pressed = false;
      }
      break;

    case CONNECTING:
      connect.disable();
      plus.disable();
      minus.disable();

      display.show_connecting(current_group);
      
      if(obd.connect(0x01, 9600))
      {
        current_state = RECEIVE_HEADER;
        display.clear();
      }
      else{
        Debug.println(F("ERROR: Connection failed"));
        current_error = CONNECT_ERROR;
        current_state = ERROR;
        display.clear();
      }
      break;

    case RECEIVE_HEADER:
      display.show_header(current_group);
      tx_block.len = 4;
      tx_block.title = GROUP_REQUEST;
      tx_block.data[0] = current_group;

      if(!obd.send_block(tx_block))
      {
        Debug.println(F("ERROR: Initial group request failed"));
        current_error = TX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else if(!obd.receive_block(rx_block))
      {
        Debug.println(F("ERROR: Didn't receive header"));
        current_error = RX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else if(rx_block.title != HEADER)
      {
        Debug.println(F("ERROR: Wrong block type"));
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
        Debug.println(F("ERROR: Group request failed"));
        current_error = TX_ERROR;
        current_state = ERROR;
        display.clear();
      }
      else if(!obd.receive_block(rx_block) || rx_block.title != GROUP_DATA)
      {
        Debug.println(F("ERROR: Failed to receive data"));
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
}


//-- Interrupt service routines -----------------------------------------------
void isr_connect_button(void)
{
  Debug.println("Connect");
  connect_button_pressed = true;
  connect.disable();
}

void isr_plus_button(void)
{
  plus.disable();
  Debug.println("Plus");

  if(next_group == LAST_GROUP)
  {
    next_group = FIRST_GROUP;
  }
  else
  {
    next_group++;
  }
}

void isr_minus_button(void)
{
  minus.disable();
  Debug.println("Minus");

  if(next_group == FIRST_GROUP)
  {
    next_group = LAST_GROUP;
  }
  else
  {
    next_group--;
  }
}