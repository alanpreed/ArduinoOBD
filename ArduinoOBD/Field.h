#ifndef __FIELD_H
#define __FIELD_H

#include <stdint.h>

#define FIELD_PRINT_LENGTH 10

// List of all possible entries within measurement groups
typedef enum {
  FIELD_RPM,
  FIELD_COOLANT_TEMP,
  FIELD_L_VOLTAGE,
  FIELD_BITFIELD_8,
  FIELD_SPEED,
  FIELD_BITFIELD_4,
  FIELD_THROTTLE_ANGLE,
  FIELD_THROTTLE_ADJ_POS,
  FIELD_INJ_TIME,
  FIELD_BATT_VOLTAGE,
  FIELD_AIR_TEMP,
  FIELD_ALTITUDE_COEFF,
  FIELD_L_VALUE,
  FIELD_CONTROL_PERCENT,
  FIELD_PERCENT,
  FIELD_L_IDLE,
  FIELD_FUEL_RATE,
  FIELD_IGN_ANGLE,
  FIELD_BIT,
  FIELD_NA,
} Field;

void FormatField(Field field, uint8_t value, char *output);

#endif
