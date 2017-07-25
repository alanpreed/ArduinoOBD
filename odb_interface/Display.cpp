
//-- Required header files ----------------------------------------------------
#include "Display.h"
#include "Arduino.h"


//-- Private defines ----------------------------------------------------------
#define LCD_ROWS 4
#define LCD_COLUMNS 20


//-- Public function definitions ----------------------------------------------
Display::Display() : lcd(A1, A0, A5, A4, A3, A2) // LCD pins: RS, E, Data 4-7
{
}

void Display::init()
{
  // LCD dimensions: 20 characters, 4 rows
  lcd.begin(LCD_COLUMNS, LCD_ROWS);

}

void Display::clear()
{
  lcd.clear();
  lcd.setCursor(0,0);
}

void Display::show_disconnected()
{
  lcd.setCursor(0,0);
  lcd.print("Disconnected");
}

void Display::show_connecting()
{
  lcd.setCursor(0,0);
  lcd.print("Connecting");
}

void Display::show_error(Error error)
{
  lcd.setCursor(0,0);
  lcd.print("Error: ");
  lcd.setCursor(0,1);

  switch(error)
  {
    case CONNECT_ERROR:
      lcd.print("Failed to connect");
      break;
    case GROUP_ERROR:
      lcd.print("Invalid group");
      break;
    case RX_ERROR:
      lcd.print("Block RX failed");
      break;
    case TX_ERROR:
      lcd.print("Block TX failed");
      break;
    default:
      lcd.print("Unknown error");
  }
}

void Display::show_header()
{
  lcd.setCursor(0,0);
  lcd.print("Receiving header");
}

void Display::show_group(uint8_t group_id, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
  lcd.setCursor(0,0);
  lcd.print("Group: ");
  lcd.print(group_id, DEC);
  int16_t voltage = 0;

  switch(group_id)
  {
    case 0x01:
      lcd.setCursor(0,2);
      lcd.print((int)a * 32, DEC);
      lcd.print("RPM");

      lcd.setCursor(10,2);
      lcd.print((int)b - 72, DEC);
      lcd.print("C");
      
      lcd.setCursor(0,3);
      voltage = (int16_t)c * 2;
      lcd.print(voltage / 100, DEC);
      lcd.print(".");
      lcd.print(voltage % 100, DEC);
      lcd.print("V");
      
      lcd.setCursor(10,3);
      lcd.print(d, BIN);
      break;

    default:
      lcd.setCursor(0,2);
      lcd.print("1: ");
      lcd.print(a, HEX);

      lcd.setCursor(10,2);
      lcd.print("2: ");
      lcd.print(b, HEX);
      
      lcd.setCursor(0,3);
      lcd.print("3: ");
      lcd.print(c, HEX);
      
      lcd.setCursor(10,3);
      lcd.print("4: ");
      lcd.print(d, HEX);
  }
}
