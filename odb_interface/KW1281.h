#ifndef __KW1281_H
#define __KW1281_H

#include <SoftwareSerial.h>
#include <Arduino.h>
#include "AckBlock.h"

#define KW1281_BAUD 9600

class KW1281 {
public:
  KW1281(uint8_t rx_pin, uint8_t tx_pin);

  bool connect(uint8_t address, int baud);

  template <typename BLOCK_T>
  BLOCK_T receive_block(void);

  template <typename BLOCK_T>
  bool send_block(BLOCK_T& block);

private:
  SoftwareSerial serial;

  uint8_t rx_pin;

  uint8_t tx_pin;
  
  bool connected;
  uint8_t block_counter;
  
  void send_5baud(uint8_t data);
  uint8_t serial_read(void);
  void serial_write(uint8_t data);
  uint8_t compliment(uint8_t in);
};

// Templated send/receive functions are defined here so that 
// specialisations in other cpp files can be compiled

// Note that BLOCK_T should inherit from Block!
template <typename BLOCK_T>
BLOCK_T KW1281::receive_block(void) {

  uint8_t rx_len = sizeof(BLOCK_T::raw_block);

  Serial.print("RX block length: ");
  Serial.println(rx_len, DEC);
  
  BLOCK_T rx_block;
  
  for (uint8_t i = 0; i < rx_len; i++)
  {
    rx_block.raw_block[i] = serial_read();
    
    // Respond to all but the last byte
    if (i < (rx_len - 1))
    {
      serial_write(compliment(rx_block.raw_block[i]));
    }
  }

  block_counter++;

  return rx_block;
}

// Note that BLOCK_T should inherit from Block!
template <typename BLOCK_T>
bool KW1281::send_block(BLOCK_T& block) {

  block.counter = ++block_counter;
  
  uint8_t response = 0;
  for (uint8_t i = 0; i <= block.len; i++)
  { 
    serial_write(block.raw_block[i]);

    // Check the ECU's response to all bytes except the last
    if (i < block.len)
    {
      response = serial_read();

      if (response != compliment(block.raw_block[i]))
      {
        return false;
      }
    }
  }
}

#endif

