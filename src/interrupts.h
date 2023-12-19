#ifndef EMULATOR_INTERRUPTS_H
#define EMULATOR_INTERRUPTS_H

#include "cpu.h"

enum Interrupts {
    VBLANK = (1 << 0),
    LCD = (1 << 1),
    TIMER = (1 << 2),
    SERIAL = (1 << 3),
    JOYPAD = (1 << 4)
};

void interruptStep(CPU& cpu);
void handleInterrupt(CPU& cpu, uint16_t address);

#endif //EMULATOR_INTERRUPTS_H
