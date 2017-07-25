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
  void show_disconnected(void);
  void show_connecting(void);
  void show_error(Error error);
  void show_header(void);
  void show_group(uint8_t group_id, uint8_t a, uint8_t b, uint8_t c, uint8_t d);
private:
LiquidCrystal lcd;
};

#endif
