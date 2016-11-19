#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

#include "matrix.h"

Configuration Conf;

void Config::initialize()
{
  load();
}

void Config::load()
{
  uint16_t i;
  //Serial.print(F("Loading settings:"));
  for(i = 0; i < sizeof(Configuration); i++){
    ((uint8_t*)&data)[i] = (uint8_t)EEPROM.read(EEPROM_CONFIG_POS + i);
    //Serial.print(EEPROM.read(EEPROM_CONFIG_POS + i), HEX);
    //Serial.print(F(" "));
  }
  //Serial.println();

  if(data.length != sizeof(Configuration)){
    Serial.println(F("Settings invalid - loading defaults"));
    loadDefaults();
  }
}

void Config::loadDefaults()
{
  Matrix mat(CALIBRATION_MATRIX_ROWS, CALIBRATION_MATRIX_COLLS);
  mat.diagonalOne();
  mat.save(data.calibration, CALIBRATION_MATRIX_ROWS * CALIBRATION_MATRIX_COLLS);

  store();
}

void Config::store()
{
  uint16_t i;
  data.length = sizeof(Configuration);

  for(i = 0; i < sizeof(Configuration); i++){
    EEPROM.write(EEPROM_CONFIG_POS + i, ((uint8_t*)&data)[i]);
  }
}
