#ifndef _RING_H
#define _RING_H

#include <Arduino.h>

#define COLOR_BLACK 0x0
#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF
#define COLOR_WHITE 0xFFFFFF

class Ring{
public:
  Ring();
  ~Ring();

  void initialize();
  void fill(uint32_t color);
  void color(uint16_t angle, uint32_t color, bool interpolate = true);
  void sync();
private:
  uint32_t alpha(float a, uint32_t color);
  int16_t ledAngle;
};




#endif
