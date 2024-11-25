#pragma once

#include <cstdint>

namespace ppu {
	
	void init();
	void deinit();
	void reset();
	void beforeFrame();
	void afterFrame();
	
	void clear(uint32_t color);
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint16_t options);
	void set1bitPalette(uint32_t color);
	void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	void drawCircleOutline(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color);
	void drawCircleFilled(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color);
	void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	void drawTriangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
	void drawTriangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
	void drawText(uint32_t fontOrigin, uint32_t textOrigin, uint16_t x, uint16_t y);
	void runFor(int pixelsToRun);
	
	uint32_t* getBuffer();
}