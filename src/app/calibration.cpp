#include <Arduino.h>

#include "calibration.h"

#include "../services.h"
#include "../vector.h"
#include "../matrix.h"

#define WAITING_DURATION 4000
#define MINMAX_ALLOWED_DISTANCE 40
#define DIRECTION_TRESHOLD 25
#define MEASURE_DURATION 5000

Calibration::Calibration() : Application(), ledAngle(0), ledState(0), lastPos(3), state(WAITING)
{

}

void Calibration::start()
{
  allocateVectors();
  started = true;
  stateDuration = millis();
  state = WAITING;
  imu.resetCalibration();
  *positions[ZP] = imu.getRawAcceleration();
  lastPos = imu.getRawAcceleration();
  ledAngle = 0;
  ledState = 0;
  loopDelay = 20;
  direction = ZP;

  Serial.println(F("APP: Calibration started"));
}

void Calibration::deallocateVectors()
{
  if(positions == NULL){
    return;
  }

  uint8_t i;
  for(i = 0; i < 6; i++){
    delete positions[i];
  }
  free(positions);
  positions = NULL;
}

void Calibration::allocateVectors()
{
  uint8_t i;
  positions = (Vector**)malloc(sizeof(Vector*) * 6);

  for(i = 0; i < 6; i++){
    positions[i] = new Vector(3);
  }
}

void Calibration::accMove(Vector pos)
{
  switch(state){
    case MEASURE:
      state = WAITING;
    case WAIT_TO_NEXT:
      stateDuration = millis();
    default:
      break;
  }

}

void Calibration::accDataRaw(Vector pos)
{
  if(state == MEASURE){
    *positions[direction] = (*positions[direction] + pos) / 2.0;
  }
}

void Calibration::loop()
{

  if(state == WAITING || state == WAIT_TO_NEXT){
    loopDelay = 20;

    if((millis() - stateDuration) > WAITING_DURATION){
      float l = (lastPos - imu.getRawAcceleration()).length();
      Serial.print(F("diff "));
      Serial.println(l);
      if(state != WAIT_TO_NEXT || l > MINMAX_ALLOWED_DISTANCE){
        state = MEASURE;
        Serial.println(F("wait check"));
      }
      stateDuration = millis();
    }

    measureEffect();
  }else if(state == MEASURE){
    loopDelay = 100;

    if((millis() - stateDuration) > MEASURE_DURATION){

      Serial.println(F("next measure"));

      direction = (CalibDir) ((uint8_t)direction + 1);

      if(direction == DONE){
        state = CALCULATE;
        Serial.println(F("set CALCULATE"));
      }else{
        state = WAIT_TO_NEXT;
        Serial.println(F("set WAIT_TO_NEXT"));
        *positions[direction] = imu.getRawAcceleration();
      }

      lastPos = imu.getRawAcceleration();
      stateDuration = millis();
    }

    measureEffect();
  }else if(state == CALCULATE){
    loopDelay = 10;

    Serial.print(F("Z+ "));
    positions[ZP]->println(Serial);

    Serial.print(F("Z- "));
    positions[ZM]->println(Serial);

    Serial.print(F("X+ "));
    positions[XP]->println(Serial);

    Serial.print(F("X- "));
    positions[XM]->println(Serial);

    Serial.print(F("Y+ "));
    positions[YP]->println(Serial);

    Serial.print(F("Y- "));
    positions[YM]->println(Serial);

    Serial.println();

    calculate();

    state = FINISH;
    ledAngle = 0;
    direction = ZP;
  }else if(state == FINISH){
    loopDelay = 20;

    if((millis() - stateDuration) > 200){
      imu.getGravityVector().println(Serial);
      stateDuration = millis();
    }

    rollEffect();
  }


}

void Calibration::calculate()
{
  Matrix w(6, 4);
  Matrix y(6, 3);

  w.fill(1);

  w[0] = *positions[XP];
  w[1] = *positions[YP];
  w[2] = *positions[ZP];

  w[3] = *positions[XM];
  w[4] = *positions[YM];
  w[5] = *positions[ZM];

  deallocateVectors();

  y[0][0] = 1;
  y[1][1] = 1;
  y[2][2] = 1;

  y[3][0] = -1;
  y[4][1] = -1;
  y[5][2] = -1;

  Matrix wt(w);
  wt.transpose();

  Matrix winv = (wt * w).inv();

  Matrix x = (winv * wt) * y;

  Serial.println(F("Calibration matrix is: "));
  x.println(Serial);

  x.save(config.data.calibration, CALIBRATION_MATRIX_ROWS * CALIBRATION_MATRIX_COLLS);
  config.store();
  imu.setCalibration(x);

}

void Calibration::stop()
{
  deallocateVectors();
  started = false;
  Serial.println(F("APP: Calibration stopped"));
}

void Calibration::measureEffect()
{
  ledState = !ledState;

  ring.fill(0x000000);

  if(ledState || state != MEASURE){
    switch(direction){
      case YM:
        ring.color(0, 0x220000, false);
        break;
      case XP:
        ring.color(90, 0x002200, false);
        break;
      case YP:
        ring.color(180, 0x000022, false);
        break;
      case XM:
        ring.color(270, 0x220022, false);
        break;
      case ZP:
      case ZM:
        rollEffect();
        return;
      default:
        break;
    }
  }

  ring.sync();
}

void Calibration::rollEffect()
{
  if(direction == ZM){
    ledAngle -= 2;
  }else{
    ledAngle += 2;
  }

  ring.fill(0x000000);
  ring.color(ledAngle, 0x220000);
  ring.color(ledAngle + 120, 0x002200);
  ring.color(ledAngle + 240, 0x000022);
  ring.sync();
}

Calibration::~Calibration()
{
  deallocateVectors();
}
