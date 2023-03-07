#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H

#include "cpu.h"
#include "memory.h"

enum {
    H_BLANK,
    V_BLANK,
    OAM_SEARCH,
    PIXEL_TRANSFER
};

class PPU {
public:
    PPU(Memory& memo) :  memory(memo) {}
    void step(int& cycles);

    Memory& memory;

    int mode;
};


#endif // EMULATOR_PPU_H
