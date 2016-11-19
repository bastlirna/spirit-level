#ifndef _APP_H
#define _APP_H


#include "vector.h"

class Application{
public:
  Application() {};
  virtual void start() = 0;
  virtual void stop() = 0;

  bool started = false;

  virtual void accData(Vector pos) {};
  virtual void accDataRaw(Vector pos) {};
  virtual void accMove(Vector pos) {};
  virtual void loop() {};

  uint32_t loopDelay;

};



#endif
