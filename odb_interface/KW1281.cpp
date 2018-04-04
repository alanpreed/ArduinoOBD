#include "KW1281.h"
#include "Block.h"
#include "Debug.h"

KW1281::KW1281(uint8_t rx_pin, uint8_t tx_pin) : rx_pin(rx_pin),
                                                 tx_pin(tx_pin)
{
  pinMode(rx_pin, INPUT_PULLUP);
  pinMode(tx_pin, OUTPUT);
  digitalWrite(tx_pin, HIGH);
}

//-- Public functions ---------------------------------------------------------

bool KW1281::connect(uint8_t address, int baud)
{
  block_counter = 0;

  Debug.print("Sending address at 5 baud\r\n");
  send_5baud(address);

  // Use hardware serial port for KW1281 communication
  Serial.begin(KW1281_BAUD);

  Block rx_block;
  Block ack_block;
  ack_block.len = 3;
  ack_block.title = ACK;

  Debug.print("Receiving initial bytes\r\n");

  // Initial 3 data bytes are treated differently
  uint8_t initial_data[3];
  for (uint8_t i = 0; i < 3; i++)
  {
    initial_data[i] = serial_read();

    Debug.print(i + 1);
    Debug.print(": ");
    Debug.println(initial_data[i], HEX);
  }

  // Check we received correct sync byte and keyword: 0x55, 0x01, 0x8A
  if (initial_data[0] != 0x55 || initial_data[1] != 0x01 || initial_data[2] != 0x8A)
  {
    return false;
  }
  else
  {
    serial_write(compliment(initial_data[2]));
  }

  // Read 4x initial connection data blocks
  for(size_t i = 0; i < 4; i++)
  {
    Debug.print("Receiving info block ");
    Debug.println(i + 1);
    if (!receive_block(rx_block))
    {
      return false;
    }

    Debug.println("Sending ACK");
    if (!send_block(ack_block))
    {
      return false;
    }
  }

  Debug.println("Receiving ACK");
  if(!receive_block(rx_block))
  {
    return false;
  }
  return true;
}

void KW1281::disconnect(void)
{
  Serial.end();
}

bool KW1281::receive_block(Block &rx_block)
{
  rx_block.len = serial_read();
  if (rx_block.len > MAX_BLOCK_SIZE)
  {
    Debug.print("ERROR: block too long: ");
    Debug.println(rx_block.len);
    return false;
  }
  serial_write(compliment(rx_block.len));

  rx_block.counter = serial_read();
  serial_write(compliment(rx_block.counter));
  if (rx_block.counter != ++block_counter)
  {
    Debug.println("ERROR: wrong block counter");
    
    Debug.print("Expected: ");
    Debug.print(block_counter, HEX);
    Debug.print(", received: ");
    Debug.println(rx_block.counter, HEX);
    return false;
  }

  rx_block.title = serial_read();
  serial_write(compliment(rx_block.title));

  for (uint8_t i = 0; i < rx_block.len - 3; i++)
  {
    rx_block.data[i] = serial_read();
    serial_write(compliment(rx_block.data[i]));
  }

  // Don't send a compliment for the last byte
  uint8_t block_end = serial_read();
  if (block_end != BLOCK_END_BYTE)
  {
    Debug.print("ERROR: expected block end");
    Debug.print(", received: ");
    Debug.println(block_end, HEX);
    return false;
  }

  return true;
}

bool KW1281::send_block(Block &tx_block)
{

  tx_block.counter = ++block_counter;

  if (tx_block.len > MAX_BLOCK_SIZE)
  {
    Debug.println("ERROR: block too long");
    return false;
  }

  uint8_t response = 0;
  for (uint8_t i = 0; i < tx_block.len; i++)
  {
    // Send each block byte and check the response
    serial_write(tx_block.raw_block[i]);
    response = serial_read();

    if (response != compliment(tx_block.raw_block[i]))
    {
      Debug.println("ERROR: invalid compliment");
      Debug.print("Expected: ");
      Debug.print(compliment(tx_block.raw_block[i]), HEX);
      Debug.print(", received: ");
      Debug.println(response, HEX);
      return false;
    }
  }

  // Send block end byte
  serial_write(BLOCK_END_BYTE);

  return true;
}

//-- Private functions --------------------------------------------------------

uint8_t KW1281::serial_read(void)
{
  unsigned long timeout = millis() + 1000;

  // Wait for data
  while (!Serial.available())
  {
    if (millis() >= timeout)
    {
      Debug.println(F("ERROR: serial timeout\r\n"));
      return 0;
    }
  }
  
  return Serial.read();
}

void KW1281::serial_write(uint8_t data)
{
  // Brief wait before writing, to prevent interfering with previous byte
  unsigned volatile long timer = millis() + 2;
  while (millis() < timer);

  Serial.write(data);

  // Handle loopback from bidirectional KW1281 connection
  uint8_t rx_data = serial_read();

  if(rx_data != data)
  {
    Debug.print("Error! Echo byte incorrect");
  }
}

uint8_t KW1281::compliment(uint8_t in)
{
  return 0xFF ^ in;
}

// Send a byte of data at 5 baud
void KW1281::send_5baud(uint8_t data)
{
  // 1 start bit, 7 data bits, 1 parity bit, 1 stop bit
  #define BITCOUNT 10

  uint8_t bits[BITCOUNT];
  uint8_t parity = 1;
  uint8_t bit;

  // Form array of bits to send
  for (int i = 0; i < BITCOUNT; i++)
  {
    bit = 0;

    if (i == 0)
    {
      // Start bit
      bit = 0;
    }
    else if (i == 8)
    {
      // Parity bit (odd)
      bit = parity;
    }
    else if (i == 9)
    {
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
  for (int i = 0; i < BITCOUNT; i++)
  {
    if (bits[i] == 1)
    {
      digitalWrite(tx_pin, HIGH);
    }
    else
    {
      digitalWrite(tx_pin, LOW);
    }

    // Wait 200 ms (5 baud), adjusted by latency correction
    delay(200);
  }
}
