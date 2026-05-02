#pragma once
#include <array>
#include "Chip8Config.h"

struct DecodedInstr
{
	uint8_t n1; // First 4 bits after the opcode
	uint8_t x; // Second 4 bits after the opcode
	uint8_t y; // Third 4 bits after the opcode
	uint8_t n4; // Last 4 bits of the opcode
	uint8_t nn; // Last 8 bits of the opcode
	uint16_t nnn; // Last 12 bits of the opcode
};

class CPU
{
private:
	//Grp 0x0
	void OP_00E0(); // CLS
	void OP_00EE(); // RET

	// Grp 0x1 - 0x7
	void OP_1NNN(uint16_t); // JP addr
	void OP_2NNN(uint16_t); // CALL addr
	void OP_3XNN(uint8_t, uint16_t); // SE Vx, byte
	void OP_4XNN(uint8_t, uint16_t); // SNE Vx, byte
	void OP_5XY0(uint8_t, uint8_t); // SE Vx, Vy
	void OP_6XNN(uint8_t, uint8_t); // LD Vx, byte
	void OP_7XNN(uint8_t, uint8_t); // ADD Vx, byte

	// Grp 0x8
	void OP_8XY0(uint8_t, uint8_t); // LD Vx, Vy
	void OP_8XY1(uint8_t, uint8_t); // OR Vx, Vy
	void OP_8XY2(uint8_t, uint8_t); // AND Vx, Vy
	void OP_8XY3(uint8_t, uint8_t); // XOR Vx, Vy
	void OP_8XY4(uint8_t, uint8_t); // ADD Vx, Vy
	void OP_8XY5(uint8_t, uint8_t); // SUB Vx, Vy
	void OP_8XY6(uint8_t); // SHR Vx
	void OP_8XY7(uint8_t, uint8_t); // SUBN Vx, Vy
	void OP_8XYE(uint8_t); // SHL Vx

	// Grp 0x9 - 0xD
	void OP_9XY0(uint8_t, uint8_t); // SNE Vx, Vy
	void OP_ANNN(uint16_t); // LD I, addr
	void OP_BNNN(uint16_t); // JP V0, addr
	void OP_CXNN(uint8_t, uint8_t); // RND Vx, byte
	void OP_DXYN(uint8_t, uint8_t, uint8_t); // DRW Vx, Vy, nibble

	// Grp 0xE (input)
	void OP_EX9E(uint8_t); // SKP Vx
	void OP_EXA1(uint8_t); // SKNP Vx

	// Grp 0xF
	void OP_FX07(uint8_t); // LD Vx, DT
	void OP_FX0A(uint8_t); // LD Vx, K
	void OP_FX15(uint8_t); // LD DT, Vx
	void OP_FX18(uint8_t); // LD ST, Vx
	void OP_FX1E(uint8_t); // ADD I, Vx
	void OP_FX29(uint8_t); // LD F, Vx
	void OP_FX33(uint8_t); // LD B, Vx
	void OP_FX55(uint8_t); // LD [I], Vx
	void OP_FX65(uint8_t); // LD Vx, [I]
public:
	std::array<uint8_t, MEMORY_SIZE> memory{};
	std::array<uint8_t, 16> V{}; // V0 - VF registers
	uint16_t I{}; // Index register
	uint16_t pc{ START_ADDRESS }; // Program counter
	std::array<uint16_t, 16> stack{}; // Stack for subroutine calls
	uint8_t sp{}; // Stack pointer
	std::array<uint8_t, 16> keypad{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	std::array<std::array<uint8_t, CHIP8_WIDTH>, CHIP8_HEIGHT> screen{ BLACK };

	void countDown();
	uint16_t fetchOpcode();
	DecodedInstr decodeInstr(uint16_t opcode); // Copy faster than ref here
	void executeInstr(const DecodedInstr& instr);
	void test();
};

