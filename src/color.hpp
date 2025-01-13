#pragma once

#include <cstdint>

namespace color {
	
	void init();
	
	extern uint32_t palette8bit[256];
	
	constexpr uint32_t convert8to32color(uint8_t color);
	
}
