#pragma once
#include <cstdint>

namespace render {
	void init();
	void reset();
	bool frame(uint32_t* frameBuffer);
	void renderAudio(int16_t* audioBuffer);
	void updateInput();
	void deinit();
}