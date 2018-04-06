#ifndef __BUTTON_H
#define __BUTTON_H

typedef void (*ButtonCB)(void);

class Button {
public:
  Button(uint8_t input_pin, ButtonCB callback);
  void enable(void);
  void disable(void);

private:
  uint8_t pin;
  bool enabled;
  ButtonCB callback;
};

#endif
