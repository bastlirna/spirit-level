#include "services.h"

#include "ring.h"
#include "imu.h"

IMU imu;
Ring ring;
Config config;

void initServices()
{
  config.initialize();
  imu.initialize();
  ring.initialize();
  ring.fill(0x090000);
  ring.sync();
}
