#include "color.hpp"

#include <iostream>

#include "common.hpp"

namespace color {
	
	uint32_t palette8bit[256];
	 
	void init() {
		for(int i = 0; i < 256; i++) {
			 palette8bit[i] = convert8to32color(i);
		}
		
	}
	
	constexpr uint32_t convert8to32color(uint8_t color) {
		// Extract the individual RGB components from the RGB332 format
		uint8_t r = (color >> 5) & 0b111;    // Top 3 bits
		uint8_t g = (color >> 2) & 0b111;    // Middle 3 bits
		uint8_t b = color & 0b11;            // Bottom 2 bits

		// Expand the bit ranges to 8-bit values
		uint8_t r8 = (r << 5) | (r << 2) | (r >> 1);  // Map 3 bits to 8 bits
		uint8_t g8 = (g << 5) | (g << 2) | (g >> 1);  // Map 3 bits to 8 bits
		uint8_t b8 = (b << 6) | (b << 4) | (b << 2) | b;  // Map 2 bits to 8 bits

		// Combine into a 32-bit RGBA value (full opacity)
		uint32_t rgba = (0xFF << 24) | (r8 << 16) | (g8 << 8) | b8;

		return rgba;
	}
}
