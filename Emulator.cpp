#include "Emulator.h"
#include <fstream>
#include <iostream>

void Emulator::loadROM(const std::string& path)
{
    // Open the file as a stream of binary and move the file pointer to the end
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file)
        throw std::runtime_error("ROM open failed");

    // Tell get position 
    std::streamsize size = file.tellg();
	// Move the file pointer back to the beginning of the file
    file.seekg(0, std::ios::beg);

    if(!file.read(
        reinterpret_cast<char*>(cpu.memory.data() + START_ADDRESS),
        size
    ))
		throw std::runtime_error("ROM read failed");
	//cpu.test();

}

void Emulator::run()
{
    if (!renderer.init()) return;

    while (renderer.isRunning()) {
        renderer.tick();
        // 17 operations here to emulate about 1000 cycles per second, which is the original speed of the Chip-8
        for (int i = 0; i < 17; i++) {
            uint16_t opcode = cpu.fetchOpcode();
            DecodedInstr instr = cpu.decodeInstr(opcode);
            cpu.executeInstr(instr);
        }
        renderer.handleEvents();
        renderer.update(cpu.screen);
        // 60 fps
        renderer.waitForNextFrame();
    }

    renderer.quit();
}
