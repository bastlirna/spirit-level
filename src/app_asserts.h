#ifndef _APP_ASSERTS_H
#define _APP_ASSERTS_H

#include <Arduino.h>
#include "hwconfig.h"

#ifdef USE_ASSERTS

#define appAssert(cond) if(!(cond)) {_aInner(F(__FILE__), __LINE__);}
#define appAssertMessage(cond, message) if(cond) {_aInnerS(F(message), F(__FILE__), __LINE__);}

void _aInner(const __FlashStringHelper *file, uint16_t line);
void _aInnerS(const __FlashStringHelper *m, const __FlashStringHelper *file, uint16_t line);

#else

#define assert(cond, message)

#endif

#endif
