#include "interrupts.h"

void interruptStep(CPU& cpu) {

    if (!cpu.memory.IME)
        return;

    uint8_t& IE = cpu.memory.IE();
    uint8_t& IF = cpu.memory.IF();

    if ((IE & VBLANK) && (IF & VBLANK)) {
        IF = IF & ~VBLANK;
        handleInterrupt(cpu, 0x40);
    }

    if ((IE & LCD) && (IF & LCD)) {
        IF = IF & ~LCD;
        handleInterrupt(cpu, 0x48);
    }

    if ((IE & TIMER) && (IF & TIMER)) {
        IF = IF & ~TIMER;
        handleInterrupt(cpu, 0x50);
    }

    if ((IE & JOYPAD) && (IF & JOYPAD)) {
        IF = IF & ~JOYPAD;
        handleInterrupt(cpu, 0x60);
    }

}

void handleInterrupt(CPU& cpu, uint16_t address) {
    cpu.memory.IME = false;
    cpu.call_nn(address);
    cpu.cycles += 24;
}