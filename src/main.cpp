#include <Arduino.h>

#include "hwconfig.h"
#include "services.h"

#include "app/calibration.h"
#include "app/position.h"

static Calibration calib;
static Position pos;

static Application* apps[] = {&calib, &pos};
static uint8_t actualApp = 0;
static Application* app = NULL;

static void startApp()
{
  app = apps[actualApp];
  if(!app->started){
    app->start();
    imu.setApp(app);
  }
}

void setup()
{
  Serial.begin(115200);

  initServices();

  Serial.println(F("Init done, starting app.."));

  if(imu.isCalibrated()){
    actualApp = 1;
  }

  startApp();
}

void loop()
{
  static uint32_t lastLoopTime = 0;
  imu.loop();

  if(app && ((millis() - lastLoopTime ) > app->loopDelay)){
    app->loop();
    lastLoopTime = millis();
  }
}
