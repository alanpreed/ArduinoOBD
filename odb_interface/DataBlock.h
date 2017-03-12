#ifndef __DATABLOCK_H
#define __DATABLOCK_H

// Generic KW1281 data block

#include "Block.h"

template <uint8_t DATASIZE>
class DataBlock : public Block<DATASIZE + 3> {
public:
  DataBlock(){};
  DataBlock(uint8_t counter, uint8_t data[DATASIZE])
  :Block<DATASIZE + 3>(counter, DATA){
  
    memcpy(&(Block<DATASIZE + 3>::raw_block[3]), data, DATASIZE);
  };

  // Create a smaller array that points to some elements within raw_blocks, taken 
  // from: http://stackoverflow.com/questions/2137361/what-is-the-best-way-to-create-a-sub-array-from-an-exisiting-array-in-c
  int (&data)[DATASIZE] = (int (&)[DATASIZE])(*(Block<DATASIZE + 3>::raw_block + 3));
};

#endif
