#include <LiquidCrystal.h>
#include "AckBlock.h"
//#include "EndBlock.h"
//#include "GroupRequestBlock.h"
//#include "GroupReplyBlock.h"
//#include "DataBlock.h"
#include "KW1281.h"

#define BUTTON_PIN 10

#define OBD_RX 2
#define OBD_TX 3

KW1281 odb(OBD_RX,OBD_TX);

AckBlock ack(0);

void setup() 
{
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.print("Arduino OBD interface\r\n");

  while(digitalRead(10)){
    // Wait for connection request
  }

  Serial.print("Connecting!\r\n");
  
  if(!odb.connect(0x01, 9600));
  {
    Serial.print("Connection failed!\r\n");
    while(1)
    {
    }
  }

}

void loop() {
  odb.receive_block<AckBlock>();

  if(!odb.send_block(ack))
  {
    Serial.print("Ack send failed");
    while(1)
    {
    }
  }

}
