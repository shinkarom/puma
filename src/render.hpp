#pragma once
#include <cstdint>
#include <string>

#include <SDL3/SDL_scancode.h>
#include "common.hpp"

namespace render {
	constexpr int inputMapping[numInputs] = {
		SDL_SCANCODE_UP,	//UP
		SDL_SCANCODE_DOWN,	//DOWN
		SDL_SCANCODE_LEFT, 	//LEFT
		SDL_SCANCODE_RIGHT,	//RIGHT
		SDL_SCANCODE_RSHIFT,	//SELECT
		SDL_SCANCODE_RETURN,	//START
		SDL_SCANCODE_Z,	//A
		SDL_SCANCODE_X,	//B
		SDL_SCANCODE_A,	//X
		SDL_SCANCODE_S,	//Y
		SDL_SCANCODE_Q,	//L
		SDL_SCANCODE_W,	//R
		
		// ZXCVBNM,ASDFGHJKQWERTYUI12345678
	};
	
	void init();
	bool frame(uint32_t* frameBuffer, int16_t* audioBuffer);
	void renderAudio(int16_t* audioBuffer);
	void updateInput();
	void deinit();
	bool tryLoadFile(std::string fileName);
}