#include "bus.hpp"
#include "common.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

namespace bus {
	
	//uint8_t memory[totalMemory];
	std::vector<uint8_t> memory(totalMemory,0);
	std::vector<uint8_t> originalFile(maxFileSize, 0);
	
	constexpr auto syscallStackSize = 1024;
	int syscallStackTop = 0;
	
	uint32_t syscallStack[syscallStackSize];
	
	void write8(uint32_t address, uint8_t value) {
		if(address>=totalMemory)
			return;
		memory[address] = value;
	}
	
	void write16(uint32_t address, uint16_t value) {
		if(address+1 >= totalMemory)
			return;
		memory[address] = (value>>8)&0xFF;
		memory[address+1] = (value)&0xFF;
	}
	
	void write32(uint32_t address, uint32_t value) {
		 if(address+3 >= totalMemory)
			return;
		memory[address] =  (value>>24)&0xFF;
		memory[address+1] =  (value>>16)&0xFF;
		memory[address+2] =  (value>>8)&0xFF;
		memory[address+3] =  (value)&0xFF;
	}
	
	uint8_t read8(uint32_t address) {
		if(address>totalMemory)
			return 0;
		return memory[address];
	}
	
	uint16_t read16(uint32_t address) {
		if(address>totalMemory)
			return 0;
		auto r = (memory[address]<<8)|(memory[address+1]);
		return r;
	}
	
	uint32_t read32(uint32_t address) {
		if(address>totalMemory)
			return 0;
		auto r = (memory[address]<<24)|(memory[address+1]<<16)|(memory[address+2]<<8)|(memory[address+3]);
		return r;
	}
	
	void push8(uint8_t value) {
		if(syscallStackTop < syscallStackSize) {
			syscallStack[syscallStackTop++] = value;
		}
	}
	
	void push16(uint16_t value) {
		if(syscallStackTop < syscallStackSize) {
			syscallStack[syscallStackTop++] = value;
		}
	}
	
	void push32(uint32_t value) {
		if(syscallStackTop < syscallStackSize) {
			syscallStack[syscallStackTop++] = value;
		}
	}
	
	uint8_t pop8() {
		if(syscallStackTop < 0) {
			return 0;
		} else {
			return syscallStack[--syscallStackTop] & 0xFF;
		}
	}
	
	uint16_t pop16() {
		if(syscallStackTop < 0) {
			return 0;
		} else {
			return syscallStack[--syscallStackTop] & 0xFFFF;
		}
	}
	
	uint32_t pop32() {
		if(syscallStackTop < 0) {
			return 0;
		} else {
			return syscallStack[--syscallStackTop];
		}
	}
	
	bool load(char* fileName) {
		std::ifstream file(fileName, std::ios::binary);
		if(!file.is_open()){
			return false;
		}
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		
		if(fileSize > maxFileSize) {
			return false;
		}
		originalFile.resize(fileSize);
		file.read(reinterpret_cast<char*>(&originalFile[0]), fileSize);
		
		std::cout<<"Read file of size "<<fileSize<<std::endl;
		
		return true;
	}
	
	void reset() {
		std::fill(memory.begin(), memory.end(), 0);
		std::copy(originalFile.begin(), originalFile.end(), memory.begin()+codeOffset);
	}
}