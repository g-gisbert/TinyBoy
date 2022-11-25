#include "cpu.h"

CPU::CPU() {
    initMemory();
}

void CPU::initMemory() {
    // Registers
    regs.a = 0x01;
    regs.f = 0x80;
    regs.pc = 0x100;

}

void CPU::step() {

    uint8_t opcode = cart.readCart(regs.pc++);

    Instruction instruction = instructions_set[opcode];

    std::printf("Debug : 0x%02X | ", opcode);

    switch(instruction.byteLength) {
        case 1: {
            std::printf("INSTRUCTION : %s (0x%02X)\n", instruction.name, opcode);
            (*this.*instruction.funcCallVoid)();
            break;
        }
        case 2: {
            uint16_t operand = read8(regs.pc);
            regs.pc++;
            std::printf("INSTRUCTION : ");
            std::printf(instruction.name, operand);
            std::printf(" (0x%02X)\n", opcode);
            (*this.*instruction.funcCall8)(operand);
            break;
        }
        case 3: {
            uint16_t operand = read16(regs.pc);
            regs.pc += 2;
            std::printf("INSTRUCTION : ");
            std::printf(instruction.name, operand);
            std::printf(" (0x%02X)\n", opcode);
            (*this.*instruction.funcCall16)(operand);
            break;
        }
        default:
            std::printf("Instruction not implemented yet!\n");
    }


    int a = 3;
    a += 5;
    //std::cin >> a;
}

// Fetch Memory
uint16_t CPU::read16(uint16_t address) {
    if(address < 0x8000) {
        uint8_t lsb = cart.readCart(address);
        uint8_t msb = cart.readCart(address+1);
        return (uint16_t(msb) << 8) + lsb;
    }
}

uint8_t CPU::read8(uint16_t address) {
    if(address < 0x8000) {
        return cart.readCart(address);
    }
}


// CPU Instructions
void CPU::nop() { // 0x00
}

void CPU::ld_bc_nn(uint16_t nn) { // 0x01
    regs.bc = nn; }

void CPU::ld_c_n(uint8_t n) { // 0x0E
    regs.c = n; }

void CPU::ld_hl_nn(uint16_t nn) { // 0x21
    regs.hl = nn; }

void CPU::jp_nn(uint16_t nn) { // 0xC3
    regs.pc = nn; }

void CPU::xor_a() { // 0xAF
    regs.a ^= regs.a; }



