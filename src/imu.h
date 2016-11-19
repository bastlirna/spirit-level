#ifndef _IMU_H
#define _IMU_H

#include <Arduino.h>
#include "app.h"
#include "vector.h"
#include "matrix.h"

class IMU{
public:

  IMU() : calibration(4, 3) {};

  void initialize();
  void loop();

  bool isCalibrated();

  Vector getGravityVector();
  Vector getRawAcceleration();
  Vector calcGravityVector(Vector &vec);


  void setApp(Application *_app) {app = _app;};

  void setCalibration(Matrix &mat) {calibration = mat; }
  void resetCalibration() {calibration.diagonalOne(); }
private:
  void decomposeOffset();

  Application *app = NULL;
  Matrix calibration;

};



#endif
