#pragma once

#include <cstdint>
#include "common.hpp"

#include <SDL3/SDL_scancode.h>

namespace input {
	
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
	};
	
	extern bool pressedInputs[numInputs];
	extern bool justPressedInputs[numInputs];
	extern bool justReleasedInputs[numInputs];
	extern bool previouslyPressedInputs[numInputs];
	
	bool isPressed(int keynum);
	bool isJustPressed(int keynum);
	bool isJustReleased(int keynum);
	
	void init();
	void beforeFrame();
}