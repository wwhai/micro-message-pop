#ifndef NEOPIXEL_HANDLER_H
#define NEOPIXEL_HANDLER_H

#include <stdint.h>

void initNeopixel();
void setPixelColor(int index, uint8_t r, uint8_t g, uint8_t b);
void blinkPixel(int index, uint8_t r, uint8_t g, uint8_t b, int duration);

#endif // NEOPIXEL_HANDLER_H