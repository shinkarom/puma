#pragma once
#include <cstdint>

namespace render {
	void init();
	bool frame(uint32_t* frameBuffer);
	void deinit();
}