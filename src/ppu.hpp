	#pragma once

#include <cstdint>

namespace ppu {
	
	void init();
	void deinit();
	void reset();
	void beforeFrame();
	void afterFrame();
	
	//void drawSprite(uint32_t address, int x, int y, int w, int h);
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint8_t flags);
	void runFor(int scanlinesToRun);
	void setClearColor(uint32_t color);
	void setTransparentIndex(uint8_t index);
	
	uint32_t* getBuffer();
}