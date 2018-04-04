#include "ControlPanel.h"
#include "Debug.h"

#include <MsTimer2.h>

#define BUTTON_COOLDOWN 100

volatile bool ControlPanel::timer_running = false;

ControlPanel& ControlPanel::get_instance(uint8_t plus_pin, uint8_t minus_pin, uint8_t connect_pin)
{
  static ControlPanel selector(plus_pin, minus_pin, connect_pin,
                                [](){selector.isr_plus_button();},
                                [](){selector.isr_minus_button();},
                                [](){selector.isr_connect_button();});

  MsTimer2::set(100, timer_cb);

  return selector;
}

ControlPanel::ControlPanel(uint8_t plus_pin, uint8_t minus_pin, uint8_t connect_pin,
                            ButtonCB plus_cb, ButtonCB minus_cb, ButtonCB connect_cb):
                            plus(plus_pin, plus_cb), minus(minus_pin, minus_cb), connect(connect_pin, connect_cb)                              
{
  current_group = FIRST_GROUP;
}

void ControlPanel::disable(void)
{
  plus.disable();
  minus.disable();
  connect.disable();
  plus_pressed = false;
  minus_pressed = false;
  connect_pressed = false;
}

void ControlPanel::enable(void)
{
  plus_pressed = false;
  minus_pressed = false;
  connect_pressed = false;
  plus.enable();
  minus.enable();
  connect.enable();
}

bool ControlPanel::check_connect(void)
{
  connect.disable();

  bool result = connect_pressed;;
  connect_pressed = false;
  
  if(result && !timer_running)
  {
    MsTimer2::start();
    timer_running = true;
  }
  else
  {
    result = false;
  }

  connect.enable();

  return result;
}

bool ControlPanel::check_selector(void)
{
  plus.disable();
  minus.disable();

  bool changed = false;

  if(plus_pressed && !minus_pressed && !timer_running)
  {
    MsTimer2::start();
    timer_running = true;

    if(current_group == NUM_GROUPS)
    {
      current_group = 1;
    }
    else
    {
      current_group++;
    }
    changed = true;
  }
  else if(minus_pressed && !plus_pressed && !timer_running)
  {
    MsTimer2::start();
    timer_running = true;

    if(current_group == 1)
    {
      current_group = NUM_GROUPS;
    }
    else
    {
      current_group--;
    }
    changed = true;
  }

  plus_pressed = false;
  minus_pressed = false;
  plus.enable();
  minus.enable();

  return changed;
}

void ControlPanel::timer_cb(void)
{
  Debug.println("Timer");
  MsTimer2::stop();
  timer_running = false;
}

void ControlPanel::isr_plus_button(void)
{
  Debug.println("Plus");
  plus_pressed = true;
  plus.disable();
}

void ControlPanel::isr_minus_button(void)
{
  Debug.println("Minus");
  minus_pressed = true;
  minus.disable();
}

void ControlPanel::isr_connect_button(void)
{
  Debug.println("Connect");
  connect_pressed = true;
  connect.disable();
}
