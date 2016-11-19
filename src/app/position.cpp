#include <Arduino.h>
#include "position.h"

#include "../services.h"

#define MINIMUM_LENGHT 0.15

void Position::start()
{
  started = true;
  data = imu.getGravityVector();
  Serial.println(F("APP: Position started"));
}

void Position::accData(Vector pos)
{
  Vector top(2);
  top[1] = 1;

  data = ((pos - data) * 1.0/30.0) + data;

  if((millis() - time) > 20){

    Vector norm = data.normalize();
    Vector v = norm.xy();
    float l = v.length();

    if(l < MINIMUM_LENGHT){
      if(norm[2] > 0){
        ring.fill(0x002200);
      }else{
        ring.fill(0x000022);
      }
    }else{
      ring.fill(0x000000);

      Vector vn = v.normalize();

      vpart_t angle = acos(vn.dot(top)) * 180.0 / 3.14;
      if(vn[0] > 0){
        angle = 360 - angle;
      }

      ring.color((uint16_t)angle, 0x220000);

    }
    ring.sync();

    time = millis();
  }
  
}

void Position::stop()
{
  started = false;
  Serial.println(F("APP: Calibration stopped"));
}
