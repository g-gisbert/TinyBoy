#ifndef EMULATOR_TIMER_H
#define EMULATOR_TIMER_H

#include "memory.h"

class Timer {
public:
    Timer(Memory& mem) : memory(mem), divCycles(0), timaCycles(0) {}

    void step(int cpuCycles);
private:
    Memory& memory;
    int divCycles;
    int timaCycles;
};


#endif //EMULATOR_TIMER_H
