#include "cpu.h"

CPU::CPU() {
    regs.pc = 0x100;
}

void CPU::step() {

    uint8_t opcode = cart.getOpcode(regs.pc);

    std::printf("INSTRUCTION : %s (0x%02X)\n", instructions_set[opcode].name, opcode);
    regs.pc++;
    int a;
    std::cin >> a;
}