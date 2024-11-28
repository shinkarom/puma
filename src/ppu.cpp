#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include "bus.hpp"
#include "color.hpp"

constexpr auto HFLIP_MASK = 0b1;
constexpr auto VFLIP_MASK = 0b10;
constexpr auto PRIORITY_MASK = 0b1100;

struct obj {
	uint32_t address;
	uint8_t xx, ww;
	int priority;
	uint16_t options;
};

int currentPixel, currentXPos, currentYPos;
uint32_t* pixelPointer;
int addedSprites;
int objAddr;
std::vector<obj> spritesOnScanline;
int hScroll1, vScroll1, hScroll2, vScroll2, hScroll3, vScroll3;

namespace ppu {
	
	uint32_t frameBuffer[screenTotalPixels];
	
	void init() {
		std::cout<<std::hex<<"Tileset: "<<tilesetOffset<<std::dec<<std::endl;
		std::cout<<std::hex<<"OAM: "<<oamOffset<<std::dec<<std::endl;
		std::cout<<std::hex<<"Tilemap1: "<<tilemap1Offset<<std::dec<<std::endl;
		std::cout<<std::hex<<"HScroll1: "<<hScroll1Address<<std::dec<<std::endl;
		std::cout<<std::hex<<"VScroll1: "<<vScroll1Address<<std::dec<<std::endl;
		std::cout<<std::hex<<"Tilemap2: "<<tilemap2Offset<<std::dec<<std::endl;
		std::cout<<std::hex<<"HScroll2: "<<hScroll2Address<<std::dec<<std::endl;
		std::cout<<std::hex<<"VScroll2: "<<vScroll2Address<<std::dec<<std::endl;
		std::cout<<std::hex<<"Tilemap3: "<<tilemap3Offset<<std::dec<<std::endl;
		std::cout<<std::hex<<"HScroll3: "<<hScroll3Address<<std::dec<<std::endl;
		std::cout<<std::hex<<"VScroll3: "<<vScroll3Address<<std::dec<<std::endl;
		 reset();
	}
	
	void deinit() {
		
	}
	
	void reset() {
		memset(frameBuffer,0,screenTotalPixels*sizeof(uint32_t));

		hScroll1 = 0;
		vScroll1 = 0;
		hScroll2 = 0;
		vScroll2 = 0;
		hScroll3 = 0;
		vScroll3 = 0;
	}
	
	void beforeFrame() {
		currentPixel = 0;
		currentXPos = 0;
		currentYPos = 0;
		pixelPointer = &frameBuffer[0];		
		addedSprites = 0;
		objAddr = oamOffset;
	}
	
	inline uint32_t evaluatePixel(int x, int y) {
		for(auto i : spritesOnScanline) {
			auto ww = i.ww;
			int xOffset = x-i.xx;
			if(i.priority != 0 || xOffset<0 || xOffset >= ww) {
				continue;
			}
			if(i.options & HFLIP_MASK) {
				xOffset = ww-xOffset;
			}
			auto newPixel = color::palette16bit[bus::read16(i.address+2*xOffset)];
			//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
			if((newPixel&0xFF000000)==0xFF000000) {
				return newPixel;
			}
		}
		
		auto xPos = (hScroll1 + x) % (tilemapWidth * 8);
		auto yPos = (vScroll1 + y) % (tilemapHeight * 8);
		auto hScrollMain = xPos / 8;
		auto hScrollRem = xPos % 8;
		auto vScrollMain = yPos / 8;
		auto vScrollRem = yPos % 8;
		auto tileAddr = tilemap1Offset+(vScrollMain * tilemapWidth + hScrollMain)*4;
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
		
		for(auto i : spritesOnScanline) {
			auto ww = i.ww;
			int xOffset = x-i.xx;
			if(i.priority != 1 || xOffset<0 || xOffset >= ww) {
				continue;
			}
			if(i.options & HFLIP_MASK) {
				xOffset = ww-xOffset;
			}
			auto newPixel = color::palette16bit[bus::read16(i.address+2*xOffset)];
			//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
			if((newPixel&0xFF000000)==0xFF000000) {
				return newPixel;
			}
		}
		
		xPos = (hScroll2 + x) % (tilemapWidth * 8);
		yPos = (vScroll2 + y) % (tilemapHeight * 8);
		hScrollMain = xPos / 8;
		hScrollRem = xPos % 8;
		vScrollMain = yPos / 8;
		vScrollRem = yPos % 8;
		tileAddr = tilemap2Offset+(vScrollMain * tilemapWidth + hScrollMain)*4;
		tileNum = bus::read16(tileAddr);
		
		options = bus::read16(tileAddr+2);
		if(options & VFLIP_MASK) {
			vScrollRem = 8 - vScrollRem;
		}
		if(options & HFLIP_MASK) {
			hScrollRem = 8 - hScrollRem;
		}
		tileStart = tilesetOffset + tileNum * 128;
		pixelAddr = tileStart + (8*2*vScrollRem) + (2*hScrollRem);
		pixelColor = color::palette16bit[bus::read16(pixelAddr)];
		if((pixelColor&0xFF000000)==0xFF000000) {
			return pixelColor;
		}
		
		for(auto i : spritesOnScanline) {
			auto ww = i.ww;
			int xOffset = x-i.xx;
			if(i.priority != 2 || xOffset<0 || xOffset >= ww) {
				continue;
			}
			if(i.options & HFLIP_MASK) {
				xOffset = ww-xOffset;
			}
			auto newPixel = color::palette16bit[bus::read16(i.address+2*xOffset)];
			//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
			if((newPixel&0xFF000000)==0xFF000000) {
				return newPixel;
			}
		}
		
		xPos = (hScroll3 + x) % (tilemapWidth * 8);
		yPos = (vScroll3 + y) % (tilemapHeight * 8);
		hScrollMain = xPos / 8;
		hScrollRem = xPos % 8;
		vScrollMain = yPos / 8;
		vScrollRem = yPos % 8;
		tileAddr = tilemap3Offset+(vScrollMain * tilemapWidth + hScrollMain)*4;
		tileNum = bus::read16(tileAddr);
		
		options = bus::read16(tileAddr+2);
		if(options & VFLIP_MASK) {
			vScrollRem = 8 - vScrollRem;
		}
		if(options & HFLIP_MASK) {
			hScrollRem = 8 - hScrollRem;
		}
		tileStart = tilesetOffset + tileNum * 128;
		pixelAddr = tileStart + (8*2*vScrollRem) + (2*hScrollRem);
		pixelColor = color::palette16bit[bus::read16(pixelAddr)];
		if((pixelColor&0xFF000000)==0xFF000000) {
			return pixelColor;
		}
		
		for(auto i : spritesOnScanline) {
			auto ww = i.ww;
			int xOffset = x-i.xx;
			if(i.priority != 3 || xOffset<0 || xOffset >= ww) {
				continue;
			}
			if(i.options & HFLIP_MASK) {
				xOffset = ww-xOffset;
			}
			auto newPixel = color::palette16bit[bus::read16(i.address+2*xOffset)];
			//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
			if((newPixel&0xFF000000)==0xFF000000) {
				return newPixel;
			}
		}
		
		return 0xFF000000;
	}
	
	void updateScanlineList() {
		spritesOnScanline.clear();
			for (int i = 0; i < numObjects; ++i) {
				auto a = oamOffset+i*objectEntrySize;
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
				if(options & VFLIP_MASK) {
					yOffset = hh-yOffset;
				}
				addr += 2*ww*yOffset;
				auto priority = (options & PRIORITY_MASK) >> 2;
				struct obj o = {.address = addr, 
					.xx = xx, .ww = ww, .priority = priority,
					.options = options};
				spritesOnScanline.push_back(o);
			}
			hScroll1 = bus::read16(hScroll1Address);
			if(hScroll1 > tilemapWidth * tileSide) {
				hScroll1 = tilemapWidth * tileSide;
			}
			vScroll1 = bus::read16(vScroll1Address);
			if(vScroll1 > tilemapHeight * tileSide) {
				vScroll1 = tilemapHeight * tileSide;
			}
			hScroll2 = bus::read16(hScroll2Address);
			if(hScroll2 > tilemapWidth * tileSide) {
				hScroll2 = tilemapWidth * tileSide;
			}
			vScroll2 = bus::read16(vScroll2Address);
			if(vScroll2 > tilemapHeight * tileSide) {
				vScroll2 = tilemapHeight * tileSide;
			}
			hScroll3 = bus::read16(hScroll3Address);
			if(hScroll3 > tilemapWidth * tileSide) {
				hScroll3 = tilemapWidth * tileSide;
			}
			vScroll3 = bus::read16(vScroll3Address);
			if(vScroll3 > tilemapHeight * tileSide) {
				vScroll3 = tilemapHeight * tileSide;
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

	void drawText(uint16_t fontOrigin, uint32_t textOrigin, uint16_t x, uint16_t y) {
		auto letterOffset = textOrigin;
		auto xOffset = x;
		do {
			char letter = bus::read8(letterOffset);
			if(letter == 0) {
				break;
			}
			auto letterAddress = tilesetOffset + (fontOrigin * tileSizeBytes) + (letter-32)*tileSizeBytes;
			if(letter<32 || letter>127) {
				
			} else {
				drawSprite(letterAddress, xOffset, y, 8, 8, 0x0000);
			}
			letterOffset++;
			xOffset+=8;
		} while(xOffset<screenWidth);
	}
	
}