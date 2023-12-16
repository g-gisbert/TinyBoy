#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H

#include "cpu.h"
#include "memory.h"
#include "display.h"

enum : uint8_t {
    H_BLANK = 0b00,
    V_BLANK = 0b01,
    OAM_SEARCH = 0b10,
    PIXEL_TRANSFER = 0b11
};

class PPU {
public:
    PPU(Memory& memo, Display& dis) : memory(memo), display(dis) {}
    void step(int& cycles);
    void statHandle(int mb);

    void renderDebug();

    Memory& memory;
    Display& display;

    int mode;
};


#endif // EMULATOR_PPU_H
