#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "registers.h"
#include "instructions.h"
#include <cstdio>
#include <iostream>

class CPU {
public:
    CPU();
    void step(uint8_t opcode);

    Registers regs;
};


#endif //EMULATOR_CPU_H
