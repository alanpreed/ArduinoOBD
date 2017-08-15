#ifndef __GROUPSELECTOR_H
#define __GROUPSELECTOR_H

#include <stdint.h>
#include "Button.h"
#include "ECUGroups.h"

class GroupSelector{
  public:
    GroupSelector(uint8_t plus_pin, uint8_t minus_pin);
    uint8_t current_group;
    bool changed;
    void disable(void);
    void enable(void);
    void update(void);

  private:
    Button plus;
    Button minus;
    bool enabled;

    static void isr_plus_button(void);
    static void isr_minus_button(void);
    static bool plus_pressed;
    static bool minus_pressed;
};

#endif
