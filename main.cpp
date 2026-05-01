#include "Emulator.h"
#include <memory>
#include <iostream>

int main(int argc, char** argv) {
    
    try {
        Emulator emulator;
        emulator.loadROM("IBM Logo.ch8");
        emulator.run();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}