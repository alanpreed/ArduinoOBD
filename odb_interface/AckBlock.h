#ifndef __ACKBLOCK_H
#define __ACKBLOCK_H

#include "Block.h"

class AckBlock : public Block<0x03> {
public:
  AckBlock(){}
  
  AckBlock(uint8_t counter)
  :Block<0x03>(counter, ACK){};
};

#endif

