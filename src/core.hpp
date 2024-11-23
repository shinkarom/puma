#pragma once

#include <cstdint>
#include <string>

namespace core {
	void init();
	void reset();
	void deinit();
	bool frame(uint32_t** videoBuffer, int16_t** audioBuffer);
	bool tryLoadFile(std::string fileName);
}
