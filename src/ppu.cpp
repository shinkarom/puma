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
std::vector<obj> spritesOnScanline[4];
int hScroll1, vScroll1, hScroll2, vScroll2, hScroll3, vScroll3;
bool hasIt[4];


constexpr bool isColorTransparent(uint32_t color) {
		return (color == 0);
}

namespace ppu {
	
	uint32_t frameBuffer[screenTotalPixels];
	
	void init() {
		std::cout<<std::hex<<"Palette: "<<paletteOffset<<std::dec<<std::endl;
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
	
	inline uint8_t evaluatePixel(int x, int y) {
		if(hasIt[0]) {
			for(auto i : spritesOnScanline[0]) {
				auto ww = i.ww;
				int xOffset = x-i.xx;
				if(xOffset<0 || xOffset >= ww) {
					continue;
				}
				if(i.options & HFLIP_MASK) {
					xOffset = ww-1-xOffset;
				}
				auto newPixel = bus::read8(i.address+xOffset);
				//std::cout<<x<<" "<<y<<" "<<std::hex<<(int)(newPixel)<<std::dec<<std::endl;
				if(!isColorTransparent(newPixel)) {
					return newPixel;
				}
			}
		}
		
		
		auto xPos = (hScroll1 + x) % (tilemapWidth * tileSide);
		auto yPos = (vScroll1 + y) % (tilemapHeight * tileSide);
		auto hScrollMain = xPos / tileSide;
		auto hScrollRem = xPos % tileSide;
		auto vScrollMain = yPos / tileSide;
		auto vScrollRem = yPos % tileSide;
		auto tileAddr = tilemap1Offset+(vScrollMain * tilemapWidth + hScrollMain)*4;
		auto tileNum = bus::read16(tileAddr);
		
		auto options = bus::read16(tileAddr+2);
		if(options & VFLIP_MASK) {
			vScrollRem = tileSide - 1 - vScrollRem;
		}
		if(options & HFLIP_MASK) {
			hScrollRem = tileSide - 1 - hScrollRem;
		}
		auto tileStart = tilesetOffset + tileNum * tileSizeBytes;
		auto pixelAddr = tileStart + (tileSide*vScrollRem) + (hScrollRem);
		auto pixelColor = bus::read8(pixelAddr);
		if(!isColorTransparent(pixelColor)) {
			return pixelColor;
		}
		
		if(hasIt[1]) {
			for(auto i : spritesOnScanline[1]) {
				auto ww = i.ww;
				int xOffset = x-i.xx;
				if(xOffset<0 || xOffset >= ww) {
					continue;
				}
				if(i.options & HFLIP_MASK) {
					xOffset = ww-1-xOffset;
				}
				auto newPixel = bus::read8(i.address+xOffset);
				//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
				if(!isColorTransparent(newPixel)) {
					return newPixel;
				}
			}
		}
		
		
		xPos = (hScroll2 + x) % (tilemapWidth * tileSide);
		yPos = (vScroll2 + y) % (tilemapHeight * tileSide);
		hScrollMain = xPos / tileSide;
		hScrollRem = xPos % tileSide;
		vScrollMain = yPos / tileSide;
		vScrollRem = yPos % tileSide;
		tileAddr = tilemap2Offset+(vScrollMain * tilemapWidth + hScrollMain)*4;
		tileNum = bus::read16(tileAddr);
		
		options = bus::read16(tileAddr+2);
		if(options & VFLIP_MASK) {
			vScrollRem = tileSide - 1 - vScrollRem;
		}
		if(options & HFLIP_MASK) {
			hScrollRem = tileSide - 1 - hScrollRem;
		}
		tileStart = tilesetOffset + tileNum * tileSizeBytes;
		pixelAddr = tileStart + (tileSide*vScrollRem) + (hScrollRem);
		pixelColor = bus::read8(pixelAddr);
		if(!isColorTransparent(pixelColor)) {
			return pixelColor;
		}
		
		if(hasIt[2]) {
			for(auto i : spritesOnScanline[2]) {
				auto ww = i.ww;
				int xOffset = x-i.xx;
				if(xOffset<0 || xOffset >= ww) {
					continue;
				}
				if(i.options & HFLIP_MASK) {
					xOffset = ww-1-xOffset;
				}
				auto newPixel = bus::read8(i.address+xOffset);
				//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
				if(!isColorTransparent(newPixel)) {
					return newPixel;
				}
			}
		}
		
		
		xPos = (hScroll3 + x) % (tilemapWidth * tileSide);
		yPos = (vScroll3 + y) % (tilemapHeight * tileSide);
		hScrollMain = xPos / tileSide;
		hScrollRem = xPos % tileSide;
		vScrollMain = yPos / tileSide;
		vScrollRem = yPos % tileSide;
		tileAddr = tilemap3Offset+(vScrollMain * tilemapWidth + hScrollMain)*4;
		tileNum = bus::read16(tileAddr);
		
		options = bus::read16(tileAddr+2);
		if(options & VFLIP_MASK) {
			vScrollRem = tileSide - 1 - vScrollRem;
		}
		if(options & HFLIP_MASK) {
			hScrollRem = tileSide - 1 - hScrollRem;
		}
		tileStart = tilesetOffset + tileNum * tileSizeBytes;
		pixelAddr = tileStart + (tileSide*vScrollRem) + (hScrollRem);
		pixelColor = bus::read8(pixelAddr);
		if(!isColorTransparent(pixelColor)) {
			return pixelColor;
		}
		
		if(hasIt[3]) {
			for(auto i : spritesOnScanline[3]) {
				auto ww = i.ww;
				int xOffset = x-i.xx;
				if(xOffset<0 || xOffset >= ww) {
					continue;
				}
				if(i.options & HFLIP_MASK) {
					xOffset = ww-1-xOffset;
				}
				auto newPixel = bus::read8(i.address+xOffset);
				//std::cout<<x<<" "<<y<<" "<<std::hex<<newPixel<<std::dec<<std::endl;
				if(!isColorTransparent(newPixel)) {
					return newPixel;
				}
			}
		}
		
		
		return 0x0;
	}
	
	void updateScanlineList() {
		hasIt[0] = false;
		hasIt[1] = false;
		hasIt[2] = false;
		hasIt[3] = false;
		for(int i = 0; i<4;i++) {
			spritesOnScanline[i].clear();
		}
		
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
				yOffset = hh-1-yOffset;
			}
			addr += ww*yOffset;
			auto priority = (options & PRIORITY_MASK) >> 2;
			hasIt[priority] = true;
			struct obj o = {.address = addr, 
				.xx = xx, .ww = ww, .priority = priority,
				.options = options};
			spritesOnScanline[priority].push_back(o);
		}
	}
	
	void updateScrollPositions() {
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
		updateScrollPositions();
		while(currentYPos < screenHeight && pixelsToRun > 0) {
			if(currentXPos == 0) {
				updateScanlineList();
			}
			auto colIndex = evaluatePixel(currentXPos, currentYPos);
			auto color = bus::read32(paletteOffset+4*colIndex);
			//std::cout<<currentXPos<<" "<<currentYPos<<" "<<std::hex<<(int)(colIndex)<<" "<<color<<std::dec<<std::endl;
			*pixelPointer = color;
			pixelsToRun--;
			pixelPointer++;
			currentXPos++;
			if(currentXPos >= screenWidth) {
				currentXPos = 0;
				currentYPos++;
			}
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