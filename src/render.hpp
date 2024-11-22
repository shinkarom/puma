#pragma once
#include <cstdint>
#include <string>

namespace render {
	void init();
	void reset();
	bool frame(uint32_t* frameBuffer);
	void renderAudio(int16_t* audioBuffer);
	void updateInput();
	void deinit();
	bool tryLoadFile(std::string fileName);
}