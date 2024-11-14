#pragma once

#include <cstdint>

constexpr int numInputs = 12;

constexpr auto maxScreenWidth = 256;
constexpr auto maxScreenHeight = 256;
constexpr auto maxScreenTotalPixels = maxScreenWidth * maxScreenHeight;

constexpr auto defaultScreenWidth = 224;
constexpr auto defaultScreenHeight = 224;

constexpr auto audioSampleRate = 44100;
constexpr auto framesPerSecond = 60;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;
constexpr auto cpuClockRate = 30*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;
constexpr auto totalMemory = 16*1024*1024;
constexpr auto systemMemorySize = 1 * 1024 * 1024;
constexpr auto systemMemoryStart = totalMemory - systemMemorySize;
constexpr auto maxFileSize = totalMemory - 0x200 - systemMemorySize;
constexpr auto codeOffset = 0x200;
constexpr auto stackOffset = totalMemory;
constexpr auto numApuRegisters = 0x106;

constexpr auto syscallStackAddress = 0xFFFFFFF9;
constexpr auto syscallAddress = 0xFFFFFFFC;

constexpr auto transparentColor = 0x00000000;

extern bool isPaused;

extern int screenWidth;
extern int screenHeight;
extern int screenTotalPixels;
extern bool haveDimensionsChanged;