#include <print>

#include "common.hpp"
#include "render.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"
#include "cpu.hpp"

int main(int argc, char *argv[])
{
	if(argc == 1) {
		std::println("No input file provided.");
		return 0;
	}
	
	input::init();
    apu::init();
    color::init();
    ppu::init();
    cpu::init();
	
	if(!bus::load(argv[1])) {
		std::println("File couldn't be loaded.");
		return 0;
	}
	cpu::onLoad();
	ppu::afterLoad();
	render::init();
	
	bool shouldContinue;
	do {
		if(!isPaused) {
			// input::beforeFrame();
		
			ppu::beforeFrame();
			
			cpu::frame();
			ppu::afterFrame();
			 apu::afterFrame();
			int16_t* audioBuffer = apu::callback();
			
			render::renderAudio(audioBuffer);
		}
	  
		shouldContinue = render::frame(ppu::getBuffer());
	} while(shouldContinue);
	
	render::deinit();
	apu::deinit();
    ppu::deinit();
    cpu::deinit();
	return 0;
}