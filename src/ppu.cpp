#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include "bus.hpp"
#include "color.hpp"

namespace ppu {
	
	uint32_t frameBuffer[screenTotalPixels];
	int drawnPixels;
	uint32_t clearColor;
	uint8_t transparentIndex;
	
	void init() {	
		 reset();
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer, 0, screenTotalPixels * sizeof(uint32_t));
		clearColor = defaultClearColor;
		transparentIndex = defaultTransparentIndex;
	}
	
	void beforeFrame() {
		drawnPixels = 0;
		for (int i = 0; i<screenTotalPixels; i++) {
			frameBuffer[i] = clearColor;
		}
	}
	
	
	void afterFrame() {
		
	}
	
	void setClearColor(uint32_t color) {
		clearColor = (color & 0xFF000000) == 0x00000000 ? defaultClearColor : color;
	}
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint8_t flags) {
		if(drawnPixels >= drawnPixelQuota) {
			return;
		}

		bool flipHorizontal = flags & 0x01;       // Bit 0
		bool flipVertical = flags & 0x02;         // Bit 1
		bool doDrawAfterWrapHorizontal = flags & 0x04;       // Bit 2
		bool doDrawAfterWrapVertical = flags & 0x08;         // Bit 3
		bool noDrawBeforeWrapHorizontal = flags & 0x10; // Bit 4
		bool noDrawBeforeWrapVertical = flags & 0x20;   // Bit 5
		bool paletteSelection = flags& 0x40; // Bit 6
		
		int rowOffset[h], frameBufferRowBase[h];
		bool noDrawY[h];
		uint32_t rowBase[h];
		int rowWrapped;
		
		for (int row = 0; row < h; row++) {
			rowOffset[row] = flipVertical ? (h - 1 - row) : row;
			rowWrapped = (y + row) % screenHeight;
			noDrawY[row] = false;
			if (rowWrapped >= y && noDrawBeforeWrapVertical) {
				noDrawY[row] = true; 
			} else
			if (rowWrapped < y && !doDrawAfterWrapVertical) {
				noDrawY[row] = true;
			}
			frameBufferRowBase[row] = rowWrapped * screenWidth;
			
			if (paletteSelection) {
				rowBase[row] = address + 2 * w * rowOffset[row];
			} else {
				rowBase[row] = address + w * rowOffset[row];
			}
		}
		
		int colOffset[w],colWrapped[w];
		bool noDrawX[w];
		for (int col = 0; col < w; col++) {
			colOffset[col] = flipHorizontal ? (w - 1 - col) : col;
			colWrapped[col] = (x + col) % screenWidth;
			noDrawX[col] = false;
			if (colWrapped[col] >= x && noDrawBeforeWrapHorizontal) {
				noDrawX[col] = true; 
			} else
			if(colWrapped[col] < x && !doDrawAfterWrapHorizontal) {
				noDrawX[col] = true; 
			}
		}
		
		for (int row = 0; row < h; row++) {
			if (noDrawY[row]) {
				continue; 
			}
			for (int col = 0; col < w; col++) {
				if (noDrawX[col]) {
					continue; 
				}
				uint8_t colorIndex;
				if (paletteSelection) {
					// tbi
				} else {
					colorIndex = bus::read8(rowBase[row] + colOffset[col]);
				}

				if (colorIndex != 0) {
					frameBuffer[frameBufferRowBase[row] + colWrapped[col]] = color::palette8bit[colorIndex];;
					drawnPixels++;
				}
			}
		}
	}

	
	uint32_t* getBuffer() {
		return frameBuffer;
	}

	void drawText(uint16_t fontOrigin, uint32_t textOrigin, uint16_t x, uint16_t y) {
		auto letterOffset = textOrigin;
		auto xOffset = x;
		do {
			char letter = bus::read8(letterOffset);
			if(letter == 0) {
				break;
			}
			auto letterAddress = fontOrigin + (letter-32) * 8 *8;
			if(letter<32 || letter>127) {
				
			} else {
				drawSprite(letterAddress, xOffset, y, 8, 8,0x00);
			}
			letterOffset++;
			xOffset+=8;
		} while(xOffset<screenWidth);
	}
	
	void setTransparentIndex(uint8_t index) {
		transparentIndex = index;
	}
	
}