#include "KW1281.h"
#include "Block.h"
#include "AckBlock.h"
#include "DataBlock.h"
#include "EndBlock.h"
#include "GroupReplyBlock.h"
#include "GroupRequestBlock.h"

#define BUFFER_SIZE 64;

KW1281::KW1281(uint8_t rx_pin, uint8_t tx_pin):
  serial(rx_pin, tx_pin, false),
  rx_pin(rx_pin),
  tx_pin(tx_pin)
{
  //Constructor code
}

/*
   Public functions
*/

bool KW1281::connect(uint8_t address, int baud) {

  block_counter = 0;

  serial.begin(KW1281_BAUD);
  
  Serial.print("Sending address at 5 baud\r\n");
  send_5baud(address);

  Serial.print("Receiving initial bytes\r\n");
  // Initial data bytes are treated differently
  uint8_t initial_data[3];
  for (uint8_t i = 0; i < 3; i++)
  {
    initial_data[i] = serial_read();
  }

  // Sync byte and keyword: 0x55, 0x01, 0x8A
  if (initial_data[0] != 0x55
      || initial_data[1] != 0x01
      || initial_data[2] != 0x8A)
  {
    // Wrong data received
    return false;
  }
  else
  {
    serial_write(compliment(initial_data[2]));
  }

  // Read 4x initial connection data blocks
  // It is possible that the lengths of these are specific to different ECUs
  Serial.write("Receiving info block 1\r\n");
  DataBlock<0x0F> initial_1 = receive_block<DataBlock<0x0F>>();
  AckBlock ack(++block_counter);
  if (!send_block(ack))
  {
    return false;
  }

  Serial.write("Receiving info block 2\r\n");
  DataBlock<0x0F> initial_2 = receive_block<DataBlock<0x0F>>();
  ack.counter = (++block_counter);
  if (!send_block(ack))
  {
    return false;
  }

  Serial.write("Receiving info block 3\r\n");
  DataBlock<0x0E> initial_3 = receive_block<DataBlock<0x0E>>();
  ack.counter = (++block_counter);
  if (!send_block(ack))
  {
    return false;
  }

  Serial.write("Receiving info block 4\r\n");
  DataBlock<0x08> initial_4 = receive_block<DataBlock<0x08>>();
  ack.counter = (++block_counter);
  if (!send_block(ack))
  {
    return false;
  }

  connected = true;
  return true;
}


// Note that BLOCK_T should inherit from Block!
template <typename BLOCK_T>
BLOCK_T KW1281::receive_block(void) {

  uint8_t rx_len = sizeof(BLOCK_T::raw_block);

  BLOCK_T rx_block;

  for (uint8_t i = 0; i <= rx_len; i++)
  {
    rx_block.raw_block[i] = serial_read();

    // Respond to all but the last byte
    if (i < rx_len)
    {
      serial.write(compliment(rx_block.raw_block[i]));
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

/*
   Private helper functions
*/

uint8_t KW1281::serial_read(void) {
  unsigned long timeout = millis() + 1000;

  // Wait for data
  while (!serial.available()) {
    if (millis() >= timeout) {
      Serial.println(F("ERROR: serial timeout\r\n"));
      //disconnect();
      return 0;
    }
  }
  return serial.read();
}

void KW1281::serial_write(uint8_t data) {
  serial.write(data);
}


uint8_t KW1281::compliment(uint8_t in)
{
  return 0xFF ^ in;
}

// Send a byte of data at 5 baud
void KW1281::send_5baud(uint8_t data) {

  // 1 start bit, 7 data bits, 1 parity bit, 1 stop bit
#define bitcount 10
  uint8_t bits[bitcount];
  uint8_t parity = 1;
  uint8_t bit;

  // Form array of bits to send
  for (int i = 0; i < bitcount; i++) {
    bit = 0;

    if (i == 0) {
      // Start bit
      bit = 0;
    }
    else if (i == 8) {
      // Parity bit (odd)
      bit = parity;
    }
    else if (i == 9) {
      // Stop bit
      bit = 1;
    }
    else
    {
      // Data bits
      bit = (data & (1 << (i - 1))) != 0;
      parity = parity ^ bit;
    }

    bits[i] = bit;
  }

  // Send bits
  for (int i = 0; i < bitcount; i++) {
    if (bits[i] == 1) {
      digitalWrite(tx_pin, HIGH);
    }
    else {
      digitalWrite(tx_pin, LOW);
    }

    // Wait 200 ms (5 baud), adjusted by latency correction
    delay(200);
  }
  serial.flush();
}








