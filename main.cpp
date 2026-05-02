#include "Emulator.h"
#include <memory>
#include <iostream>

int main(int argc, char** argv) {
    
    std::string romPath = "ufo.rom";

    std::cout << "Please enter rom path or press enter to load default (" << romPath << "): " << std::endl;

    std::string input;
    std::getline(std::cin, input);

    if (!input.empty()) {
        romPath = input;
    }

    try {
        Emulator emulator;
        emulator.loadROM(romPath);
        emulator.run();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}