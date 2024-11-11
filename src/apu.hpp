#pragma once

#include "common.hpp"
#include <cstdint>

namespace apu {
	void init();
	void deinit();
	void afterFrame();
	int16_t* callback();
	void writeReg(int reg, int value);
	
	void initChannel(int chanNum, uint32_t start, uint32_t end, int smplRate);
}