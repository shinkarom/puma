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
int noteVelocities[numApuChannels];
int notePresets[numApuChannels];
int noteValues[numApuChannels];
uint32_t soundTimers[numApuChannels];
int currentChannel;

namespace apu {
		
	void setupChannels() {
		for(int i = 0; i<numApuChannels; i++) {
			tsf_channel_set_presetindex(sf, i, 0);
			tsf_channel_set_pan(sf, i, 0.5);
			tsf_channel_set_volume(sf, i, 1.0);
			noteVelocities[i] = 127;
			notePresets[i] = 0;
			noteValues[i] = 60;
			soundTimers[i] = 0;
		}
		currentChannel = 0;
	}	
		
	void init() {
		hasAudio = false;
		clearBuffer();
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
			clearBuffer();
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
		auto chan = reg & 0x0F;
		switch(reg & 0xF0) {
			case 0x00: {
				auto v = value > 127 ? value - 127: value;
				tsf_channel_note_off_all(sf, chan);
				if(v != 0) {
					tsf_channel_note_on(sf, chan, v, noteVelocities[chan]/127.0);
				}
				break;
			}
			case 0x10:{
				auto v = value & 0xFF;
				notePresets[chan] = (notePresets[chan]&0xFF00)| (v);
				tsf_channel_set_presetindex(sf, chan, notePresets[chan]);
				break;
			}
			case 0x20: {
				auto v = value & 0xFF;
				notePresets[chan] = (notePresets[chan]&0x00FF)| (v<<8);
				tsf_channel_set_presetindex(sf, chan, notePresets[chan]);
				break;
			}
			case 0x30: {
				auto v = value & 0xFF;
				noteVelocities[chan] = v;
				break;
			}
			case 0x40: {
				tsf_channel_sounds_off_all(sf, chan);
				break;
			}
			default:
				break;
		}			
		
		//std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
	
	int16_t* getBuffer() {
		return audioBuffer;
	}
	
	void clearBuffer() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
	}
	
}