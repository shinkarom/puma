#include "input.hpp"

#include "common.hpp"
#include "bus.hpp"

#include <iostream>

namespace input {
	bool pressedInputs[numInputs];
	bool justPressedInputs[numInputs];
	bool justReleasedInputs[numInputs];
	bool previouslyPressedInputs[numInputs];
	
	void init() {
		for (int i = 0; i< numInputs; i++) {
			previouslyPressedInputs[i] = false;
		}
	}
	
	bool isPressed(int keynum) {
		if(keynum > numInputs) {
			return false;
		}
		bool result = pressedInputs[keynum];
		return result;
	}
	
	bool isJustPressed(int keynum) {
		if(keynum > numInputs) {
			return false;
		}
		return justPressedInputs[keynum];
	}
	
	bool isJustReleased(int keynum) {
		if(keynum > numInputs) {
			return false;
		}
		return justReleasedInputs[keynum];
	}
	
	void beforeFrame() {
		
	}
	
	void updateInput(int inputNum, bool value) {
		if(inputNum < 0 || inputNum >= numInputs) {
			return;
		}
		previouslyPressedInputs[inputNum] = pressedInputs[inputNum];
		pressedInputs[inputNum] = value;
		justPressedInputs[inputNum] = (! previouslyPressedInputs[inputNum]) && pressedInputs[inputNum];
		justReleasedInputs[inputNum] = previouslyPressedInputs[inputNum] & (! pressedInputs[inputNum]);
	}
	
}