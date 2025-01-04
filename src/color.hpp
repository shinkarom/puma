#pragma once

#include <cstdint>

namespace color {
	
	void init();
	
	extern uint32_t palette16bit[65536];
	extern uint32_t palette8bit[256];
	
	constexpr uint32_t convert16to32color(uint16_t color);
	constexpr uint32_t convert8to32color(uint8_t color);
	
}
