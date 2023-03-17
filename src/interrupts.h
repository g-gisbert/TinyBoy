#ifndef EMULATOR_INTERRUPTS_H
#define EMULATOR_INTERRUPTS_H

#include "cpu.h"

void interruptStep(CPU& cpu);
void handleInterrupt(CPU& cpu, uint16_t address);

#endif //EMULATOR_INTERRUPTS_H
