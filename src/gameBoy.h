#ifndef EMULATOR_GAMEBOY_H
#define EMULATOR_GAMEBOY_H

#include "cpu.h"
#include "ppu.h"
#include "interrupts.h"
#include "display.h"
#include "timer.h"
#include "joypad.h"
#include <string>
#include <chrono>


class GameBoy {
public:
    GameBoy(const std::string& filepath);
    void run();

private:
    void setupSequence(const std::string& filepath);
    void loadCartridge(const std::string& filename);

public:
    Memory memory;
    Display renderer;
    CPU cpu;
    PPU ppu;
    Timer timer;
    Joypad joypad;

    bool running;
    bool pausing;
    int prevCycles;
};


#endif //EMULATOR_GAMEBOY_H
