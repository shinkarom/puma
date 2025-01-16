#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>
#include <random>
#include <algorithm>

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

enum {
	API_printRegisters = 0,
	API_writeAudioRegister,
	API_endFrame,
	API_isPressed,
	API_isJustPressed,
	API_isJustReleased,
	API_drawSprite,
	API_drawText,
	API_getRandomNumber,
	API_printStack,
	API_setClearColorIndex,
	API_setTransparentIndex,
	API_setPaletteColor,
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
			auto keynum = bus::pop8();
			auto res = input::isPressed(keynum);
			if(res) {
				bus::push8(1);
			} else {
				bus::push8(0);
			}
			break;
		}
		case API_isJustPressed: {
			auto keynum = bus::pop8();
			if(input::isJustPressed(keynum)) {
				bus::push8(1);
			} else {
				bus::push8(0);
			}
			break;
		}
		case API_isJustReleased: {
			auto keynum = bus::pop8();
			if(input::isJustReleased(keynum)) {
				bus::push8(1);
			} else {
				bus::push8(0);
			}
			break;
		}
		case API_drawSprite: {
			auto options = bus::pop8();
			auto h = bus::pop16();
			auto w = bus::pop16();
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto address = bus::pop32();
			ppu::drawSprite(address, x, y, w, h, options);
			break;
		}
		case API_drawText: {
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto textOrigin = bus::pop32();
			auto fontOrigin = bus::pop16();
			ppu::drawText(fontOrigin, textOrigin, x, y);
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
		case API_setClearColorIndex: {
			auto index = color::palette8bit[bus::pop8()];
			ppu::setClearColor(index);
			break;
		}
		case API_setTransparentIndex: {
			auto index = bus::pop8();
			ppu::setTransparentIndex(index);
			break;
		}
		case API_setPaletteColor: {
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
	}
	
	void reset() {
		bus::write32(0, stackOffset);
		bus::write32(0x4, codeOffset);
		m68k_pulse_reset();
	}
	
}