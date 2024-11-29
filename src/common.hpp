#pragma once

#include <cstdint>

constexpr int numInputs = 12;

constexpr auto screenWidth = 224;
constexpr auto screenHeight = 224;
constexpr auto screenTotalPixels = screenWidth * screenHeight;

constexpr auto framesPerSecond = 60;

constexpr auto audioSampleRate = 44100;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;

constexpr auto cpuClockRate = 30*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;

constexpr auto tileSide = 8;
constexpr auto tileSizeBytes = tileSide * tileSide * 1;
constexpr auto numTiles = 65536;
constexpr auto tilesetSizeBytes = numTiles * tileSizeBytes;

constexpr auto totalMemory = 64*1024*1024;
constexpr auto systemMemorySize = 9 * 1024 * 1024;
constexpr auto systemMemoryStart = totalMemory - systemMemorySize;
constexpr auto maxFileSize = totalMemory - 0x200 - systemMemorySize;

constexpr auto codeOffset = 0x200;
constexpr auto stackOffset = totalMemory;

constexpr auto numTilemapScreens = 2;
constexpr auto tilemapWidth = screenWidth / tileSide * numTilemapScreens;
constexpr auto tilemapHeight = screenHeight / tileSide * numTilemapScreens;
constexpr auto tilemapSize = tilemapWidth * tilemapHeight * 4;

constexpr auto numObjects = 128;
constexpr auto objectEntrySize = 10;
constexpr auto oamSize = numObjects * objectEntrySize;

constexpr auto paletteOffset = systemMemoryStart;
constexpr auto numPaletteColors = 256;
constexpr auto paletteSize = numPaletteColors * 4;

constexpr auto tilesetOffset = paletteOffset + paletteSize;
constexpr auto oamOffset = tilesetOffset + tilesetSizeBytes;

constexpr auto tilemap1Offset = oamOffset + oamSize;
constexpr auto hScroll1Address = tilemap1Offset + tilemapSize;
constexpr auto vScroll1Address = hScroll1Address + 2;

constexpr auto tilemap2Offset = vScroll1Address + 2;
constexpr auto hScroll2Address = tilemap2Offset + tilemapSize;
constexpr auto vScroll2Address = hScroll2Address + 2;

constexpr auto tilemap3Offset = vScroll2Address + 2;
constexpr auto hScroll3Address = tilemap3Offset + tilemapSize;
constexpr auto vScroll3Address = hScroll3Address + 2;

constexpr auto numApuChannels = 16;

constexpr auto syscallStackAddress = 0xFFFFFFF9;
constexpr auto syscallAddress = 0xFFFFFFFC;

constexpr auto transparentColor = 0x00000000;

extern bool isRunning;
extern bool isFileLoaded;