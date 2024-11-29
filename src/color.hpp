#pragma once

#include <cstdint>

namespace color {
	
	void init();
	
	extern uint32_t palette16bit[65536];
	
	constexpr uint16_t convert32to16color(uint32_t color);
	constexpr uint32_t convert16to32color(uint16_t color);
	
}
