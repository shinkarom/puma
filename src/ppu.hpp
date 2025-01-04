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
	void drawText(uint16_t fontOrigin, uint32_t textOrigin, uint16_t x, uint16_t y);
	void runFor(int scanlinesToRun);
	void setClearColor(uint32_t color);
	
	uint32_t* getBuffer();
}