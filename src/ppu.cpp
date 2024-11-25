#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include "bus.hpp"
#include "color.hpp"

constexpr auto HFLIP_MASK = 1<<0;
constexpr auto VFLIP_MASK = 1<<1;

namespace ppu {
	
	static uint32_t *frame_buf;
	
	uint32_t palette1bit[2] = {transparentColor, 0xFFFFFFFF};
	
	int bitOffset;
	
	void inline setFullPixel(int x, int y, uint32_t color) {
		if(x < 0 || x >= screenWidth || y<0 || y >= screenHeight || ((color&0xFF000000) == 0x00000000)) {
			return;
		}
		frame_buf[y*screenWidth+x] = color;
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<std::hex<<color<<std::dec<<std::endl;
	}
	
	void init() {
		 frame_buf = new uint32_t[screenTotalPixels]; 
		 reset();
	}
	
	void deinit() {
		delete[] frame_buf;
	}
	
	void reset() {
		memset(frame_buf,0,screenTotalPixels*sizeof(uint32_t));
	}
	
	void beforeFrame() {
			
	}
	
	void afterFrame() {

	}
	
	void clear(uint32_t color) {
		/*
		for(int i = 0; i<screenTotalPixels; i++) {
			frame_buf[i] = (color|0xFF000000);
		}	
		*/
		drawRectangleFilled(0, 0, screenWidth-1, screenHeight-1, color);
	}
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint16_t options) {
		//std::cout<<std::hex<<"Will draw sprite from "<<address<<std::dec<<std::endl;
		bitOffset = 0;
		auto x_start = x;
		auto x_delta = 1;
		auto x_end = x+w;
		auto y_start = y;
		auto y_delta = 1;
		auto y_end = y+h;
		//std::cout<<x_start<<" "<<x_delta<<" "<<x_end<<" "<<y_start<<" "<<y_delta<<" "<<y_end<<" "<<std::endl;
		if(options & HFLIP_MASK) {
			//std::cout<<"HFLIP ";
			
			auto z = x_start;
			x_start = x_end-1;
			x_end = z;
			x_delta = -1;
		}
		if(options & VFLIP_MASK) {
			//std::cout<<"VFLIP ";
			
			auto z = y_start;
			y_start = y_end-1;
			y_end = z;
			y_delta = -1;
		}
		
		//std::cout<<std::endl;

		//std::cout<<x_start<<" "<<x_delta<<" "<<x_end<<" "<<y_start<<" "<<y_delta<<" "<<y_end<<" "<<std::endl;
		auto pxa = address;
		auto xx = x_start;
		auto yy = y_start;
		uint32_t color;
		for(int _i = 0; _i < w * h; _i++) {
			color = color::palette16bit[bus::read16(pxa)];
			pxa += 2;
			//std::cout<<xx<<" "<<yy<<" "<<std::hex<<color<<std::dec<<std::endl;
			setFullPixel(xx, yy, color);
			xx+=x_delta;
			bool lineEnd = (x_delta>0) ? (xx >= x_end) : (xx < x_end);
			if(lineEnd) {
				xx = x_start;
				yy += y_delta;
			}
			
		}
		//std::cout<<"---"<<std::endl;
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
	
	void set1bitPalette(uint32_t color) {
		palette1bit[1] = color;
	}
	
	void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
		//std::cout<<"Draw line from "<<x1<<" "<<y1<<" to "<<x2<<" "<<y2<<" with "<<std::hex<<color<<std::dec<<std::endl;
		int dx = std::abs(x2 - x1);
		int dy = std::abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;
		int err = dx - dy;

		while (true) {
			setFullPixel(x1, y1, color);  // Draw the pixel

			// If the start point reaches the end point, stop drawing
			if (x1 == x2 && y1 == y2) break;

			int e2 = 2 * err;
			
			if (e2 > -dy) {
				err -= dy;
				x1 += sx;
			}
			
			if (e2 < dx) {
				err += dx;
				y1 += sy;
			}
		}
	}
	
	void drawCircleOutline(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color) {
		int x = radius;
		int y = 0;
		int err = 0;

		while (x >= y) {
			setFullPixel(x0 - x, y0 - y, color);
			setFullPixel(x0 + x, y0 - y, color);
			setFullPixel(x0 - x, y0 + y, color);
			setFullPixel(x0 + x, y0 + y, color);
			setFullPixel(x0 - y, y0 - x, color);
			setFullPixel(x0 + y, y0 - x, color);
			setFullPixel(x0 - y, y0 + x, color);
			setFullPixel(x0 + y, y0 + x, color);

			y += 1;
			if (err <= 0) {
				err += 2*y + 1;
			} else {
				x -= 1;
				//err -= 2*x + 1;
				err += 2*(y-x) + 1;
			}
		}
	}

	void drawCircleFilled(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color) {
		int x = radius;
		int y = 0;
		int err = 0;

		while (x >= y) {
			for (int i = x0 - x; i <= x0 + x; i++) {
				setFullPixel(i, y0 - y, color);
				setFullPixel(i, y0 + y, color);
			}
			for (int i = x0 - y; i <= x0 + y; i++) {
				setFullPixel(i, y0 - x, color);
				setFullPixel(i, y0 + x, color);
			}

			y++;
			if (err <= 0) {
				err += 2 * y + 1;
			} else {
				x--;
				err += 2 * (y - x) + 1;
			}
		}
	}

	void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
		//std::cout<<"Will draw rectangle outline"<<std::endl;
		for (uint16_t x = x1; x <= x2; x++) {
			setFullPixel(x, y1, color);
			setFullPixel(x, y2, color);
		}
		for (uint16_t y = y1; y <= y2; y++) {
			setFullPixel(x1, y, color);
			setFullPixel(x2, y, color);
		}
	}

	void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
		for (uint16_t y = y1; y <= y2; y++) {
			for (uint16_t x = x1; x <= x2; x++) {
				setFullPixel(x, y, color);
			}
		}
	}

	void drawTriangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color) {
		drawLine(x1, y1, x2, y2, color);
		drawLine(x2, y2, x3, y3, color);
		drawLine(x3, y3, x1, y1, color);
	}

	void drawTriangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color) {
		auto swap = [](uint16_t &a, uint16_t &b) { uint16_t temp = a; a = b; b = temp; };
		
		// Sort the vertices by y-coordinate
		if (y1 > y2) { swap(x1, x2); swap(y1, y2); }
		if (y1 > y3) { swap(x1, x3); swap(y1, y3); }
		if (y2 > y3) { swap(x2, x3); swap(y2, y3); }

		auto interpolate = [](int y, int x1, int y1, int x2, int y2) -> int {
			if (y1 == y2) return x1;
			return x1 + (y - y1) * (x2 - x1) / (y2 - y1);
		};

		for (uint16_t y = y1; y <= y3; y++) {
			uint16_t xStart = (y < y2) ? interpolate(y, x1, y1, x2, y2) : interpolate(y, x2, y2, x3, y3);
			uint16_t xEnd = interpolate(y, x1, y1, x3, y3);

			if (xStart > xEnd) swap(xStart, xEnd);

			for (uint16_t x = xStart; x <= xEnd; x++) {
				setFullPixel(x, y, color);
			}
		}
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