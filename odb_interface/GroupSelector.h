#ifndef __GROUPSELECTOR_H
#define __GROUPSELECTOR_H

#include <stdint.h>
#include "Button.h"
#include "ECUGroups.h"

class GroupSelector{
  public:
    static GroupSelector& get_instance(uint8_t plus_pin, uint8_t minus_pin);
    
    uint8_t current_group;
    bool changed;
    void disable(void);
    void enable(void);
    void update(void);

  private:
    GroupSelector(uint8_t plus_pin, uint8_t minus_pin, ButtonCB plus_cb, ButtonCB minus_cb);
    Button plus;
    Button minus;
    bool enabled;

    void isr_plus_button(void);
    void isr_minus_button(void);
    bool plus_pressed;
    bool minus_pressed;
};

#endif
