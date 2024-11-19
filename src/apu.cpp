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
		
	void setupChannels() {
		for(int i = 0; i<numApuChannels; i++) {
			tsf_channel_set_presetindex(sf, i, 0);
			tsf_channel_set_pan(sf, i, 0.5);
			tsf_channel_set_volume(sf, i, 1.0);
			tsf_channel_set_pitchwheel(sf, i, 8192);
			tsf_channel_set_pitchrange(sf, i, 2.0);
			tsf_channel_set_tuning(sf, i, 0.0);
		}
	}	
		
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		auto exePath = std::filesystem::path(PathFind::FindExecutable()).parent_path();
		auto sf2name = exePath / "EMU.SF2";
		sf = tsf_load_filename(sf2name.string().c_str());
		if(sf == nullptr) {
			std::cout<<"Could not load soundfont"<<std::endl;
		} else {
			tsf_set_output(sf, TSF_STEREO_INTERLEAVED, audioSampleRate);
			tsf_set_max_voices(sf, 64);
		}
		setupChannels();
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
	
	void noteOn(int channelNum, int keyNum, int vel) {
		if(channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_note_on(sf, channelNum, keyNum, vel/127);
	}
	
	void noteOff(int channelNum, int keyNum) {
		if(channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_note_off(sf, channelNum, keyNum);
	}
	void allNotesOff(int channelNum) {
		if(channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_note_off_all(sf, channelNum);
	}
	
	void allSoundsOff(int channelNum) {
		if(channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_sounds_off_all(sf, channelNum);
	}
	
}