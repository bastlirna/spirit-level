#ifndef _SERVICES_H
#define _SERVICES_H

#include "ring.h"
#include "imu.h"
#include "config.h"

extern IMU imu;
extern Ring ring;
extern Config config;

void initServices();

#endif
