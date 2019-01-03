/*  SK6182Serial - Non-blocking SK6182 LED Display Library
    Modification of WS2812Serial for use with RGBW LED strips
    https://github.com/PaulStoffregen/WS2812Serial
    Copyright (c) 2017 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef SK6182Serial_h_
#define SK6182Serial_h_

#include <Arduino.h>
#include "DMAChannel.h"

#define SK6182_RGBW      0       // The WS2811 datasheet documents this way
#define SK6182_RBGW      1
#define SK6182_GRBW      2       // Most LED strips are wired this way
#define SK6182_GBRW      3
#define SK6182_BRGW      4
#define SK6182_BGRW      5

class SK6182Serial {
public:
	constexpr SK6182Serial(uint16_t num, void *fb, void *db, uint8_t pin, uint8_t cfg) :
		numled(num), pin(pin), config(cfg),
		frameBuffer((uint8_t *)fb), drawBuffer((uint8_t *)db) {
	}
	bool begin();
	void setPixel(uint32_t num, int color) {
		if (num >= numled) return;
		num *= 3;
		drawBuffer[num+0] = color & 255;
		drawBuffer[num+1] = (color >> 8) & 255;
		drawBuffer[num+2] = (color >> 16) & 255;
		drawBuffer[num+3] = (color >> 24) & 255;
	}
	void setPixel(uint32_t num, uint8_t red, uint8_t green, uint8_t blue, uint8_t white=0) {
		if (num >= numled) return;
		num *= 3;
		drawBuffer[num+0] = blue;
		drawBuffer[num+1] = green;
		drawBuffer[num+2] = red;
		drawBuffer[num+3] = white;
	}
	void clear() {
        	memset(drawBuffer, 0, numled * 4);
	} 	
	void show();
	bool busy();
	uint16_t numPixels() {
		return numled;
	}
private:
	const uint16_t numled;
	const uint8_t pin;
	const uint8_t config;
	uint8_t *frameBuffer;
	uint8_t *drawBuffer;
	DMAChannel *dma = nullptr;
	uint32_t prior_micros = 0;
};

#endif
