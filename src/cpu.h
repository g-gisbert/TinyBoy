#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "registers.h"
#include "instructions.h"
#include "cartridge.h"
#include <cstdio>
#include <iostream>

class CPU {
public:
    CPU();
    void step();

    Registers regs;
    Cartridge cart;
};


#endif //EMULATOR_CPU_H
