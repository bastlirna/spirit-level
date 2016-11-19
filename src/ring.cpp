
#include "hwconfig.h"
#include "ring.h"
#include "SPI.h"

#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(RING_PIXELS_COUNT, RING_DATA_PIN, NEO_GRB + NEO_KHZ800);

Ring::Ring()
{
  ledAngle = 360 / RING_PIXELS_COUNT;
}

void Ring::initialize()
{
  strip.begin();
  strip.show();
}

void Ring::fill(uint32_t color)
{
  uint16_t i;
  for(i = 0; i < RING_PIXELS_COUNT; i++){
    strip.setPixelColor(i, color);
  }
}

void Ring::color(uint16_t angle, uint32_t color, bool interpolate)
{
  uint16_t ip = angle % ledAngle;
  uint16_t i = (angle / ledAngle) % RING_PIXELS_COUNT;

  if(ip == 0 || !interpolate){
    strip.setPixelColor(i, color);
  }else{
    float a = (float)ip / (float)ledAngle;
    strip.setPixelColor(i, alpha(1.0 - a, color));
    strip.setPixelColor((i + 1) % RING_PIXELS_COUNT, alpha(a, color));
  }

}

uint32_t Ring::alpha(float a, uint32_t _color)
{
  int32_t color = _color;
  uint8_t *rgb = (uint8_t*)&color;
  uint8_t i;

  for(i = 0; i < 3; i++){
    rgb[i] = ((float)rgb[i]) * a;
  }

  return color;
}

void Ring::sync()
{
  strip.show();
}

Ring::~Ring()
{

}
