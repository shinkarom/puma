#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>
#include <random>

#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"

std::default_random_engine gen;

//#define DEBUG 

void printRegisters() {
	std::cout<<"---"<<std::hex<<std::endl;
	uint32_t t;
	t = m68k_get_reg(nullptr, M68K_REG_PC);
	std::cout<<t<<std::endl;;
	t = m68k_get_reg(nullptr, M68K_REG_D0);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D1);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D2);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D3);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D4);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D5);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D6);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_D7);
	std::cout<<t<<std::endl;
	t = m68k_get_reg(nullptr, M68K_REG_A0);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A1);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A2);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A3);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A4);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A5);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A6);
	std::cout<<t<<" ";
	t = m68k_get_reg(nullptr, M68K_REG_A7);
	std::cout<<t<<std::endl;
	std::cout<<std::dec<<"---"<<std::endl;
}

void instr_callback(unsigned int pc) {
	//std::cout<<std::hex<<"pc="<<pc<<std::dec<<std::endl;
	printRegisters();
}

uint32_t popColor() {
	return color::palette16bit[bus::pop16()];
}

enum {
	API_printRegisters = 0,
	API_writeAudioRegister,
	API_endFrame,
	API_isPressed,
	API_isJustPressed,
	API_isJustReleased,
	API_set1bitPalette,
	API_set2bitPalette,
	API_cls,
	API_setPixel,
	API_drawSprite,
	API_drawLine,
	API_drawCircle,
	API_drawCircleOutline,
	API_drawEllipse,
	API_drawEllipseOutline,
	API_drawRectangle, 
	API_drawRectangleOutline,
	API_drawTriangle,
	API_drawTriangleOutline,
	API_drawText,
	API_getRandomNumber,
	API_printStack,
	API_note_on,
	API_note_off,
	API_allNotesOff,
	API_allSoundsOff,
	API_setGlobalVolume,
	API_setChannelVolume,
	API_setChannelPan,
};

void syscall_handler(int value) {
	//std::cout<<"Syscall "<<value<<" triggered."<<std::endl;
	switch(value) {
		case API_printRegisters: {
			printRegisters();
			break;
		}
		case API_writeAudioRegister: {
			auto value = bus::pop16();
			auto reg = bus::pop16();
			apu::writeReg(reg, value);
			break;
		}
		case API_endFrame: {
			//std::cout<<"ending frame at "<<(m68k_cycles_run())<<std::endl;
			m68k_end_timeslice();
			break;
		}
		case API_isPressed: {
			auto keynum = bus::pop16();
			//if(input::isPressed(keynum)) {
			//	bus::push16(1);
			//} else {
			//	bus::push16(0);
			//}
			bus::push16(input::isPressed(keynum));
			break;
		}
		case API_isJustPressed: {
			auto keynum = bus::pop16();
			if(input::isJustPressed(keynum)) {
				bus::push16(1);
			} else {
				bus::push16(0);
			}
			break;
		}
		case API_isJustReleased: {
			auto keynum = bus::pop16();
			if(input::isJustReleased(keynum)) {
				bus::push16(1);
			} else {
				bus::push16(0);
			}
			break;
		}
		case API_cls: {
			auto color = popColor();
			ppu::clear(color);
			break;
		}
		case API_drawSprite: {
			auto options = bus::pop16();
			auto h = bus::pop16();
			auto w = bus::pop16();
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto address = bus::pop32();
			ppu::drawSprite(address, x, y, w, h, options);
			break;
		}
		case API_set1bitPalette: {
			auto color = popColor();
			ppu::set1bitPalette(color);
			break;
		}
		case API_set2bitPalette: {
			auto color2 = popColor();
			auto color1 = popColor();
			ppu::set2bitPalette(color1, color2);
			break;
		}
		case API_drawLine: {
			auto color = popColor();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawLine(x1, y1, x2, y2, color);
			break;
		}
		case API_drawCircle: {
			auto color = popColor();
			auto radius = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawCircleFilled(x0, y0, radius, color);
			break;
		}
		case API_drawCircleOutline: {
			auto color = popColor();
			auto radius = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawCircleOutline(x0, y0, radius, color);
			break;
		}
		case API_drawEllipse: {
			auto color = popColor();
			auto b = bus::pop16();
			auto a = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawEllipseFilled(x0, y0, a, b, color);
			break;
		}
		case API_drawEllipseOutline: {
			auto color = popColor();
			auto b = bus::pop16();
			auto a = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawEllipseOutline(x0, y0, a, b, color);
			break;
		}
		case API_drawRectangle: {
			auto color = popColor();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawRectangleFilled(x1, y1, x2, y2, color);
			break;
		}
		case API_drawRectangleOutline: {
			auto color = popColor();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawRectangleOutline(x1, y1, x2, y2, color);
			break;
		}
		case API_drawTriangle: {
			auto color = popColor();
			auto y3 = bus::pop16();
			auto x3 = bus::pop16();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawTriangleFilled(x1, y1, x2, y2, x3, y3, color);
			break;
		}
		case API_drawTriangleOutline: {
			auto color = popColor();
			auto y3 = bus::pop16();
			auto x3 = bus::pop16();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawTriangleOutline(x1, y1, x2, y2, x3, y3, color);
			break;
		}
		case API_drawText: {
			// font_origin, text_origin, x, y, color
			auto color = popColor();
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto textOrigin = bus::pop32();
			auto fontHeight = bus::pop16();
			auto fontWidth = bus::pop16();
			auto fontOrigin = bus::pop32();
			ppu::drawText(fontOrigin, fontWidth, fontHeight, textOrigin, x, y, color);
			break;
		}
		case API_getRandomNumber: {
			auto mmax = bus::pop32();
			auto mmin = bus::pop32();
			std::uniform_int_distribution<uint32_t> dist(mmin, mmax);
			auto r = dist(gen);
			//std::cout<<mmin<<" "<<mmax<<" "<<r<<std::endl;
			bus::push32(r);
			break;
		}
		case API_printStack: {
			std::cout<<"---"<<std::hex<<std::endl;
			auto t = m68k_get_reg(nullptr, M68K_REG_SP);
			for(int i = 0; i<10; i++) {
				auto r = bus::read16(t+2*i);
				std::cout<<r<<" ";
			}
			std::cout<<std::endl;
			for(int i = 0; i<5; i++) {
				auto r = bus::read32(t+4*i);
				std::cout<<r<<" ";
			}
			std::cout<<std::endl;
			std::cout<<"sp="<<t<<" "<<std::dec<<"---"<<std::endl;
			break;
		}
		case API_note_on: {
			int vel = bus::pop8();
			int keyNum = bus::pop8();
			int channelNum = bus::pop8();
			apu::noteOn(channelNum, keyNum, vel);
			break;
		}
		case API_note_off: {
			int keyNum = bus::pop8();
			int channelNum = bus::pop8();
			apu::noteOff(channelNum, keyNum);
			break;	
		}
		case API_allNotesOff: {
			int channelNum = bus::pop8();
			apu::allNotesOff(channelNum);
			break;
		}
		case API_allSoundsOff: {
			int channelNum = bus::pop8();
			apu::allSoundsOff(channelNum);
			break;
		}
		case API_setGlobalVolume: {
			int value = bus::pop8();
			apu::setGlobalVolume(value);
			break;
		}
		case API_setChannelVolume: {
			int value = bus::pop8();
			int channelNum = bus::pop8();
			apu::setChannelVolume(channelNum, value);
			break;
		}
		case API_setChannelPan: {
			int value = bus::pop8();
			int channelNum = bus::pop8();
			apu::setChannelPan(channelNum, value);
			break;
		}
		default:
			break;
	}
}

unsigned int  m68k_read_memory_8(unsigned int address) {
	auto r = bus::read8(address);
	if(address == syscallStackAddress) {
		return bus::pop8();
	}
	//std::cout<<"Read 8bit from "<<address<<": "<<r<<std::endl;
	return r;
}

unsigned int  m68k_read_memory_16(unsigned int address) {
	auto r = bus::read16(address);
	if(address == syscallStackAddress) {
		return bus::pop16();
	}
	//std::cout<<"Read 16bit from "<<address<<": "<<r<<std::endl;
	return r;
}

unsigned int  m68k_read_memory_32(unsigned int address) {
	auto r = bus::read32(address);
	if(address == syscallStackAddress) {
		return bus::pop32();
	}
	//std::cout<<"Read 32bit from "<<address<<": "<<r<<std::endl;
	return r;
}

void m68k_write_memory_8(unsigned int address, unsigned int value) {
	bus::write8(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	} else if(address == syscallStackAddress) {
		bus::push8(value);
	}
}

void m68k_write_memory_16(unsigned int address, unsigned int value) {
	bus::write16(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	} else if(address == syscallStackAddress) {
		bus::push16(value);
	}
}

void m68k_write_memory_32(unsigned int address, unsigned int value) {
	bus::write32(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	} else if(address == syscallStackAddress) {
		bus::push32(value);
	}
}

namespace cpu {
	void init() {
		m68k_set_cpu_type(M68K_CPU_TYPE_68040);
		m68k_init();
		std::random_device rd;
		gen = std::default_random_engine(rd());
		#ifdef DEBUG
		m68k_set_instr_hook_callback(&instr_callback);
		#endif
	}
	
	void deinit() {
		
	}
	
	void frame() {
		m68k_execute(cyclesPerFrame);
		m68k_set_irq(2);
	}
	
	void onLoad() {
		bus::write32(0, stackOffset);
		bus::write32(0x4, codeOffset);
		m68k_pulse_reset();
	}
	
}