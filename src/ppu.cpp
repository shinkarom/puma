#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include "bus.hpp"
#include "color.hpp"

constexpr auto HFLIP_MASK = 1<<0;
constexpr auto VFLIP_MASK = 1<<1;

int currentPixel, currentXPos, currentYPos;
uint32_t* pixelPointer;
int addedSprites;
int objAddr;
std::vector<int> spritesOnScanline;

namespace ppu {
	
	uint32_t frameBuffer[screenTotalPixels];
	uint32_t secondBuffer[screenTotalPixels];
	
	void inline setFullPixel(int x, int y, uint32_t color) {
		if(x < 0 || x >= screenWidth || y<0 || y >= screenHeight || ((color&0xFF000000) == 0x00000000)) {
			return;
		}
		secondBuffer[y*screenWidth+x] = color;
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<std::hex<<color<<std::dec<<std::endl;
	}
	
	void init() {
		std::cout<<std::hex<<"OAM: "<<oamOffset<<std::dec<<std::endl;
		 reset();
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer,0,screenTotalPixels*sizeof(uint32_t));
		memset(secondBuffer,0,screenTotalPixels*sizeof(uint32_t));
	}
	
	void beforeFrame() {
		currentPixel = 0;
		currentXPos = 0;
		currentYPos = 0;
		pixelPointer = &frameBuffer[0];		
		addedSprites = 0;
		objAddr = oamOffset;
	}
	
	uint32_t evaluatePixel(int x, int y) {
		for(auto i = spritesOnScanline.begin(); i != spritesOnScanline.end(); ) {
			auto oamCursor = oamOffset+(*i)*10;
			auto addr = bus::read32(oamCursor);			
			if(addr == 0) {
				break;
			}
			auto xx = bus::read8(oamCursor+4);
			auto yy = bus::read8(oamCursor+5);
			auto ww = bus::read8(oamCursor+6);
			auto hh = bus::read8(oamCursor+7);
			auto options = bus::read16(oamCursor+8);
			int yOffset = y-yy;
			int xOffset = x-xx;
			if(yOffset<0 || yOffset>=hh) {
				i = spritesOnScanline.erase(i);
				continue;
			}
			if(xOffset<0 || xOffset >= ww) {
				++i;
				continue;
			}
			if(options && VFLIP_MASK) {
				yOffset = hh-yOffset;
			}
			if(options && HFLIP_MASK) {
				xOffset = ww-xOffset;
			}
			auto newPixel = color::palette16bit[bus::read16(addr+2*ww*yOffset+2*xOffset)];
			//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
			if((newPixel&0xFF000000)==0xFF000000) {
				return newPixel;
			}
			++i;
		}
		return 0xFF000000;
	}
	
	void runFor(int pixelsToRun) {
		if(pixelsToRun <= 0 || currentPixel >= screenTotalPixels) {
			return;
		}
		for(int i = 0; i < pixelsToRun; i++) {
			if(currentXPos == 0) {
				spritesOnScanline.clear();
				// Fill the vector with numbers from 0 to 127
				for (int i = 0; i <= 127; ++i) {
					spritesOnScanline.push_back(i);
				}
			}
			*pixelPointer = evaluatePixel(currentXPos,currentYPos);
			currentPixel++;
			pixelPointer++;
			currentXPos++;
			if(currentXPos >= screenWidth) {
				currentXPos = 0;
				currentYPos++;
			}
			//if(currentPixel >= screenTotalPixels) {
			//	break;
			//}
		}
	}
	
	void afterFrame() {
		if(currentPixel < screenTotalPixels - 1) {
			runFor(screenTotalPixels - currentPixel - 1);
		}
	}
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint16_t options) {
		if(addedSprites>=128) {
			return;
		}
		bus::write32(objAddr, address);
		bus::write8(objAddr+4, x);
		bus::write8(objAddr+5, y);
		bus::write8(objAddr+6, w);
		bus::write8(objAddr+7, h);
		bus::write16(objAddr+8, options);
		addedSprites++;
		objAddr+=10;
	}
	
	uint32_t* getBuffer() {
		return frameBuffer;
	}

	void drawText(uint32_t fontOrigin, uint32_t textOrigin, uint16_t x, uint16_t y) {
		auto letterOffset = textOrigin;
		auto xOffset = x;
		do {
			char letter = bus::read8(letterOffset);
			if(letter == 0) {
				break;
			}
			auto letterAddress = fontOrigin + (letter-32)*128;
			if(letter<32 || letter>127) {
				
			} else {
				drawSprite(letterAddress, xOffset, y, 8, 8, 0x0000);
			}
			letterOffset++;
			xOffset+=8;
		} while(xOffset<screenWidth);
	}
	
}