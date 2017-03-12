#ifndef __GROUPREPLYBLOCK_H
#define __GROUPREPLYBLOCK_H

// KW1281 block containing group measurement data

#include "Block.h"
#include "Groups.h"

class GroupReplyBlock : public Block<0x0F> {
public:
  GroupReplyBlock(){};
  GroupReplyBlock(uint8_t counter, uint8_t data[12])
  :Block<0x0F>(counter, GROUP_REPLY){
  
    memcpy(&raw_block[3], data, 12);
  };

  uint8_t& data_1_id = raw_block[3];
  uint8_t& data_1_a = raw_block[4];
  uint8_t& data_1_b = raw_block[5];

  uint8_t& data_2_id = raw_block[6];
  uint8_t& data_2_a = raw_block[7];
  uint8_t& data_2_b = raw_block[8];

  uint8_t& data_3_id = raw_block[9];
  uint8_t& data_3_a = raw_block[10];
  uint8_t& data_3_b = raw_block[11];

  uint8_t& data_4_id = raw_block[12];
  uint8_t& data_4_a = raw_block[13];
  uint8_t& data_4_b = raw_block[14];
};

#endif
