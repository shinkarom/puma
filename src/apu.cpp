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
bool hasAudio = false;

namespace apu {
		
	void setupChannels() {
		for(int i = 0; i<numApuChannels; i++) {
			tsf_channel_set_presetindex(sf, i, 0);
			tsf_channel_set_pan(sf, i, 0.5);
			tsf_channel_set_volume(sf, i, 1.0);
		}
	}	
		
	void init() {
		hasAudio = false;
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		auto exePath = std::filesystem::path(PathFind::FindExecutable()).parent_path();
		auto sf2name = exePath / "CHAOS8M.SF2";
		sf = tsf_load_filename(sf2name.string().c_str());
		if(sf == nullptr) {
			std::cout<<"Could not load soundfont"<<std::endl;
		} else {
			hasAudio = true;
		}
		if(hasAudio) {
			tsf_set_output(sf, TSF_STEREO_INTERLEAVED, audioSampleRate);
			tsf_set_max_voices(sf, 256);
			//for(int i = 0; i<tsf_get_presetcount(sf);i++) {
			//	const char* presetname = tsf_get_presetname(sf, i);
			//	std::cout<<i<<" "<<presetname<<std::endl;
			//}
		}
		
	}
	
	void reset() {
		if(hasAudio) {
			tsf_reset(sf);
			setupChannels();
			//for(int i = 0; i<tsf_get_presetcount(sf);i++) {
			//	const char* presetname = tsf_get_presetname(sf, i);
			//	std::cout<<i<<" "<<presetname<<std::endl;
			//}
		}
	}
	
	void deinit() {
		if(hasAudio){
			tsf_close(sf);
		}
	}
	
	void afterFrame() {
		
	}
	
	void frame() {
		if(hasAudio) {
			tsf_render_short(sf, audioBuffer, samplesPerFrame);
		}
	}
	
	void writeReg(int reg, int value) {
		
		
		//std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
	
	void noteOn(int channelNum, int keyNum, int vel) {
		if(!hasAudio || channelNum < 0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_note_on(sf, channelNum, keyNum, vel/127.0);
	}
	
	void noteOff(int channelNum, int keyNum) {
		if(!hasAudio || channelNum < 0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_note_off(sf, channelNum, keyNum);
	}
	void allNotesOff(int channelNum) {
		if(!hasAudio || channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_note_off_all(sf, channelNum);
	}
	
	void allSoundsOff(int channelNum) {
		if(!hasAudio || channelNum < 0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_sounds_off_all(sf, channelNum);
	}
	
	void setChannelVolume(int channelNum, int value) {
		if(!hasAudio || channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_set_volume(sf, channelNum, value/127.0);
	}
	
	void setChannelPan(int channelNum, int value) {
		if(!hasAudio || channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_set_pan(sf, channelNum, value/255.0);
	}
	
	void setGlobalVolume(int value) {
		if(!hasAudio || value < 0 || value > 127) {
			return;
		}
		tsf_set_volume(sf, value/127.0);
	}
	
	void setChannelPreset(int channelNum, int value) {
		if(!hasAudio || channelNum <0 || channelNum >= numApuChannels) {
			return;
		}
		tsf_channel_set_presetindex(sf, channelNum, value);
	}
	
	int16_t* getBuffer() {
		return audioBuffer;
	}
	
}