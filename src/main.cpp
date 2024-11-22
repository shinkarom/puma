#include <print>
#include <string>

#include "common.hpp"
#include "render.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"
#include "cpu.hpp"
#include "args.hxx"

int main(int argc, char *argv[])
{
	args::ArgumentParser parser("Puma");
	args::Positional<std::string> filename(parser, "filename", "The file to run Puma with");
	
	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help&) {
		std::cout<<parser;
	} catch (args::ParseError& e) {
		std::cerr<<e.what()<<std::endl;
		std::cerr<<parser;
		return 1;
	}
	
	if(!filename) {
		std::println("No input file provided.");
		return 1;
	}
	
	input::init();
    apu::init();
    color::init();
    ppu::init();
    cpu::init();
	render::init();
	
	if(!bus::load(argv[1])) {
		std::println("File couldn't be loaded.");
		return 0;
	}
	render::reset();	
	
	bool shouldContinue;
	do {
		if(isFileLoaded && isRunning) {
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