#pragma once

constexpr int CHIP8_WIDTH = 64;
constexpr int CHIP8_HEIGHT = 32;

constexpr int SCALE = 10;
constexpr int SCREEN_WIDTH = CHIP8_WIDTH * SCALE;
constexpr int SCREEN_HEIGHT = CHIP8_HEIGHT * SCALE;

constexpr uint8_t BLACK = 0;
constexpr uint8_t WHITE = 1;

// 17 operations here to emulate about 1000 cycles per second, which is the original speed of the Chip-8
constexpr int CYCLES_PER_FRAME = 17;
constexpr int FPS = 60;
constexpr int FRAME_DELAY = 1000 / FPS;

constexpr size_t MEMORY_SIZE = 4096;
constexpr uint16_t START_ADDRESS = 0x200;