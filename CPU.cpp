#include "CPU.h"

void CPU::countDown()
{
	delayTimer = (delayTimer > 0) ? delayTimer - 1 : 0;
	soundTimer = (soundTimer > 0) ? soundTimer - 1 : 0;
}

uint16_t CPU::fetchOpcode()
{
	return (memory[pc] << 8) | memory[pc + 1];
}

DecodedInstr CPU::decodeInstr(uint16_t opcode)
{
	return DecodedInstr{ 
		static_cast<uint8_t>((opcode & 0xF000) >> 12),
		static_cast<uint8_t>((opcode & 0x0F00) >> 8),
		static_cast<uint8_t>((opcode & 0x00F0) >> 4),
		static_cast<uint8_t>(opcode & 0x000F),
		static_cast<uint8_t>(opcode & 0x00FF),
		static_cast<uint16_t>(opcode & 0x0FFF)
	};
}

void CPU::executeInstr(const DecodedInstr& instr)
{

	switch (instr.n1) {
	case 0x0:
		if (instr.nn == 0xE0) {
			// 00E0 Clear the display
			OP_00E0();
		}
		else if (instr.nn == 0xEE) {
			// 0x00EE Return from subroutine
			OP_00EE();
		}
		break;
	case 0x1:
		// 1NNN: pc = NNN
		OP_1NNN(instr.nnn);
		break;
	case 0x2:
		// 2NNN: Call subroutine
		OP_2NNN(instr.nnn);
		break;
	case 0x3:
		// 3XNN: Skip next instruction if VX == NN
		OP_3XNN(instr.x, instr.nn);
		break;
	case 0x4:
		// 4XNN: Skip next instruction if VX != NN
		OP_4XNN(instr.x, instr.nn);
		break;
	case 0x5:
		// 5XY0: Skip next instruction if VX == VY
		OP_5XY0(instr.x, instr.y);
		break;
	case 0x6:
		// 6XNN: Set VX to NN
		OP_6XNN(instr.x, instr.nn);
		break;
	case 0x7:
		// 7XNN: VX += NN (carry flag is not changed)
		OP_7XNN(instr.x, instr.nn);
		break;
	case 0x8:
		switch (instr.n4) {
		case 0x0:
			// 8XY0: VX = VY
			OP_8XY0(instr.x, instr.y);
			break;
		case 0x1:
			// 8XY1:  VX = VX OR VY
			OP_8XY1(instr.x, instr.y);
			break;
		case 0x2:
			// 8XY2: VX = VX AND VY
			OP_8XY2(instr.x, instr.y);
			break;
		case 0x3:
			// 8XY3:  VX = VX XOR VY
			OP_8XY3(instr.x, instr.y);
			break;
		case 0x4:
			// 8XY4: VX += VY. VF = 1 if there's a carry, otherwise 0
			OP_8XY4(instr.x, instr.y);
			break;
		case 0x5:
			// 8XY5: VX -= VY. VF = 0 if there's a borrow, otherwise 1
			OP_8XY5(instr.x, instr.y);
			break;
		case 0x6:
			// 8XY6: Store the least significant bit of VX in VF, then VX >>= 1
			OP_8XY6(instr.x);
			break;
		case 0x7:
			// 8XY7: VX = VY - VX. VF = 0 if there's a borrow, otherwise 1
			OP_8XY7(instr.x, instr.y);
			break;
		case 0xE:
			// 8XYE: Store the most significant bit of VX in VF, then  VX <<= 1
			OP_8XYE(instr.x);
			break;
		}
		break;
	case 0x9:
		// 9XY0: Skip next instruction if VX != VY
		OP_9XY0(instr.x, instr.y);
		break;
	case 0xA:
		// ANNN: I = NNN
		OP_ANNN(instr.nnn);
		break;
	case 0xB:
		// BNNN: Jump to the address NNN + V0
		OP_BNNN(instr.nnn);
		break;
	case 0xC:
		// CXNN: VX = a random number AND NN
		OP_CXNN(instr.x, instr.nn);
		break;
	case 0xD:
		// DXYN: Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I. VF = 1 if any pixels are flipped from set to unset when the sprite is drawn (means collision), and 0 otherwise
		OP_DXYN(instr.x, instr.y, instr.n4);
		break;
	case 0xE:
		if (instr.nn == 0x9E) {
			// EX9E: Skip next instruction if the key stored in VX is pressed
			OP_EX9E(instr.x);
		}
		else if (instr.nn == 0xA1) {
			// EXA1: Skip next instruction if the key stored in VX is not pressed
			OP_EXA1(instr.x);
		}
		break;
	case 0xF:
		switch (instr.nn) {
		case 0x07:
			// FX07: VX = delay timer
			OP_FX07(instr.x);
			break;
		case 0x0A:
			// FX0A: Wait for a key press, store the value of the key in VX
			OP_FX0A(instr.x);
			break;
		case 0x15:
			// FX15: delay timer = VX
			OP_FX15(instr.x);
			break;
		case 0x18:
			// FX18: VX = sound timer
			OP_FX18(instr.x);
			break;
		case 0x1E:
			// FX1E: I += VX. VF is not affected
			OP_FX1E(instr.x);
			break;
		case 0x29:
			// FX29: Set I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
			OP_FX29(instr.x);
			break;
		case 0x33:
			// FX33: Store the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I + 1, and the least significant digit at I + 2
			OP_FX33(instr.x);
			break;
		case 0x55:
			// FX55: Store V0 to VX in memory starting at address I. I = I + X + 1 after operation
			OP_FX55(instr.x);
			break;
		case 0x65:
			// FX65: Fill V0 to VX with values from memory starting at address I. I = I + X + 1 after operation
			OP_FX65(instr.x);
			break;
		}
		break;
	}
	// Only increment pc if the instruction didn't change it
	//if (pc == oldPc){
		pc += 2;
	//}
	
}

void CPU::test()
{
	// 6XNN / 7XNN
	memory[0x200] = 0x60; // V0 = 5
	memory[0x201] = 0x05;

	memory[0x202] = 0x61; // V1 = 2
	memory[0x203] = 0x02;

	memory[0x204] = 0x70; // V0 += 3  -> V0 = 8
	memory[0x205] = 0x03;

	// 8XY0 (V0 = V1)
	memory[0x206] = 0x80;
	memory[0x207] = 0x10;

	// 8XY1 (OR)
	memory[0x208] = 0x80;
	memory[0x209] = 0x11;

	// 8XY2 (AND)
	memory[0x20A] = 0x80;
	memory[0x20B] = 0x12;

	// 8XY3 (XOR)
	memory[0x20C] = 0x80;
	memory[0x20D] = 0x13;

	// 8XY4 (ADD + carry)
	memory[0x20E] = 0x80;
	memory[0x20F] = 0x14;

	// 8XY5 (SUB)
	memory[0x210] = 0x80;
	memory[0x211] = 0x15;

	// 1NNN (jump test)
	memory[0x212] = 0x12;
	memory[0x213] = 0x20; // jump to 0x220

	// skipped instruction (test jump)
	memory[0x214] = 0x60;
	memory[0x215] = 0xFF;

	// target jump
	memory[0x220] = 0x61;
	memory[0x221] = 0x0A;
}

void CPU::onKeyDown(uint8_t key)
{
	keypad[key] = 1;
}

void CPU::onKeyUp(uint8_t key)
{
	keypad[key] = 0;
}

void CPU::OP_00E0()
{
	for(auto & row : screen)
		row.fill({ BLACK });
}

void CPU::OP_00EE()
{
	sp--;
	pc = stack[sp];
	pc -= 2;
}

void CPU::OP_1NNN(uint16_t NNN)
{
	pc = NNN;
	pc -= 2;
}

void CPU::OP_2NNN(uint16_t NNN)
{
	stack[sp] = pc + 2;
	sp++;
	pc = NNN;
	pc -= 2;
}

void CPU::OP_3XNN(uint8_t X, uint16_t NN)
{
	if(V[X] == NN) {
		pc += 2;
	}
}

void CPU::OP_4XNN(uint8_t X, uint16_t NN)
{
	if(V[X] != NN) {
		pc += 2;
	}
}

void CPU::OP_5XY0(uint8_t X, uint8_t Y)
{
	if(V[X] == V[Y]) {
		pc += 2;
	}
}

void CPU::OP_6XNN(uint8_t X, uint8_t NN)
{
	V[X] = NN;
}

void CPU::OP_7XNN(uint8_t X, uint8_t NN)
{
	V[X] += NN;
}

void CPU::OP_8XY0(uint8_t X, uint8_t Y)
{
	V[X] = V[Y];
}

void CPU::OP_8XY1(uint8_t X, uint8_t Y)
{
	V[X] |= V[Y];
}

void CPU::OP_8XY2(uint8_t X, uint8_t Y)
{
	V[X] &= V[Y];
}

void CPU::OP_8XY3(uint8_t X, uint8_t Y)
{
	V[X] ^= V[Y];
}

void CPU::OP_8XY4(uint8_t X, uint8_t Y)
{
	uint16_t sum = V[X] + V[Y];
	// Set carry flag
	V[0xF] = (sum > 0xFF) ? 1 : 0;
	// Keep only the least significant byte
	V[X] = sum & 0xFF;
}

void CPU::OP_8XY5(uint8_t X, uint8_t Y)
{
	V[0xF] = (V[X] > V[Y]) ? 1 : 0;
	V[X] -= V[Y];
}

void CPU::OP_8XY6(uint8_t X)
{
	V[0xF] = V[X] & 0x1;
	// Dvide by 2
	V[X] >>= 1;
}

void CPU::OP_8XY7(uint8_t X, uint8_t Y)
{
	V[0xF] = (V[Y] > V[X]) ? 1 : 0;
	V[X] = V[Y] - V[X];
}

void CPU::OP_8XYE(uint8_t X)
{
	V[0xF] = (V[X] >> 7) & 0x1;
	// Multiply by 2
	V[X] <<= 1;
}

void CPU::OP_9XY0(uint8_t X, uint8_t Y)
{
	if(V[X] != V[Y]) {
		pc += 2;
	}
}

void CPU::OP_ANNN(uint16_t NNN)
{
	I = NNN;
}

void CPU::OP_BNNN(uint16_t NNN)
{
	pc = NNN + V[0];
}

void CPU::OP_CXNN(uint8_t X, uint8_t NN)
{
	uint8_t random = rand() % 256;
	V[X] = random & NN;
}

void CPU::OP_DXYN(uint8_t X, uint8_t Y, uint8_t N)
{
	// DXYN: Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I. VF = 1 if any pixels are flipped from set to unset when the sprite is drawn, and 0 otherwise
	// Wrap around the screen if necessary
	uint8_t xPos = V[X] % CHIP8_WIDTH;
	uint8_t yPos = V[Y] % CHIP8_HEIGHT;

	// Reset collision flag
	V[0xF] = 0; 

	for (uint8_t row = 0; row < N; row++) {
		uint8_t spriteByte = memory[I + row];
		// Each sprite byte represents 8 horizontal pixels
		for (uint8_t col = 0; col < 8; col++) {
			uint8_t spritePixel = (spriteByte >> (7 - col)) & 1;
			uint8_t& screenPixel = screen[(yPos + row) % CHIP8_HEIGHT][(xPos + col) % CHIP8_WIDTH];

			if (screenPixel && spritePixel) {
				// Collision detected
				V[0xF] = 1; 
			}
			screenPixel ^= spritePixel; // XOR the sprite pixel with the screen pixel
		}
	}
}

void CPU::OP_EX9E(uint8_t X)
{
	if(keypad[V[X]]) {
		pc += 2;
	}
}

void CPU::OP_EXA1(uint8_t X)
{
	if(!keypad[V[X]]) {
		pc += 2;
	}
}

void CPU::OP_FX07(uint8_t X)
{
	V[X] = delayTimer;
}

void CPU::OP_FX0A(uint8_t X)
{
	for(int i = 0; i < 16; i++) {
		if(keypad[i]) {
			V[X] = i;
			return;
		}
	}
	// Still waiting for key press
	pc -= 2; 
}

void CPU::OP_FX15(uint8_t X)
{
	delayTimer = V[X];
}

void CPU::OP_FX18(uint8_t X)
{
	soundTimer = V[X];
}

void CPU::OP_FX1E(uint8_t X)
{
	I += V[X];
}

void CPU::OP_FX29(uint8_t X)
{
	// Each character is 5 bytes long
	I = V[X] * 5;
}

void CPU::OP_FX33(uint8_t X)
{
	uint8_t value = V[X];
	memory[I] = value / 100; // Hundreds digit
	memory[I + 1] = (value / 10) % 10; // Tens digit
	memory[I + 2] = value % 10; // Ones digit
}

void CPU::OP_FX55(uint8_t X)
{
	for (uint8_t i = 0; i <= X; i++) {
		memory[I + i] = V[i];
	}
	I += X + 1;
}

void CPU::OP_FX65(uint8_t X)
{
	for (uint8_t i = 0; i <= X; i++) {
		V[i] = memory[I + i];
	}
	I += X + 1;
}
