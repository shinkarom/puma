#pragma once

#include <cstdint>
#include "common.hpp"

namespace input {
	
	constexpr int inputMapping[numInputs] = {
		4,	//RETRO_DEVICE_ID_JOYPAD_UP
		5,	//RETRO_DEVICE_ID_JOYPAD_DOWN
		6, 	//RETRO_DEVICE_ID_JOYPAD_LEFT
		7,	//RETRO_DEVICE_ID_JOYPAD_RIGHT
		2,	//RETRO_DEVICE_ID_JOYPAD_SELECT
		3,	//RETRO_DEVICE_ID_JOYPAD_START
		8,	//RETRO_DEVICE_ID_JOYPAD_A
		0,	//RETRO_DEVICE_ID_JOYPAD_B
		9,	//RETRO_DEVICE_ID_JOYPAD_X
		1,	//RETRO_DEVICE_ID_JOYPAD_Y
		10,	//RETRO_DEVICE_ID_JOYPAD_L
		11,	//RETRO_DEVICE_ID_JOYPAD_R
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