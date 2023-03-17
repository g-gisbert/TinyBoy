#include "interrupts.h"

void interruptStep(CPU& cpu) {

    if (!cpu.memory.IME)
        return;

    uint8_t& IE = cpu.memory.IE();
    uint8_t& IF = cpu.memory.IF();

    if ((IE & 0x1) && (IF & 0x1)) { // VBlank interrupt
        IF = IF & ~(0x1);
        handleInterrupt(cpu, 0x40);
    }

}

void handleInterrupt(CPU& cpu, uint16_t address) {
    cpu.memory.IME = false;
    cpu.call_nn(address);
    cpu.cycles += 24;
}