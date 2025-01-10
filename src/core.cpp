#include "core.hpp"

#include <iostream>
#include <string>

#include "common.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"

namespace core {
	
	std::string originalFileName;
	
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
			apu::frame();
			apu::afterFrame();
			
			
			return true;
		}
		apu::clearBuffer();
		return false;
	}
	
	void deinit() {
		apu::deinit();
		ppu::deinit();
		cpu::deinit();
	}
	
	bool tryLoadFile(std::string fileName) {
		if(!bus::load(fileName.c_str())) {
			originalFileName = "";
			return false;
		} else {
			originalFileName = fileName;
			core::reset();
			return true;
		}
	}
	
	bool tryReloadFile() {
		if(originalFileName == "") {
			return false;
		} else {
			return tryLoadFile(originalFileName);
		}
	}
	
	void unloadFile() {
		isRunning = false;
		isFileLoaded = false;
		originalFileName = "";
		bus::unload();
		ppu::reset();
	}
	
	void updateInput(int inputNum, bool value) {
		input::updateInput(inputNum, value);
	}
	
}
