#ifndef __ENDBLOCK_H
#define __ENDBLOCK_H

// KW1281 block that signals the termination of the connection

#include "Block.h"

class EndBlock : public Block<0x03> {
public:
  EndBlock(){};
  EndBlock(uint8_t counter)
  :Block<0x03>(counter, END){};

};

#endif
