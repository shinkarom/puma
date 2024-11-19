#pragma once

#include "common.hpp"
#include <cstdint>

namespace apu {
	void init();
	void deinit();
	void afterFrame();
	int16_t* callback();
	void writeReg(int reg, int value);
	void noteOn(int channelNum, int keyNum, int vel);
	void noteOff(int channelNum, int keyNum);
	void allNotesOff(int channelNum);
	void allSoundsOff(int channelNum);
}