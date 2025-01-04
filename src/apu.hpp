#pragma once

#include "common.hpp"
#include <cstdint>

namespace apu {
	void init();
	void reset();
	void deinit();
	void afterFrame();
	void frame();
	int16_t* getBuffer();
	void writeReg(int reg, int value);
	void clearBuffer();
}