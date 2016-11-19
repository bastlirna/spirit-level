#include <Arduino.h>
#include "app_asserts.h"

#ifdef USE_ASSERTS

static void _assert(const __FlashStringHelper *file, uint16_t line)
{
  Serial.print(F("ASSERT FAILED ["));
  Serial.print(file);
  Serial.print(F(":"));
  Serial.print(line);
  Serial.print(F("] "));
}

void _aInner(const __FlashStringHelper *file, uint16_t line)
{
  _assert(file, line);
  Serial.println();
  while(1);
}

void _aInnerS(const __FlashStringHelper *m, const __FlashStringHelper *file, uint16_t line)
{
  _assert(file, line);
  Serial.println(m);
  while(1);
}

#endif
