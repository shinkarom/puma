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

struct obj {
	uint32_t address;
	uint8_t xx, ww;
	uint16_t options;
};

int currentPixel, currentXPos, currentYPos;
uint32_t* pixelPointer;
int addedSprites;
int objAddr;
std::vector<obj> spritesOnScanline;
int hScroll, vScroll;

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
		std::cout<<std::hex<<"Tileset: "<<tilesetOffset<<std::dec<<std::endl;
		std::cout<<std::hex<<"OAM: "<<oamOffset<<std::dec<<std::endl;
		std::cout<<std::hex<<"Tilemap: "<<tilemapOffset<<std::dec<<std::endl;
		std::cout<<std::hex<<"HScroll: "<<hScrollAddress<<std::dec<<std::endl;
		std::cout<<std::hex<<"VScroll: "<<vScrollAddress<<std::dec<<std::endl;
		 reset();
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer,0,screenTotalPixels*sizeof(uint32_t));
		memset(secondBuffer,0,screenTotalPixels*sizeof(uint32_t));
		hScroll = 0;
		vScroll = 0;
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
		for(auto i : spritesOnScanline) {
			auto addr = i.address;
			auto xx = i.xx;
			auto ww = i.ww;
			auto options = i.options;
			int xOffset = x-xx;
			if(xOffset<0 || xOffset >= ww) {
				continue;
			}
			if(options && HFLIP_MASK) {
				xOffset = ww-xOffset;
			}
			auto newPixel = color::palette16bit[bus::read16(addr+2*xOffset)];
			//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
			if((newPixel&0xFF000000)==0xFF000000) {
				return newPixel;
			}
		}
		auto xPos = (hScroll + x) % (tilemapWidth * 8);
		auto yPos = (vScroll + y) % (tilemapHeight * 8);
		auto hScrollMain = xPos / 8;
		auto hScrollRem = xPos % 8;
		auto vScrollMain = yPos / 8;
		auto vScrollRem = yPos % 8;
		auto tileAddr = tilemapOffset+(vScrollMain * tilemapWidth + hScrollMain)*4;
		auto tileNum = bus::read16(tileAddr);
		
		auto options = bus::read16(tileAddr+2);
		if(options & VFLIP_MASK) {
			vScrollRem = 8 - vScrollRem;
		}
		if(options & HFLIP_MASK) {
			hScrollRem = 8 - hScrollRem;
		}
		auto tileStart = tilesetOffset + tileNum * 128;
		auto pixelAddr = tileStart + (8*2*vScrollRem) + (2*hScrollRem);
		auto pixelColor = color::palette16bit[bus::read16(pixelAddr)];
		if((pixelColor&0xFF000000)==0xFF000000) {
				return pixelColor;
			}
		
		return 0xFF000000;
	}
	
	void updateScanlineList() {
		spritesOnScanline.clear();
			for (int i = 0; i < numObjects; ++i) {
				auto a = oamOffset+i*10;
				auto addr = bus::read32(a);
				if(addr == 0) {
					break;
				}
				auto xx = bus::read8(a+4);
				auto yy = bus::read8(a+5);
				auto ww = bus::read8(a+6);
				auto hh = bus::read8(a+7);
				auto yOffset = currentYPos - yy;
				if(yOffset <0 || yOffset >= hh) {
					continue;
				}
				auto options = bus::read16(a+8);
				if(options && VFLIP_MASK) {
					yOffset = hh-yOffset;
				}
				addr += 2*ww*yOffset;
				struct obj o = {.address = addr, 
					.xx = xx, .ww = ww, 
					.options = options};
				spritesOnScanline.push_back(o);
			}
	}
	
	void runFor(int pixelsToRun) {
		if(pixelsToRun <= 0 || currentPixel >= screenTotalPixels) {
			return;
		}
		for(int i = 0; i < pixelsToRun; i++) {
			if(currentXPos == 0) {
				updateScanlineList();
			}
			*pixelPointer = evaluatePixel(currentXPos, currentYPos);
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