#ifndef __GROUPREQUESTBLOCK_H
#define __GROUPREQUESTBLOCK_H

// KW1281 block for requesting a group measurement

#include "Block.h"
#include "Groups.h"

class GroupRequestBlock : public Block<0x04> {
public:
  GroupRequestBlock(){};
  GroupRequestBlock(uint8_t counter, GroupID group_id)
  :Block<0x04>(counter, GROUP_REQUEST){
    
    this->group_id = (uint8_t)group_id;
  };

  uint8_t& group_id = raw_block[3];
};

#endif

