#pragma once

#include <cstdint>

constexpr int numInputs = 32;

constexpr auto screenWidth = 192;
constexpr auto screenHeight = 192;
constexpr auto screenTotalPixels = screenWidth * screenHeight;
constexpr auto drawnPixelQuota = screenTotalPixels * 10;

constexpr auto framesPerSecond = 60;

constexpr auto audioSampleRate = 44100;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;

constexpr auto cpuClockRate = 40*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;

constexpr auto tileSide = 8;
constexpr auto tileSizeBytes = tileSide * tileSide * 2;
constexpr auto numTiles = 65536;
constexpr auto tilesetSizeBytes = numTiles * tileSizeBytes;

constexpr auto totalMemory = 64*1024*1024;
constexpr auto systemMemorySize = 9 * 1024 * 1024;
constexpr auto systemMemoryStart = totalMemory - systemMemorySize;
constexpr auto maxFileSize = totalMemory - 0x200 - systemMemorySize;

constexpr auto codeOffset = 0x200;
constexpr auto stackOffset = totalMemory;

constexpr auto numApuChannels = 16;

constexpr auto syscallStackAddress = 0xFFFFFFF9;
constexpr auto syscallAddress = 0xFFFFFFFC;

constexpr auto transparentColor = 0x00000000;

extern bool isRunning;
extern bool isFileLoaded;