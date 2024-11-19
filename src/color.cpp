#include "color.hpp"

#include <iostream>

namespace color {
	
	uint32_t palette16bit[65536];
	 
	void init() {
		for(int i = 0; i < 65536; i++) {
			 palette16bit[i] = convert16to32color(i);
		 }
	}
	
	/// Convert 32-bit ARGB8888 to 16-bit ARGB4444
	constexpr uint16_t convert32to16color(uint32_t color) {
		uint8_t a = (color >> 24) & 0xFF; // Alpha
		uint8_t r = (color >> 16) & 0xFF; // Red
		uint8_t g = (color >> 8) & 0xFF;  // Green
		uint8_t b = color & 0xFF;         // Blue

		// Convert to ARGB1555
		uint16_t a1 = (a > 127) ? 1 : 0;  // Convert 8-bit alpha to 1-bit
		uint16_t r5 = r >> 3;             // Convert 8-bit red to 5-bit
		uint16_t g5 = g >> 3;             // Convert 8-bit green to 5-bit
		uint16_t b5 = b >> 3;             // Convert 8-bit blue to 5-bit

		// Combine into ARGB1555
		uint16_t argb1555 = (a1 << 15) | (r5 << 10) | (g5 << 5) | b5;
		return argb1555;
	}

	/// Convert 16-bit ARGB4444 to 32-bit ARGB8888
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
		return argb8888;
	}
	
}
