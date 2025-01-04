#include "color.hpp"

#include <iostream>

#include "common.hpp"

namespace color {
	
	uint32_t palette16bit[65536];
	uint32_t palette8bit[256];
	 
	void init() {
		for(int i = 0; i < 65536; i++) {
			 palette16bit[i] = convert16to32color(i);
		}
		for(int i = 0; i < 256; i++) {
			 palette8bit[i] = convert8to32color(i);
		}
		
	}


	constexpr uint32_t convert16to32color(uint16_t color) {
		// Extract individual channels from ARGB1555
		uint8_t a1 = (color >> 15) & 0x1; // Alpha
		uint8_t r5 = (color >> 10) & 0x1F; // Red
		uint8_t g5 = (color >> 5) & 0x1F;  // Green
		uint8_t b5 = color & 0x1F;         // Blue

		// Convert to ARGB8888
		uint8_t a = a1 ? 255 : 0;         // Convert 1-bit alpha to 8-bit
		uint8_t r = r5 << 3 | r5 >> 2;    // Convert 5-bit red to 8-bit
		uint8_t g = g5 << 3 | g5 >> 2;    // Convert 5-bit green to 8-bit
		uint8_t b = b5 << 3 | b5 >> 2;    // Convert 5-bit blue to 8-bit

		// Combine into ARGB8888
		uint32_t argb8888 = (a << 24) | (r << 16) | (g << 8) | b;
		if(!a) {
			return transparentColor;
		}
		return argb8888;
	}
	
	
	constexpr uint32_t convert8to32color(uint8_t color) {
		uint32_t c = 0xFF000000 | (color << 16) | (color << 8) | color;
		if(color == 0) {
			return transparentColor;
		} else {
			return c;
		}
	}
}
