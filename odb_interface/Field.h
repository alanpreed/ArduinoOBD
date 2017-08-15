#ifndef __FIELD_H
#define __FIELD_H

#include <stdint.h>

// List of all possible entries within measurement groups
typedef enum {
  FIELD_RPM,
  FIELD_TEMPERATURE,
  FIELD_L_VOLTAGE,
  FIELD_BITFIELD_8,
  FIELD_VOLTAGE,
  FIELD_SPEED,
  FIELD_BITFIELD_4,
  FIELD_ANGLE,
  FIELD_PERCENTAGE,
  FIELD_INJ_TIME, 
  FIELD_RAW, 
  FIELD_NA,
} Field;

void FormatField(Field field, uint8_t value, char *output);

#endif
