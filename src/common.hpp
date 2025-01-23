#pragma once

#include <cstdint>

constexpr int numInputs = 32;

constexpr auto screenWidth = 192;
constexpr auto screenHeight = 192;
constexpr auto screenTotalPixels = screenWidth * screenHeight;
constexpr auto drawnPixelQuota = screenTotalPixels * 15;

constexpr auto framesPerSecond = 60;

constexpr auto audioSampleRate = 44100;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;

constexpr auto cpuClockRate = 15*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;

constexpr auto tileSide = 8;
constexpr auto tileSizeBytes = tileSide * tileSide * 2;

constexpr auto totalMemory = 16*1024*1024;
constexpr auto maxFileSize = totalMemory - 0x200;

constexpr auto codeOffset = 0x200;
constexpr auto stackOffset = totalMemory;

constexpr auto numApuChannels = 16;

constexpr auto syscallStackAddress = 0xFFFFFFF9;
constexpr auto syscallAddress = 0xFFFFFFFC;

constexpr auto defaultTransparentIndex = 0;
constexpr auto defaultClearColor = 0xFF000000;

extern bool isRunning;
extern bool isFileLoaded;