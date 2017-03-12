#ifndef __BLOCK_H
#define __BLOCK_H

// Base class for a generic KW1281 block

typedef enum {
    CLEAR_ERRORS    = 0x05,
    END             = 0x06,
    ERRORS_REQUEST  = 0x07,
    ACK             = 0x09,
    GROUP_REQUEST   = 0x29,
    GROUP_REPLY     = 0xE7,
    DATA            = 0xF6,
    ERRORS_REPLY    = 0xFC
} BlockTitle;

template <uint8_t LENGTH>
class Block {
public:
  Block(){};
  Block(uint8_t counter, BlockTitle title);
  uint8_t& len = raw_block[0];
  uint8_t& counter= raw_block[1];
  uint8_t& title = raw_block[2];
  
//protected:
  uint8_t raw_block[LENGTH + 1];
};

template <uint8_t LENGTH>
Block<LENGTH>::Block(uint8_t counter, BlockTitle title){ 
  this->len = LENGTH;
  this->counter = counter;
  this->title = (uint8_t)title;

  // Block end byte
  this->raw_block[LENGTH] = 0x03;
}

#endif

