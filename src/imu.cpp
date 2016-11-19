#include "ADXL345.h"
#include "Wire.h"
#include "imu.h"

#include "services.h"
#include "matrix.h"

static ADXL345 accel;

#define MOVE_TRESHOLD 20
#define MEASURE_PERIOD 5
static Vector lastAcceleration(3);
static Vector offset(3);

void IMU::initialize()
{
  Wire.begin();
  accel.initialize();
  accel.setRange(ADXL345_RANGE_16G);
  accel.setRate(ADXL345_RATE_200);

  lastAcceleration = getRawAcceleration();

  calibration.load(config.data.calibration, CALIBRATION_MATRIX_ROWS * CALIBRATION_MATRIX_COLLS);

  Serial.println(F("IMU initialized with calibration:"));
  calibration.println(Serial);

  if(isCalibrated()){
    decomposeOffset();
  }
}


void IMU::decomposeOffset()
{
  Matrix a = calibration.part(3, 3);
  a.transpose();

  Vector b = calibration[3].vector() * -1;
  b[2] = b[2] + 1;

  Vector v = a.solve(b);

  //v.println(Serial);
  //getRawAcceleration().println(Serial);
  offset = getRawAcceleration() - v;

  Serial.println(F("Offset: "));
  offset.println(Serial);

}


bool IMU::isCalibrated()
{
  return abs((calibration[0][0] * calibration[1][1] * calibration[2][2]) - 1) > 0.3;
}

Vector IMU::getRawAcceleration()
{
  int16_t ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);

  return Vector3(ax, ay, az);
}

Vector IMU::calcGravityVector(Vector &vec)
{
  Matrix v(1, 4);
  Vector vreal = vec - offset;
  v[0] = vreal;
  v[0][3] = 1;

  return (v * calibration)[0].vector();
}

Vector IMU::getGravityVector()
{
  Vector acc = getRawAcceleration();
  return calcGravityVector(acc);
}

void IMU::loop()
{
  static uint32_t lastMeasure = 0;

  if((millis() - lastMeasure) > MEASURE_PERIOD){
    Vector v = getRawAcceleration();
    Vector gv = calcGravityVector(v);
    if(app != NULL){
      app->accDataRaw(v);
    }

    if(app != NULL){
      app->accData(gv);
    }

    if((v - lastAcceleration).length() > MOVE_TRESHOLD && app != NULL){
      lastAcceleration = v;
      app->accMove(gv);
    }
  }

}
