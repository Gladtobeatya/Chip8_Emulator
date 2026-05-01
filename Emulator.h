#pragma once
#include "CPU.h"
#include "Renderer.h"
#include <string>

class Emulator
{
private:
	CPU cpu;
	Renderer renderer;

public:
	void loadROM(const std::string& path);
	void run();
};

