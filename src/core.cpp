#include "core.hpp"

#include "common.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"

namespace core {
	void init() {
		input::init();
		apu::init();
		color::init();
		ppu::init();
		cpu::init();
	}
	
	void reset() {
		if(!isFileLoaded) {
			return;
		}
		bus::reset();
		cpu::reset();
		ppu::reset();
		apu::reset();	
		isRunning = true;
	}
	
	bool frame(uint32_t** videoBuffer, int16_t** audioBuffer) {
		*audioBuffer = apu::getBuffer();
		*videoBuffer = ppu::getBuffer();
		if(isFileLoaded && isRunning) {
			// input::beforeFrame();
		
			ppu::beforeFrame();
			
			cpu::frame();
			ppu::afterFrame();
			 apu::afterFrame();
			 apu::frame();
			return true;
		}
		return false;
	}
	
	void deinit() {
		apu::deinit();
		ppu::deinit();
		cpu::deinit();
	}
	
	bool tryLoadFile(std::string fileName) {
		if(!bus::load(fileName.c_str())) {
			return false;
		} else {
			core::reset();
			return true;
		}
	}
	
}
