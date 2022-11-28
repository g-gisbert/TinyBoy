#include "cpu.h"

CPU::CPU() {
    initMemory();
}

void CPU::initMemory() {
    // Registers
    regs.af = 0x0180;
    regs.bc = 0x0013;
    regs.de = 0x00D8;
    regs.hl = 0x014D;
    regs.pc = 0x100;
    regs.sp = 0xFFFE;


    std::memset(stack, 0, 0xFFFF);
    std::memset(WRAM, 0, 0x2000);
    IME = 0x00;
}

void CPU::step() {

    uint8_t opcode = cart.readCart(regs.pc++);

    Instruction instruction = instructions_set[opcode];

    //std::printf("Debug : 0x%02X | ", opcode);

    switch(instruction.byteLength) {
        case 1: {
            //std::printf("INSTRUCTION : %s (0x%02X)\n", instruction.name, opcode);
            (*this.*instruction.funcCallVoid)();
            break;
        }
        case 2: {
            uint16_t operand = read8(regs.pc);
            regs.pc++;
            //std::printf("INSTRUCTION : ");
            //std::printf(instruction.name, operand);
            //std::printf(" (0x%02X)\n", opcode);
            (*this.*instruction.funcCall8)(operand);
            break;
        }
        case 3: {
            uint16_t operand = read16(regs.pc);
            regs.pc += 2;
            //std::printf("INSTRUCTION : ");
            //std::printf(instruction.name, operand);
            //std::printf(" (0x%02X)\n", opcode);
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
    uint8_t lsb = read8(address);
    uint8_t msb = read8(address+1);
    return (uint16_t(msb) << 8) + lsb;
    /*if(address < 0x8000) {
        uint8_t lsb = cart.readCart(address);
        uint8_t msb = cart.readCart(address+1);

    }*/
}

uint8_t CPU::read8(uint16_t address) {
    if(address < 0x8000) { // ROM
        return cart.readCart(address);
    } else if (address < 0xA000) { // VRAM

    } else if (address < 0xC000) { // extern RAM

    } else if (address < 0xE000) { // WRAM
        return WRAM[address - 0xC000];
    } else if (address < 0xFE00) { // unused
    } else if (address < 0xFEA0) { // OAM
        return OAM[address - 0xFE00];
    } else if (address < 0xFF00) { // unused
    } else if (address < 0xFF80) { // I/O Registers
        return IORegisters[address - 0xFF00];
    } else if (address < 0xFFFE) { // HRAM
        return HRAM[address - 0xFF80];
    } else if (address == 0xFFFF) { // IME
        return IME;
    }
}

void CPU::write8(uint16_t address, uint8_t value) {
    if(address < 0x8000) { // ROM

    } else if (address < 0xA000) { // VRAM

    } else if (address < 0xC000) { // extern RAM

    } else if (address < 0xE000) { // WRAM
        WRAM[address - 0xC000] = value;
    } else if (address < 0xFE00) { // unused
    } else if (address < 0xFEA0) { // OAM
        OAM[address - 0xFE00] = value;
    } else if (address < 0xFF00) { // unused
    } else if (address < 0xFF80) { // I/O Registers
        IORegisters[address - 0xFF00] = value;
    } else if (address < 0xFFFE) { // HRAM
        HRAM[address - 0xFF80] = value;
    } else if (address == 0xFFFF) { // IME
        IME = value;
    }
}

// Helpers
void CPU::dec(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG, 1);
    if (r & 0x0f)
        regs.setFlag(HALF_CARRY_FLAG, 0);
    r--;
    if (r)
        regs.setFlag(ZERO_FLAG, 0);
}

void CPU::_xor(uint8_t r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    regs.a ^= r;
    if (regs.a == 0)
        regs.setFlag(ZERO_FLAG, 1);
}

void CPU::cp(uint8_t val) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 1);
    if (regs.a != val)
        regs.setFlag(ZERO_FLAG, 0);
    if (regs.a >= val)
        regs.setFlag(CARRY_FLAG, 0);
    if ((regs.a & 0x0F) >= (val & 0x0F))
        regs.setFlag(HALF_CARRY_FLAG, 0);
}


// CPU Instructions
void CPU::nop() { // 0x00
}

void CPU::ld_bc_nn(uint16_t nn) { // 0x01
    regs.bc = nn; }

void CPU::dec_b() { // 0x05
    dec(regs.b); }

void CPU::ld_b_n(uint8_t n) { // 0x06
    regs.b = n; }

void CPU::dec_c() { // 0x0D
    dec(regs.c); }

void CPU::ld_c_n(uint8_t n) { // 0x0E
    regs.c = n; }

void CPU::jr_nz_n(uint8_t n) { // 0x20
    if(regs.checkFlagClear(ZERO_FLAG)) regs.pc += int8_t(n); }

void CPU::ld_hl_nn(uint16_t nn) { // 0x21
    regs.hl = nn; }

void CPU::ld_hlpm_a() { // 0x32
    write8(regs.hl, regs.a); regs.hl--; }

void CPU::ld_a_n(uint8_t n) { // 0x3E
    regs.a = n; }

void CPU::jp_nn(uint16_t nn) { // 0xC3
    regs.pc = nn; }

void CPU::xor_a() { // 0xAF
    _xor(regs.a); }

void CPU::ldh_n_a(uint8_t n) { // 0xE0
    write8(0xFF00 + n, regs.a); }

void CPU::ldh_a_n(uint8_t n) { // 0xF0
    regs.a = read8(0xFF00 + n); }

void CPU::di() { //0xF3
    write8(0xFFFF, 0x00); }

void CPU::cp_n(uint8_t n) { // 0xFE
    cp(n); }