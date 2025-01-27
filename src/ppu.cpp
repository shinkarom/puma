#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include "bus.hpp"

constexpr uint32_t convert8to32color(uint8_t color) {
	// Extract the individual RGB components from the RGB332 format
	uint8_t r = (color >> 5) & 0b111;    // Top 3 bits
	uint8_t g = (color >> 2) & 0b111;    // Middle 3 bits
	uint8_t b = color & 0b11;            // Bottom 2 bits

	// Expand the bit ranges to 8-bit values
	uint8_t r8 = (r << 5) | (r << 2) | (r >> 1);  // Map 3 bits to 8 bits
	uint8_t g8 = (g << 5) | (g << 2) | (g >> 1);  // Map 3 bits to 8 bits
	uint8_t b8 = (b << 6) | (b << 4) | (b << 2) | b;  // Map 2 bits to 8 bits

	// Combine into a 32-bit RGBA value (full opacity)
	uint32_t rgba = (0xFF << 24) | (r8 << 16) | (g8 << 8) | b8;

	return rgba;
}

namespace ppu {
	
	uint32_t palette8bit[256];
	uint32_t frameBuffer[screenTotalPixels];
	int drawnPixels;
	
	void init() {	
		 reset();
		 for(int i = 0; i < 256; i++) {
			 palette8bit[i] = convert8to32color(i);
		}
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer, 0, screenTotalPixels * sizeof(uint32_t));
	}
	
	void beforeFrame() {
		drawnPixels = 0;
	}
	
	
	void afterFrame() {
		
	}
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint8_t transparentIndex, uint8_t flags) {
		if(drawnPixels >= drawnPixelQuota || w > screenWidth || h > screenHeight) {
			return;
		}

		bool flipHorizontal = flags & 0x01;       // Bit 0
		bool flipVertical = flags & 0x02;         // Bit 1
		bool doDrawAfterWrapHorizontal = flags & 0x04;       // Bit 2
		bool doDrawAfterWrapVertical = flags & 0x08;         // Bit 3
		bool noDrawBeforeWrapHorizontal = flags & 0x10; // Bit 4
		bool noDrawBeforeWrapVertical = flags & 0x20;   // Bit 5
		
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
			
			rowBase[row] = address + w * rowOffset[row];
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
				uint8_t colorIndex = bus::read8(rowBase[row] + colOffset[col]);
				uint32_t fullColor = palette8bit[colorIndex];

				if (colorIndex != transparentIndex) {
					frameBuffer[frameBufferRowBase[row] + colWrapped[col]] = fullColor;
					drawnPixels++;
				}
			}
		}
	}

	
	uint32_t* getBuffer() {
		return frameBuffer;
	}
	
	void clearScreen(uint8_t index){
		auto color = palette8bit[index];
		for (int i = 0; i<screenTotalPixels; i++) {
			frameBuffer[i] = color;
		}
	}
	
}