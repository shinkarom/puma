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
		uint32_t c = 0xFF000000 | (color << 16) | (color << 8) | color;
		return c;
	}
}
