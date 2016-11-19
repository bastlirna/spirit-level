#ifndef _APP_POSITION_H
#define _APP_POSITION_H

#include <Arduino.h>

#include "../vector.h"
#include "../app.h"

class Position : public Application
{
public:
  Position() : data(3), time(0) {};
  void start();
  void stop();

  void accData(Vector pos);
private:
  Vector data;
  uint32_t time = 0;
};



#endif
