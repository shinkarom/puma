#include <print>

#include "common.hpp"
#include "render.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"

int main(int argc, char *argv[])
{
	if(argc == 1) {
		std::println("No input file provided.");
		return 0;
	}
	
	if(!bus::load(argv[1])) {
		std::println("File couldn't be loaded.");
		return 0;
	}
	
	render::init();
	
	bool shouldContinue;
	do {
		shouldContinue = render::frame();
	} while(shouldContinue);
	
	render::deinit();
	return 0;
}