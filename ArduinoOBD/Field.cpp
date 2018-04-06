#include "Field.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void FormatField(Field field, uint8_t value, char *output)
{
  switch(field)
  {
    case FIELD_RPM:
      // 32 RPM per bit, no offset
      sprintf(output, "%u RPM", (int)value * 32);
      break;

    case FIELD_COOLANT_TEMP:
      // 1 degree Celsius per bit, apparent offset of -95
      sprintf(output, "%dC", (int)value - 95);
      break;

    case FIELD_L_VOLTAGE:
      // 0.02V per bit, no offset
      sprintf(output, "%u.%uV", ((int16_t)value * 2) / 100, ((int16_t)value * 2) % 100);
      break;

    case FIELD_BITFIELD_8:
      sprintf(output, "%u%u%u%u%u%u%u%u",
                (value & 0x80) >> 7,
                (value & 0x40) >> 6,
                (value & 0x20) >> 5,
                (value & 0x10) >> 4,
                (value & 0x08) >> 3,
                (value & 0x04) >> 2,
                (value & 0x02) >> 1,
                (value & 0x01) >> 0);
      break;
        
    case FIELD_SPEED:
      // Unknown decoding
      sprintf(output, "%u", value);
      break;

    case FIELD_BITFIELD_4:
      sprintf(output, "%u%u%u%u",
                (value & 0x08) >> 3,
                (value & 0x04) >> 2,
                (value & 0x02) >> 1,
                (value & 0x01) >> 0);
      break;

    case FIELD_THROTTLE_ANGLE:
      {
        // 0.5 degrees per bit, no offset
        uint8_t scaled_value = (((int16_t)value * 10) / 2);
        sprintf(output, "%u.%u deg.", scaled_value / 10, scaled_value % 10);
      }
      break;

    case FIELD_THROTTLE_ADJ_POS:
      // Doesn't fit exactly, but appears to be roughly 1% per bit, offset of -122
      sprintf(output, "%d%%", (int)value - 122);
      break;

    case FIELD_INJ_TIME:
      // 0.1 ms per bit, no offset
      sprintf(output, "%u.%ums", value / 10, value % 10);
      break; 

    case FIELD_BATT_VOLTAGE:
      {
        // Doesn't fit exactly, but appears to be roughly 0.1V per bit, offset of 0.2
        uint16_t voltage = value + 2;
        sprintf(output, "%u.%uV", voltage / 10, voltage % 10);
      }
      break;

    case FIELD_AIR_TEMP:
      // 1 degree Celsius per bit, apparent offset of -77
      sprintf(output, "%dC", (int)value - 77);
      break;

    case FIELD_ALTITUDE_COEFF:
      // 5 units per bit, no offset
      sprintf(output, "%u", (uint16_t)value * 5);
      break;

    case FIELD_L_VALUE:
      {
        // Lambda value = 1/256 * (value + 128)
        float lambda = ((float)value + 128.0) / 256 * 100;
        // AVR library can't print floats directly
        sprintf(output, "%u.%u", (int)(lambda / 100), (int)roundf(lambda) % 100);
      }
      break;

    case FIELD_CONTROL_PERCENT:
      {
        // Doesn't fit exactly, but appears to be roughly 1/2(value - 128)
        float percent = ((float)value -128.0) / 2.0 * 10;
        sprintf(output, "%d.%d%%", (int)percent / 10, abs((int)roundf(percent) % 10));
      }
      break;

    case FIELD_PERCENT:
      {
        // Percentage ranging from 0-100
        float percent = ((float)value * 100.0) / 255.0 * 10;
        sprintf(output, "%u.%u%%", (int)percent / 10, (int)roundf(percent) % 10);
      }
      break;

    case FIELD_L_IDLE:
      // Unknown decoding
      sprintf(output, "%u", value);
      break;

    case FIELD_FUEL_RATE:
      // Unknown decoding
      sprintf(output, "%u", value);
      break;

    case FIELD_IGN_ANGLE:
      {
        // Appears to be 0.38 * (value - 128), unsure why 0.38 exactly.
        // Possibly related to some_angle/256 or something similar.
        float angle = ((float)value - 128.0) * 0.38 * 100;
        sprintf(output, "%d.%d deg.", (int)(angle / 100), abs((int)roundf(angle) % 100));
      }
      break;

    case FIELD_BIT:
      sprintf(output, "%u", value & 0x01);
      break;

    case FIELD_NA:
      sprintf(output, "(%u)", value);
      break;

    default:
      sprintf(output, "%x", value);
      break;
  }

  // Pad output with spaces to fill display
  for(int i = strlen(output); i < FIELD_PRINT_LENGTH; i++)
  {
    output[i] = ' ';
  }
  output[FIELD_PRINT_LENGTH] = '\0';
}
