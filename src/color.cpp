#include "color.hpp"

#include <iostream>

namespace color {
	
	uint32_t palette16bit[65536];
	uint32_t palette8bit[256];
	uint32_t palette4bit[64];
	 
	void init() {
		for(int i = 0; i < 65536; i++) {
			 palette16bit[i] = convert16to32color(i);
		 }
		 for(int i = 0; i < 256; i++) {
			 palette8bit[i] = convert8to32color(i);
		 }
		 for(int i = 0; i < 16; i++) {
			 palette4bit[i] = convert4to32color(i);
		 }
	}
	
	/// Convert 32-bit ARGB8888 to 16-bit ARGB4444
	constexpr uint16_t convert32to16color(uint32_t color8888) {
		uint8_t alpha = (color8888 >> 28) & 0xF; // Keep top 4 bits of Alpha
		uint8_t red = (color8888 >> 20) & 0xF;   // Keep top 4 bits of Red
		uint8_t green = (color8888 >> 12) & 0xF; // Keep top 4 bits of Green
		uint8_t blue = (color8888 >> 4) & 0xF;   // Keep top 4 bits of Blue
		return (alpha << 12) | (red << 8) | (green << 4) | blue;
	}

	/// Convert 32-bit ARGB8888 to 8-bit ARGB2222
	constexpr uint8_t convert32to8color(uint32_t color8888) {
		uint8_t alpha = (color8888 >> 30) & 0x3; // Keep top 2 bits of Alpha
		uint8_t red = (color8888 >> 22) & 0x3;   // Keep top 2 bits of Red
		uint8_t green = (color8888 >> 14) & 0x3; // Keep top 2 bits of Green
		uint8_t blue = (color8888 >> 6) & 0x3;   // Keep top 2 bits of Blue
		return (alpha << 6) | (red << 4) | (green << 2) | blue;
	}

	/// Convert 16-bit ARGB4444 to 32-bit ARGB8888
	constexpr uint32_t convert16to32color(uint16_t color4444) {
		uint8_t alpha = (color4444 >> 12) & 0xF;
		uint8_t red = (color4444 >> 8) & 0xF;
		uint8_t green = (color4444 >> 4) & 0xF;
		uint8_t blue = color4444 & 0xF;

		// Expand 4-bit values to 8-bit by scaling
		alpha = (alpha << 4) | alpha;
		red = (red << 4) | red;
		green = (green << 4) | green;
		blue = (blue << 4) | blue;

		return (alpha << 24) | (red << 16) | (green << 8) | blue;
	}

	/// Convert 8-bit ARGB2222 to 32-bit ARGB8888
	constexpr uint32_t convert8to32color(uint8_t color2222) {
		uint8_t alpha = (color2222 >> 6) & 0x3;
		uint8_t red = (color2222 >> 4) & 0x3;
		uint8_t green = (color2222 >> 2) & 0x3;
		uint8_t blue = color2222 & 0x3;

		// Expand 2-bit values to 8-bit by scaling
		alpha = (alpha << 6) | (alpha << 4) | (alpha << 2) | alpha;
		red = (red << 6) | (red << 4) | (red << 2) | red;
		green = (green << 6) | (green << 4) | (green << 2) | green;
		blue = (blue << 6) | (blue << 4) | (blue << 2) | blue;

		return (alpha << 24) | (red << 16) | (green << 8) | blue;
	}

	/// Convert 16-bit ARGB4444 to 8-bit ARGB2222
	constexpr uint8_t convert16to8color(uint16_t color4444) {
		uint8_t alpha = (color4444 >> 12) & 0x3; // Keep top 2 bits of Alpha
		uint8_t red = (color4444 >> 8) & 0x3;    // Keep top 2 bits of Red
		uint8_t green = (color4444 >> 4) & 0x3;  // Keep top 2 bits of Green
		uint8_t blue = color4444 & 0x3;          // Keep top 2 bits of Blue
		return (alpha << 6) | (red << 4) | (green << 2) | blue;
	}

	/// Convert 8-bit ARGB2222 to 16-bit ARGB4444
	constexpr uint16_t convert8to16color(uint8_t color2222) {
		uint8_t alpha = (color2222 >> 6) & 0x3;
		uint8_t red = (color2222 >> 4) & 0x3;
		uint8_t green = (color2222 >> 2) & 0x3;
		uint8_t blue = color2222 & 0x3;

		// Expand 2-bit values to 4-bit by scaling
		alpha = (alpha << 2) | alpha;
		red = (red << 2) | red;
		green = (green << 2) | green;
		blue = (blue << 2) | blue;

		return (alpha << 12) | (red << 8) | (green << 4) | blue;
	}
	
	/// Convert 32-bit ARGB8888 to 4-bit ARGB1111
	constexpr uint8_t convert32to4color(uint32_t color8888) {
		uint8_t alpha = (color8888 >> 31) & 0x1; // Keep top 1 bit of Alpha
		uint8_t red = (color8888 >> 23) & 0x1;   // Keep top 1 bit of Red
		uint8_t green = (color8888 >> 15) & 0x1; // Keep top 1 bit of Green
		uint8_t blue = (color8888 >> 7) & 0x1;   // Keep top 1 bit of Blue
		return (alpha << 3) | (red << 2) | (green << 1) | blue;
	}
	
	/// Convert 16-bit ARGB4444 to 4-bit ARGB1111
	constexpr uint8_t convert16to4color(uint16_t color4444) {
		uint8_t alpha = (color4444 >> 12) & 0x1; // Keep top 1 bit of Alpha
		uint8_t red = (color4444 >> 8) & 0x1;    // Keep top 1 bit of Red
		uint8_t green = (color4444 >> 4) & 0x1;  // Keep top 1 bit of Green
		uint8_t blue = color4444 & 0x1;          // Keep top 1 bit of Blue
		return (alpha << 3) | (red << 2) | (green << 1) | blue;
	}
	
	/// Convert 8-bit ARGB2222 to 4-bit ARGB1111
	constexpr uint8_t convert8to4color(uint8_t color2222) {
		uint8_t alpha = (color2222 >> 6) & 0x1; // Keep top 1 bit of Alpha
		uint8_t red = (color2222 >> 4) & 0x1;   // Keep top 1 bit of Red
		uint8_t green = (color2222 >> 2) & 0x1; // Keep top 1 bit of Green
		uint8_t blue = color2222 & 0x1;         // Keep top 1 bit of Blue
		return (alpha << 3) | (red << 2) | (green << 1) | blue;
	}

	/// Convert 4-bit ARGB1111 to 32-bit ARGB8888
	constexpr uint32_t convert4to32color(uint8_t color1111) {
		uint8_t alpha = (color1111 >> 3) & 0x1;
		uint8_t red = (color1111 >> 2) & 0x1;
		uint8_t green = (color1111 >> 1) & 0x1;
		uint8_t blue = color1111 & 0x1;

		// Expand 1-bit values to 8-bit by full on/off scaling
		alpha = alpha ? 0xFF : 0x00;
		red = red ? 0xFF : 0x00;
		green = green ? 0xFF : 0x00;
		blue = blue ? 0xFF : 0x00;

		return (alpha << 24) | (red << 16) | (green << 8) | blue;
	}

	/// Convert 4-bit ARGB1111 to 16-bit ARGB4444
	constexpr uint16_t convert4to16color(uint8_t color1111) {
		uint8_t alpha = (color1111 >> 3) & 0x1;
		uint8_t red = (color1111 >> 2) & 0x1;
		uint8_t green = (color1111 >> 1) & 0x1;
		uint8_t blue = color1111 & 0x1;

		// Expand 1-bit values to 4-bit by full on/off scaling
		alpha = alpha ? 0xF : 0x0;
		red = red ? 0xF : 0x0;
		green = green ? 0xF : 0x0;
		blue = blue ? 0xF : 0x0;

		return (alpha << 12) | (red << 8) | (green << 4) | blue;
	}

	/// Convert 4-bit ARGB1111 to 8-bit ARGB2222
	constexpr uint8_t convert4to8color(uint8_t color1111) {
		uint8_t alpha = (color1111 >> 3) & 0x1;
		uint8_t red = (color1111 >> 2) & 0x1;
		uint8_t green = (color1111 >> 1) & 0x1;
		uint8_t blue = color1111 & 0x1;

		// Expand 1-bit values to 2-bit by full on/off scaling
		alpha = alpha ? 0x3 : 0x0;
		red = red ? 0x3 : 0x0;
		green = green ? 0x3 : 0x0;
		blue = blue ? 0x3 : 0x0;

		return (alpha << 6) | (red << 4) | (green << 2) | blue;
	}
	
	uint32_t blendARGB(uint32_t src, uint32_t dst) {
		uint8_t srcA = (src >> 24) & 0xFF;
		uint8_t srcR = (src >> 16) & 0xFF;
		uint8_t srcG = (src >> 8) & 0xFF;
		uint8_t srcB = src & 0xFF;

		uint8_t dstA = (dst >> 24) & 0xFF;
		uint8_t dstR = (dst >> 16) & 0xFF;
		uint8_t dstG = (dst >> 8) & 0xFF;
		uint8_t dstB = dst & 0xFF;

		uint8_t outA = srcA + dstA * (255 - srcA) / 255;
		uint8_t outR = (srcR * srcA + dstR * dstA * (255 - srcA) / 255) / outA;
		uint8_t outG = (srcG * srcA + dstG * dstA * (255 - srcA) / 255) / outA;
		uint8_t outB = (srcB * srcA + dstB * dstA * (255 - srcA) / 255) / outA;

		return (outA << 24) | (outR << 16) | (outG << 8) | outB;
	}
	
}
