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
	
	void drawSprite(uint32_t address, int x, int y, int w, int h) {		
		for(int yy = 0; yy < h; yy++) {
			const auto yyy = y + yy;
			if(yyy > screenHeight) {
				break;
			}
			for(int xx = 0; xx < w; xx++){
				const auto xxx = x + xx;
				if(xxx>screenWidth) {
					break;
				}
				auto color = color::palette16bit[bus::read16(address + 2*w*yy + 2*xx)];
				if(!isColorTransparent(color)) {
					frameBuffer[yyy*screenWidth + xxx] = color;
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
				drawSprite(letterAddress, xOffset, y, 8, 8);
			}
			letterOffset++;
			xOffset+=8;
		} while(xOffset<screenWidth);
	}
	
}