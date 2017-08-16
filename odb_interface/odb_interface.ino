#include "Block.h"
#include "KW1281.h"
#include "Debug.h"
#include "Error.h"
#include "Display.h"
#include "Button.h"
#include "GroupSelector.h"

// External connections
#define CONNECT_BUTTON 4
#define PLUS_BUTTON 2
#define MINUS_BUTTON 3
#define OBD_RX 0
#define OBD_TX 1

typedef enum {
  DISCONNECTED,
  CONNECTING,
  RECEIVE_HEADER,
  RECEIVE_DATA,
  ERROR,
} State; 

KW1281 obd(OBD_RX,OBD_TX);
GroupSelector& selector = GroupSelector::get_instance(PLUS_BUTTON, MINUS_BUTTON);
Display display;
State current_state;
Error current_error;

Block rx_block;
Block tx_block;

void isr_connect_button(void);
volatile bool connect_button_pressed;
Button connect(CONNECT_BUTTON, isr_connect_button);

void setup() 
{ 
  Debug.init();
  Debug.println(F("Arduino OBD Interface"));
  display.init();

  current_state = DISCONNECTED;
  current_error = SUCCESS;

  connect_button_pressed = false;
  connect.enable();

  selector.enable();
}

void loop() {
  selector.update();

  switch(current_state)
  {
    case DISCONNECTED:
      if(selector.changed)
      {
        display.clear();
      }
      display.show_disconnected(selector.current_group);
    
      if(connect_button_pressed)
      {
        current_state = CONNECTING;
        display.clear();
        selector.disable();
        connect.disable();
        connect_button_pressed = false;
      }
      break;

    case CONNECTING:
      display.show_connecting(selector.current_group);
      
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
      display.show_header(selector.current_group);
      tx_block.len = 4;
      tx_block.title = GROUP_REQUEST;
      tx_block.data[0] = selector.current_group;

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
        display.show_group(selector.current_group, 
                              rx_block.data[0], rx_block.data[1],
                              rx_block.data[2], rx_block.data[3]);
      }
      break;  

    case ERROR:
      display.show_error(current_error);
      break;
  }
}

void isr_connect_button(void)
{
  Debug.println("Connect");
  connect_button_pressed = true;
  connect.disable();
}
