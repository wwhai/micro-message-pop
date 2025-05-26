// Copyright (C) 2025 wwhai
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Adafruit_NeoPixel.h>
#include "neopixel_config.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void initNeopixel()
{
    strip.begin();
    strip.show();
}

void setPixelColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
    if (index >= 0 && index < LED_COUNT)
    {
        strip.setPixelColor(index, strip.Color(r, g, b));
        strip.show();
    }
}

void blinkPixel(int index, uint8_t r, uint8_t g, uint8_t b, int duration)
{
    setPixelColor(index, r, g, b);
    delay(duration);
    setPixelColor(index, 0, 0, 0);
}
