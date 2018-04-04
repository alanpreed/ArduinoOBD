#include "Block.h"
#include "KW1281.h"
#include "Debug.h"
#include "Error.h"
#include "Display.h"
#include "Button.h"
#include "ControlPanel.h"

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
  DISCONNECTING,
  ERROR,
} State; 

KW1281 obd(OBD_RX,OBD_TX);
ControlPanel& panel = ControlPanel::get_instance(PLUS_BUTTON, MINUS_BUTTON, CONNECT_BUTTON);
Display display;
State current_state;
Error current_error;

Block rx_block;
Block tx_block;
void enter_error_state(Error error);

void setup() 
{ 
  Debug.init();
  Debug.println(F("Arduino OBD Interface"));
  display.init();

  current_state = DISCONNECTED;
  current_error = SUCCESS;

  panel.enable();
}

void loop() {
  switch(current_state)
  {
    case DISCONNECTED:
      if(panel.check_selector())
      {
        display.clear();
      }
      display.show_disconnected(panel.current_group);
    
      if(panel.check_connect())
      {
        current_state = CONNECTING;
        panel.disable();
        display.clear();
      }
      break;

    case CONNECTING:
      display.show_connecting(panel.current_group);
      
      if(obd.connect(0x01, 9600))
      {
        current_state = RECEIVE_HEADER;
        display.clear();
      }
      else
      {
        Debug.println(F("ERROR: Connection failed"));
        enter_error_state(CONNECT_ERROR);
      }
      break;

    case RECEIVE_HEADER:
      display.show_header(panel.current_group);
      tx_block.len = 4;
      tx_block.title = GROUP_REQUEST;
      tx_block.data[0] = panel.current_group;

      if(!obd.send_block(tx_block))
      {
        Debug.println(F("ERROR: Initial group request failed"));
        enter_error_state(TX_ERROR);
      }
      else if(!obd.receive_block(rx_block))
      {
        Debug.println(F("ERROR: Didn't receive header"));
        enter_error_state(RX_ERROR);
      }
      else if(rx_block.title != HEADER && rx_block.title != GROUP_DATA) // Only the first group selection sends a header
      {
        Debug.print(F("ERROR: Wrong block type: "));
        Debug.println(rx_block.title, HEX);
        enter_error_state(RX_ERROR);
        current_state = RECEIVE_DATA;
        panel.enable();
        display.clear();
      }
      else
      {
        current_state = RECEIVE_DATA;
        panel.enable();
        display.clear();
      }
      break;

    case RECEIVE_DATA:
      if(panel.check_connect())
      {
        current_state = DISCONNECTING;
        panel.disable();
        display.clear();
      }
      else if(panel.check_selector())
      {
        current_state = RECEIVE_HEADER;
        panel.disable();
        display.clear();
      }
      else
      {
        if(!obd.send_block(tx_block))
        {
          Debug.println(F("ERROR: Group request failed"));
          enter_error_state(TX_ERROR);
        }
        else if(!obd.receive_block(rx_block) || rx_block.title != GROUP_DATA)
        {
          Debug.println(F("ERROR: Failed to receive data"));
          enter_error_state(RX_ERROR);
        }
        else
        {
          display.clear();
          display.show_group(panel.current_group, 
                                rx_block.data[0], rx_block.data[1],
                                rx_block.data[2], rx_block.data[3]);
        }
      }
      break;

    case DISCONNECTING:
      display.show_disconnecting();
      tx_block.len = 3;
      tx_block.title = END;

      if(!obd.send_block(tx_block))
      {
        Debug.println(F("ERROR: disconnect not sent"));
        enter_error_state(TX_ERROR);
      }
      else if(!obd.receive_block(rx_block) || rx_block.title != ACK)
      {
        Debug.println(F("ERROR: disconnect not acknowledged"));
        enter_error_state(RX_ERROR);
      }
      else
      {
        current_state = DISCONNECTED;
        obd.disconnect();
        panel.enable();
        display.clear();
      }

      break;

    case ERROR:
      display.show_error(current_error);

      if(panel.check_connect())
      {
        current_state = DISCONNECTED;
        current_error = SUCCESS;
        panel.enable();
        display.clear();
      }
      break;
  }
}

void enter_error_state(Error error)
{
  current_error = error;
  current_state = ERROR;
  obd.disconnect();
  panel.enable();
  display.clear();
}
