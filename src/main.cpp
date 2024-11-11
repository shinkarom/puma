#include <print>

#include "common.hpp"
#include "render.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"
#include "color.hpp"
#include "cpu.hpp"

void update_input(void)
{
	uint16_t state;
	
	for (int i = 0; i< numInputs; i++) {
		input::previouslyPressedInputs[i] = input::pressedInputs[i];
		//state = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, input::inputMapping[i]);
		state = 0;
		input::pressedInputs[i] = (state != 0);
		input::justPressedInputs[i] = (! input::previouslyPressedInputs[i]) && input::pressedInputs[i];
		input::justReleasedInputs[i] = input::previouslyPressedInputs[i] & (! input::pressedInputs[i]);
	}
}

void audio_callback(void)
{
   int16_t* buf = apu::callback();
   /*
   for(int i = 0; i<samplesPerFrame;i++){
		std::cout<<buf[i*2]<<" "<<buf[i*2+1]<<std::endl;
	}
	std::cout<<"---"<<std::endl;
	*/
	//audio_batch_cb(buf, samplesPerFrame);
   
}

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
		update_input();
	   input::beforeFrame();
		
		ppu::beforeFrame();
		
		cpu::frame();
		ppu::afterFrame();
		 apu::afterFrame();
		audio_callback();
		
		shouldContinue = render::frame(ppu::getBuffer());
	} while(shouldContinue);
	
	render::deinit();
	apu::deinit();
    ppu::deinit();
    cpu::deinit();
	return 0;
}