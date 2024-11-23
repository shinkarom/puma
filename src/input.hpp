#pragma once

#include <cstdint>
#include "common.hpp"

namespace input {
	
	extern bool pressedInputs[numInputs];
	extern bool justPressedInputs[numInputs];
	extern bool justReleasedInputs[numInputs];
	extern bool previouslyPressedInputs[numInputs];
	
	bool isPressed(int keynum);
	bool isJustPressed(int keynum);
	bool isJustReleased(int keynum);
	
	void init();
	void beforeFrame();
	
	void updateInput(int inputNum, bool value);
}