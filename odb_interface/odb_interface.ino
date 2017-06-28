#include "Block.h"
#include "KW1281.h"

#define BUTTON_PIN 10

#define OBD_RX 2
#define OBD_TX 3

KW1281 obd(OBD_RX,OBD_TX);

typedef enum {
  DISCONNECTED,
  CONNECTED,
  ERROR,
} State; 

State main_state;
uint8_t current_group;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.print("Arduino OBD interface\r\n");

  while(digitalRead(10)){
    // Wait for connection request
  }

  Serial.print("Connecting!\r\n");

  bool result = obd.connect(0x01, 9600);

  if(result)
  {
    Serial.println("Connection succeeded");
  }
  else
  {
    Serial.println("Connection failed");
    while(1);
  }
}

void loop() {
  Block ack;
  ack.len = 3;
  ack.title = ACK;

  Block rx_block;
  
  if(!obd.receive_block(rx_block))
  {
    Serial.print("Ack receive failed");
    while(1);
  }
  
  if(!obd.send_block(ack))
  {
    Serial.print("Ack send failed");
    while(1);
  }
}
