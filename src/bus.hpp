#pragma once

#include <cstdint>
#include <string>

namespace bus {
	void write8(uint32_t address, uint8_t value);
	void write16(uint32_t address, uint16_t value);
	void write32(uint32_t address, uint32_t value);
	uint8_t read8(uint32_t address);
	uint16_t read16(uint32_t address);
	uint32_t read32(uint32_t address);
	
	void push8(uint8_t value);
	void push16(uint16_t value);
	void push32(uint32_t value);
	uint8_t pop8();
	uint16_t pop16();
	uint32_t pop32();
	
	bool load(std::string fileName);
	void unload();
	void reset();
}