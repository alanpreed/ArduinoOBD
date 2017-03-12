#ifndef __KW1281_H
#define __KW1281_H

#include <SoftwareSerial.h>
#include <Arduino.h>

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


#endif

