#pragma once
#include <cstdint>

namespace render {
	void init();
	bool frame(uint32_t* frameBuffer, int16_t* audioBuffer);
	void updateInput();
	void deinit();
}