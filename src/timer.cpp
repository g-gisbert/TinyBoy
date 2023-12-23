#include "timer.h"


void Timer::step(int cpuCycles) {
    divCycles += cpuCycles;

    if (divCycles >= 256) {
        divCycles -= 256;
        memory.DIV()++;
    }

    if (memory.TAC() & 0x04) {
        int period = 0;
        switch (memory.TAC() & 0x03) {
            case 0x0:
                period = 1024;
                break;
            case 0x1:
                period = 16;
                break;
            case 0x2:
                period = 64;
                break;
            case 0x3:
                period = 256;
                break;
        }

        timaCycles += cpuCycles;

        if (timaCycles >= period) {
            timaCycles -= period;
            if (memory.TIMA() == 0xFF) {
                memory.TIMA() = memory.TMA();
                memory.IF() |= 0x04;
            } else {
                memory.TIMA()++;
            }
        }
    }
}