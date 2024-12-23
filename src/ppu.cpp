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
	
	void init() {	
		 reset();
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer,0,screenTotalPixels*sizeof(uint32_t));
	}
	
	void beforeFrame() {
		drawnPixels = 0;
	}
	
	
	void afterFrame() {
		
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
		uint8_t paletteSelection = (flags >> 6) & 0x07; // Bits 6 to 8
		
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
			
			if (!paletteSelection) {
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
				uint32_t pixelColor;
				if (!paletteSelection) {
					pixelColor = color::palette16bit[bus::read16(rowBase[row] + 2 * colOffset[col])];
				} else {
					pixelColor = color::palette8bit[paletteSelection - 1][bus::read8(rowBase[row] + colOffset[col])];
				}

				if (pixelColor != transparentColor) {
					frameBuffer[frameBufferRowBase[row] + colWrapped[col]] = pixelColor;
				}
				drawnPixels++;
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
			auto letterAddress = fontOrigin + (letter-32) * 8 *8 *2;
			if(letter<32 || letter>127) {
				
			} else {
				drawSprite(letterAddress, xOffset, y, 8, 8,0);
			}
			letterOffset++;
			xOffset+=8;
		} while(xOffset<screenWidth);
	}
	
}