#ifndef _CONFIG_H
#define _CONFIG_H

#include "hwconfig.h"
#include "vector.h"

#define CALIBRATION_MATRIX_ROWS 4
#define CALIBRATION_MATRIX_COLLS 3

typedef struct{
  uint16_t length;
  vpart_t calibration[CALIBRATION_MATRIX_ROWS * CALIBRATION_MATRIX_COLLS];
}Configuration;

class Config{
public:
  Config() {}

  void initialize();
  void store();
  void load();
  void loadDefaults();

  Configuration data;
};


#endif
