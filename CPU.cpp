#include "CPU.h"

void CPU::countDown()
{
	delayTimer = (delayTimer > 0) ? delayTimer-- : 0;
	soundTimer = (soundTimer > 0) ? soundTimer-- : 0;
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
	bool bshouldJump = true;
	switch (instr.n1) {
		case 0x0:
			if (instr.nn == 0xE0) {
				// 00E0 Clear the display
				OP_00E0();
			} else if (instr.nn == 0xEE) {
				// 0x00EE Return from subroutine
				OP_00EE();
				bshouldJump = false;
			}
			break;
		case 0x1:
			// 1NNN: pc = NNN
			OP_1NNN(instr.nnn);
			bshouldJump = false;
			break;
		case 0x2:
			// 2NNN: Call subroutine
			break;
		case 0x3:
			// 3XNN: Skip next instruction if VX == NN
			break;
		case 0x4:
			// 4XNN: Skip next instruction if VX != NN
			break;
		case 0x5:
			// 5XY0: Skip next instruction if VX == VY
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
				break;
			case 0x1:
				// 8XY1:  VX = VX OR VY
				break;
			case 0x2:
				// 8XY2: VX = VX AND VY
				break;
			case 0x3:
				// 8XY3:  VX = VX XOR VY
				break;
			case 0x4:
				// 8XY4: VX += VY. VF = 1 if there's a carry, otherwise 0
				break;
			case 0x5:
				// 8XY5: VX -= VY. VF = 0 if there's a borrow, otherwise 1
				break;
			case 0x6:
				// 8XY6: Store the least significant bit of VX in VF, then VX >>= 1
				break;
			case 0x7:
				// 8XY7: Set VX to VY minus VX. VF = 0 if there's a borrow, otherwise 1
				break;
			case 0xE:
				// 8XYE: Store the most significant bit of VX in VF, then  VX <<= 1
				break;
			}
			break;
		case 0x9:
			// 9XY0: Skip next instruction if VX != VY
			break;
		case 0xA:
			// ANNN: I = NNN
			OP_ANNN(instr.nnn);
			break;
		case 0xB:
			// BNNN: Jump to the address NNN + V0
			break;
		case 0xC:
			// CXNN: VX = a random number AND NN
			break;
		case 0xD:
			// DXYN: Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I. VF = 1 if any pixels are flipped from set to unset when the sprite is drawn (means collision), and 0 otherwise
			OP_DXYN(instr.x, instr.y, instr.n4);
			break;
		case 0xE:
			if(instr.nn == 0x9E) {
				// EX9E: Skip next instruction if the key stored in VX is pressed
			} else if (instr.nn == 0xA1) {
				// EXA1: Skip next instruction if the key stored in VX is not pressed
			}
			break;
		case 0xF:
			switch (instr.nn) {
				case 0x07:
					// FX07: VX = delay timer
					break;
				case 0x0A:
					// FX0A: Wait for a key press, store the value of the key in VX
					break;
				case 0x15:
					// FX15: delay timer = VX
					break;
				case 0x18:
					// FX18: VX = sound timer
					break;
				case 0x1E:
					// FX1E: I += VX. VF is not affected
					break;
				case 0x29:
					// FX29: Set I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
					break;
				case 0x33:
					// FX33: Store the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I + 1, and the least significant digit at I + 2
					break;
				case 0x55:
					// FX55: Store V0 to VX in memory starting at address I. I = I + X + 1 after operation
					break;
				case 0x65:
					// FX65: Fill V0 to VX with values from memory starting at address I. I = I + X + 1 after operation
					break;
			}
			break;
	}
	if (bshouldJump) {
		// Move to the next instruction
		pc += 2; 
	}
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
}

void CPU::OP_1NNN(uint16_t NNN)
{
	pc = NNN;
}

void CPU::OP_6XNN(uint8_t X, uint8_t NN)
{
	V[X] = NN;
}

void CPU::OP_7XNN(uint8_t X, uint8_t NN)
{
	V[X] += NN;
}

void CPU::OP_ANNN(uint16_t NNN)
{
	I = NNN;
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
