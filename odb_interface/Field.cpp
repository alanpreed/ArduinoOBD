#include "Field.h"
#include <stdio.h>

void FormatField(Field field, uint8_t value, char *output)
{
  switch(field)
  {
    case FIELD_RPM:
      sprintf(output, "%u RPM", (int)value * 32);
      break;

    case FIELD_TEMPERATURE:
      sprintf(output, "%uC", (int)value - 72);
      break;

    case FIELD_L_VOLTAGE:
      sprintf(output, "%u.%uV", ((int16_t)value * 2) / 100, ((int16_t)value * 2) % 100);
      break;

    case FIELD_BITFIELD_8:
      sprintf(output, "%x", value);
      break;
        
    case FIELD_VOLTAGE:
      sprintf(output, "%x", value);
      break;

    case FIELD_SPEED:
      sprintf(output, "%x", value);
      break;

    case FIELD_BITFIELD_4:
      sprintf(output, "%x", value);
      break;

    case FIELD_ANGLE:
      sprintf(output, "%x", value);
    break;

    case FIELD_PERCENTAGE:
      sprintf(output, "%x", value);
      break;

    case FIELD_INJ_TIME:
      sprintf(output, "%x", value);
      break; 

    case FIELD_RAW:
      sprintf(output, "%x", value);
      break; 
        
    case FIELD_NA:
      sprintf(output, "%x", value);
      break;

    default:
      return;
  }
}