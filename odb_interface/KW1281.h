#ifndef __KW1281_H
#define __KW1281_H

// Class that implements the KW1281 message protocol
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Block.h"

#define KW1281_BAUD 9600

class KW1281 {
public:
  KW1281(uint8_t rx_pin, uint8_t tx_pin);

  // Return values indicate success or failure
  bool connect(uint8_t address, int baud);
  bool receive_block(Block &block);
  bool send_block(Block &block);

private:
  uint8_t rx_pin;
  uint8_t tx_pin;
  SoftwareSerial serial;
  uint8_t block_counter;
  
  void send_5baud(uint8_t data);
  uint8_t serial_read(void);
  void serial_write(uint8_t data);
  uint8_t compliment(uint8_t in);
};

#endif
