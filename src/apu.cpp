#include "apu.hpp"
#include <cstring>
#include <iostream>
#include <filesystem>

#include "common.hpp"
#include "bus.hpp"
#include "tsf.h"
#include "pathfind.hpp"
 
int16_t audioBuffer[samplesPerFrame*2];
tsf* sf;

namespace apu {
		
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		auto exePath = std::filesystem::path(PathFind::FindExecutable()).parent_path();
		auto sf2name = exePath / "CHAOS8M.SF2";
		sf = tsf_load_filename(sf2name.string().c_str());
		if(sf == nullptr) {
			std::cout<<"Could not load soundfont"<<std::endl;
		} else {
			tsf_set_output(sf, TSF_STEREO_INTERLEAVED, audioSampleRate);
			tsf_set_max_voices(sf, 64);
		}
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {
		
	}
	
	int16_t* callback() {
		tsf_render_short(sf, audioBuffer, samplesPerFrame);
		return audioBuffer;
	}
	
	void writeReg(int reg, int value) {
		
		//chip.writeReg(reg, value);
		
		//std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
	
	void noteOn(int presetNum, int keyNum, int vel) {
		tsf_note_on(sf, presetNum, keyNum, vel/127);
	}
	
	void noteOff(int presetNum, int keyNum) {
		tsf_note_off(sf, presetNum, keyNum);
	}
	
}