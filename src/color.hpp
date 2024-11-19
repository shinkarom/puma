#pragma once

#include <cstdint>

namespace color {
	
	void init();
	
	extern uint32_t palette16bit[65536];
	extern uint32_t palette8bit[256];
	extern uint32_t palette4bit[64];
	
	constexpr uint16_t convert32to16color(uint32_t color);
	constexpr uint32_t convert16to32color(uint16_t color);
	
}
