#pragma once

#include <cstdint>

namespace color {
	
	void init();
	
	extern uint32_t palette16bit[65536];
	extern uint32_t palette8bit[256];
	extern uint32_t palette4bit[64];
	
	constexpr uint16_t convert32to16color(uint32_t color);
	constexpr uint8_t convert32to8color(uint32_t color);
	constexpr uint8_t convert32to4color(uint32_t color);
	constexpr uint32_t convert16to32color(uint16_t color);
	constexpr uint8_t convert16to8color(uint16_t color4444);
	constexpr uint8_t convert16to4color(uint16_t color4444);
	constexpr uint32_t convert8to32color(uint8_t color);
	constexpr uint16_t convert8to16color(uint8_t color2222);
	constexpr uint8_t convert8to4color(uint8_t color2222);
	constexpr uint32_t convert4to32color(uint8_t color);
	constexpr uint16_t convert4to16color(uint8_t color1111);
	constexpr uint8_t convert4to8color(uint8_t color1111);
	
	uint32_t blendARGB(uint32_t src, uint32_t dst);
	
}
