#include "Display.h"
#include "Arduino.h"
#include "Field.h"
#include "ECUGroups.h"

#define LCD_ROWS 4
#define LCD_COLUMNS 20

Display::Display() : lcd(A0, A1, A2, A3, A4, A5) // LCD pins: RS, E, Data 4-7
{
}

void Display::init()
{
  // LCD dimensions: 20 characters, 4 rows
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  clear();
}

void Display::clear()
{
  lcd.clear();
  lcd.setCursor(0,0);
}

void Display::show_disconnected(uint8_t group_id)
{
  lcd.setCursor(0,0);
  lcd.print("Disconnected");

  lcd.setCursor(0,2);
  lcd.print("Group: ");
  lcd.print(group_id, DEC);
}

void Display::show_connecting(uint8_t group_id)
{
  lcd.setCursor(0,0);
  lcd.print("Connecting");
  
  lcd.setCursor(0,2);
  lcd.print("Group: ");
  lcd.print(group_id, DEC);
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

void Display::show_header(uint8_t group_id)
{
  lcd.setCursor(0,0);
  lcd.print("Receiving header");

  lcd.setCursor(0,2);
  lcd.print("Group: ");
  lcd.print(group_id, DEC);
}

void Display::show_group(uint8_t group_id, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
  char str[FIELD_PRINT_LENGTH + 1];

  lcd.setCursor(0,0);
  lcd.print("Group: ");
  lcd.print(group_id, DEC);

  lcd.setCursor(0,2);
  FormatField(ECUGroups[group_id - 1][0], a, str);
  lcd.print(str);

  lcd.setCursor(10,2);
  FormatField(ECUGroups[group_id - 1][1], b, str);
  lcd.print(str);
  
  lcd.setCursor(0,3);
  FormatField(ECUGroups[group_id - 1][2], c, str);
  lcd.print(str);
  
  lcd.setCursor(10,3);
  FormatField(ECUGroups[group_id - 1][3], d, str);
  lcd.print(str);
}

void Display::show_disconnecting(void)
{
  lcd.setCursor(0,0);
  lcd.print("Disconnecting");
}