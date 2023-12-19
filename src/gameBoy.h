#ifndef EMULATOR_GAMEBOY_H
#define EMULATOR_GAMEBOY_H

#include "cpu.h"
#include "ppu.h"
#include "interrupts.h"
#include "display.h"
#include "timer.h"
#include "joypad.h"
#include <string>


class GameBoy {
public:
    GameBoy();
    void run();

private:
    void setupSequence();
    void loadCartridge(std::string filename);

public:
    Memory memory;
    Display renderer;
    CPU cpu;
    PPU ppu;
    Timer timer;
    Joypad joypad;

    bool running;
    bool pausing;
};


#endif //EMULATOR_GAMEBOY_H
