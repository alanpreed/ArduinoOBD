#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <LiquidCrystal.h>
#include "Block.h"
#include "Error.h"

class Display{
public:
  Display();
  void init(void);
  void clear(void);
  void show_disconnected(uint8_t group_id);
  void show_connecting(uint8_t group_id);
  void show_error(Error error);
  void show_header(uint8_t group_id);
  void show_group(uint8_t group_id, uint8_t a, uint8_t b, uint8_t c, uint8_t d);
  void show_disconnecting(void);
private:
LiquidCrystal lcd;
};

#endif
