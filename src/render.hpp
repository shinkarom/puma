#pragma once
#include <cstdint>
#include <string>

namespace render {
	void init();
	bool frame(uint32_t* frameBuffer, int16_t* audioBuffer);
	void renderAudio(int16_t* audioBuffer);
	void updateInput();
	void deinit();
	bool tryLoadFile(std::string fileName);
}