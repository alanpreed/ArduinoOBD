#ifndef __GROUPS_H
#define __GROUPS_H

// KW1281 groups for the Skoda Felicia

// TODO: Fill in remaining groups

typedef enum {
  IDLING_TEST = 0x01,
  INJ_TIME = 0x02,
  LOAD_STATE = 0x03,
  OPERATING_STATE = 0x04,
  GROUP5 = 0x05,
  GROUP6 = 0x06,
  BASIC_VALUES = 0x07,
  IDLING_STABIL = 0x08,
  IDLE_AIR_USE = 0x09,
  LAMBDA_CONTROL = 0x0A,
  LAMBDA_VAL_1 = 0x0B,
  LAMBDA_VAL_2 = 0x0C
} GroupID;

// TODO: Find out IDs for each sensor
typedef enum {

  
} DataID;

#endif

