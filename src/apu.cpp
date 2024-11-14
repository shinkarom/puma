#include "apu.hpp"
#include <cstring>
#include <iostream>

#include "common.hpp"
#include "bus.hpp"
#include "ymfm_opl.h"

int16_t audioBuffer[samplesPerFrame*2];

class Chip: public ymfm::ymfm_interface {
	public:
		Chip(): _chip(*this) {
			_chip.reset();
		}
		
		void generate(int16_t* output, uint32_t numsamples = 1) {	
			uint32_t ratio = (static_cast<uint64_t>(49700) << 16) / audioSampleRate;
			uint32_t inputIndexFixed = 0;  // Fixed-point input index
			
			ymfm::ym2413::output_data previousFrame, currentFrame;
			_chip.generate(&previousFrame, 1);
			
			for(int i = 0; i<numsamples; i++) {
				// Calculate integer and fractional parts of the fixed-point index
				int16_t frac = inputIndexFixed & 0xFFFF;
				
				// Generate the next sample only when the integer part advances
				if ((inputIndexFixed >> 16) != 0) {
					previousFrame = currentFrame;
					_chip.generate(&currentFrame, 1);
					inputIndexFixed &= 0xFFFF;  // Keep only the fractional part
				}
				
				// Interpolate between previousSample and currentSample for each channel
				for (int channel = 0; channel < 2; ++channel) {
					int16_t sample1 = previousFrame.data[channel];
					int16_t sample2 = currentFrame.data[channel];
					
					// Perform fixed-point linear interpolation
					int32_t interpolatedSample = (sample1 * (0x10000 - frac) + sample2 * frac) >> 16;
					*output++ = static_cast<int16_t>(interpolatedSample); // Write and increment pointer
				}
				
				// Increment the input index by the fixed-point ratio
				inputIndexFixed += ratio;
			}
		}
		
		void writeReg(int reg, uint8_t value) {
			_chip.write_address(reg);
			_chip.write_data(value);
		}
		
	private:
		ymfm::ym2413 _chip;
};

Chip chip;

namespace apu {
		
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {
		
	}
	
	int16_t* callback() {
		chip.generate(audioBuffer, samplesPerFrame);
		return audioBuffer;
	}
	
	void writeReg(int reg, int value) {
		
		chip.writeReg(reg, value);
		
			std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
	
}