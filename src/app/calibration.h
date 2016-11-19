#ifndef _APP_CALIBRATION_H
#define _APP_CALIBRATION_H

#include <Arduino.h>

#include "../vector.h"
#include "../app.h"

class Calibration : public Application
{
public:

  Calibration();
  ~Calibration();
  void start();
  void stop();

  void accDataRaw(Vector pos);
  void accMove(Vector pos);
  void loop();
private:
  void rollEffect();
  void measureEffect();
  void calculate();
  void allocateVectors();
  void deallocateVectors();
  uint16_t ledAngle;
  uint8_t ledState;

  Vector **positions;
  Vector lastPos;

  enum CalibState{
    WAITING = 0,
    MEASURE,
    WAIT_TO_NEXT,
    CALCULATE,
    FINISH
  };

  enum CalibDir{
    ZP = 0,
    ZM,
    XP,
    XM,
    YP,
    YM,
    DONE
  };

  enum CalibState state;
  uint32_t stateDuration;
  enum CalibDir direction;
};



#endif
