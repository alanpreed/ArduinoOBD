#ifndef __GROUPSELECTOR_H
#define __GROUPSELECTOR_H

#include <stdint.h>
#include "Button.h"
#include "ECUGroups.h"

class ControlPanel{
  public:
    static ControlPanel& get_instance(uint8_t plus_pin, uint8_t minus_pin, uint8_t connect_pin);
    
    bool check_connect(void);
    bool check_selector(void);

    void disable(void);
    void enable(void);

    uint8_t current_group;

  private:
    ControlPanel(uint8_t plus_pin, uint8_t minus_pin, uint8_t connect_pin,
                  ButtonCB plus_cb, ButtonCB minus_cb, ButtonCB connect_cb);

    Button plus;
    Button minus;
    Button connect;

    void isr_plus_button(void);
    void isr_minus_button(void);
    void isr_connect_button(void);
    static void timer_cb(void);
    
    static volatile bool timer_running;
    volatile bool plus_pressed;
    volatile bool minus_pressed;
    volatile bool connect_pressed;
};

#endif
