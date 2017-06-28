#ifndef __BLOCK_H
#define __BLOCK_H

// Class for a generic KW1281 block
#include <stdint.h>

// Technically, blocks can be up to 256 bytes long.  However, this makes them
// take up a lot of memory, and none of the blocks we use are nearly this long,
// so we can use smaller.
#define MAX_BLOCK_SIZE 64
#define BLOCK_END_BYTE 0x03

typedef enum {
    HEADER          = 0x02,
    CLEAR_ERRORS    = 0x05,
    END             = 0x06,
    ERRORS_REQUEST  = 0x07,
    ACK             = 0x09,
    GROUP_REQUEST   = 0x29,
    GROUP_REPLY     = 0xE7,
    DATA            = 0xF6,
    ERRORS_REPLY    = 0xFC
} BlockTitle;

class Block {
public:
  Block(){};
  
  uint8_t& len = raw_block[0];
  uint8_t& counter = raw_block[1];
  uint8_t& title = raw_block[2];

  // Create a smaller array that points to some elements within raw_blocks, 
  // taken from: 
  // http://stackoverflow.com/questions/2137361/what-is-the-best-way-to-create-a-sub-array-from-an-exisiting-array-in-c
  uint8_t (&data)[MAX_BLOCK_SIZE] = (uint8_t (&)[MAX_BLOCK_SIZE])(*(Block::raw_block + 3));
  
  uint8_t raw_block[MAX_BLOCK_SIZE + 3 + 1];
};

#endif
