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

void Emulator::initFont()
{
    for (int i = 0; i < FONT_ARR_SIZE; i++) {
        cpu.memory[i] = fontset[i];
	}
}

void Emulator::run()
{
    if (!renderer.init()) return;
	renderer.initKeyMap();
    renderer.evtKeyDown = [&](uint8_t key) {
        cpu.onKeyDown(key); };
    renderer.evtKeyUp = [&](uint8_t key) {
        cpu.onKeyUp(key); };

	audio.init();
	initFont();

    while (renderer.isRunning()) {
        renderer.tick();
        
        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            uint16_t opcode = cpu.fetchOpcode();
            DecodedInstr instr = cpu.decodeInstr(opcode);
            cpu.executeInstr(instr);
        }
        renderer.handleEvents();
        renderer.update(cpu.screen);
        // 60 fps
        renderer.waitForNextFrame();
		cpu.countDown();
		audio.update(cpu.soundTimer > 0);
    }

    renderer.quit();
}
