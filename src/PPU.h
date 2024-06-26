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
    PPU(Memory& memo, Display& dis) : memory(memo), display(dis), internalCycles(0) {}
    void step(int cycles);
    void changeMode(int m);

    Memory& memory;
    Display& display;
    bool BGW1_3[160];

    int mode;
    int internalCycles;

private:
    void printScreen(int LY);
    void printBackground(int LY);
    void printWindow(int LY);
    void printSprites(int LY);
};


#endif // EMULATOR_PPU_H
