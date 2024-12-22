#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include "bus.hpp"
#include "color.hpp"

constexpr bool isColorTransparent(uint32_t color) {
		return (color & 0xFF000000)  == 0;
}

namespace ppu {
	
	uint32_t frameBuffer[screenTotalPixels];
	
	void init() {	
		 reset();
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer,0,screenTotalPixels*sizeof(uint32_t));
	}
	
	void beforeFrame() {
		
	}
	
	
	void afterFrame() {
		
	}
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint8_t flags) {

		// Decode the flags from the uint8_t
		bool flipHorizontal = flags & 0x01;       // Bit 0
		bool flipVertical = flags & 0x02;         // Bit 1
		bool doDrawAfterWrapHorizontal = flags & 0x04;       // Bit 2
		bool doDrawAfterWrapVertical = flags & 0x08;         // Bit 3
		bool noDrawBeforeWrapHorizontal = flags & 0x10; // Bit 4
		bool noDrawBeforeWrapVertical = flags & 0x20;   // Bit 5
		uint8_t paletteSelection = (flags >> 6) & 0x07; // Bits 6 to 8

		for (int row = 0; row < h; row++) {
			
			int rowAdjusted = flipVertical ? (h - 1 - row) : row;
			
			const auto yyy = (y + row) % screenHeight;
			if (yyy >= y && noDrawBeforeWrapVertical) {
				continue; 
			}
			if (yyy < y && !doDrawAfterWrapVertical) {
				continue;
			}

			for (int col = 0; col < w; col++) {
				// Apply horizontal flipping if enabled
				int colAdjusted = flipHorizontal ? (w - 1 - col) : col;

				const auto xxx = (x + col) % screenWidth;
				if (xxx >= x && noDrawBeforeWrapHorizontal) {
					continue; 
				}
				if(xxx < x && !doDrawAfterWrapHorizontal) {
					continue;
				}

				// Determine the correct color read method based on the color depth flag
				uint32_t pixelColor;
				if (!paletteSelection) {
					// Use 16-bit color palette
					pixelColor = color::palette16bit[bus::read16(address + 2 * w * rowAdjusted + 2 * colAdjusted)];
				} else {
					// Use 8-bit color palette based on paletteSelection
					pixelColor = color::palette8bit[paletteSelection - 1][bus::read8(address + w * rowAdjusted + colAdjusted)];
				}

				if (!isColorTransparent(pixelColor)) {
					frameBuffer[yyy * screenWidth + xxx] = pixelColor;
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