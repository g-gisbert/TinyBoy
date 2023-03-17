#include "cpu.h"

CPU::CPU(Memory& memo) : memory(memo) {
    initMemory();
    debug = false;
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
    cycles = 0;
}

int& CPU::step() {

    uint8_t opcode = memory.read8(regs.pc++);

    Instruction instruction = instructions_set[opcode];

    //std::printf("Debug : 0x%02X | ", opcode);

    switch(instruction.byteLength) {
        case 1: {
            //std::printf("INSTRUCTION : %s (0x%02X)\n", instruction.name, opcode);
            (*this.*instruction.funcCallVoid)();
            break;
        }
        case 2: {
            uint16_t operand = memory.read8(regs.pc);
            regs.pc++;
            //std::printf("INSTRUCTION : ");
            //std::printf(instruction.name, operand);
            //std::printf(" (0x%02X)\n", opcode);
            (*this.*instruction.funcCall8)(operand);
            break;
        }
        case 3: {
            uint16_t operand = memory.read16(regs.pc);
            regs.pc += 2;
            //std::printf("INSTRUCTION : ");
            //std::printf(instruction.name, operand);
            //std::printf(" (0x%02X)\n", opcode);
            (*this.*instruction.funcCall16)(operand);
            break;
        }
        default: {
            std::printf("Instruction not implemented yet! PC : %x, OPCODE : %x\n", regs.pc-1, opcode);
            showState();
            int a;
            std::cin >> a;
        }
    }

    if (regs.pc == 0x9999)
        debug = true;

    if (debug) {
        std::printf("INSTRUCTION : %s (0x%02X)\n", instruction.name, opcode);
        showState();
        int a;
        std::cin >> a;
    }

    cycles += instruction.cycles;
    //std::cout << "PC : " << std::hex << regs.pc << std::endl;
    return cycles;
}

void CPU::showState() const {
    std::cout << "### Registers ###" << std::endl;
    std::cout << "af=" << std::hex << regs.af << std::endl;
    std::cout << "bc=" << std::hex << regs.bc << std::endl;
    std::cout << "de=" << std::hex << regs.de << std::endl;
    std::cout << "hl=" << std::hex << regs.hl << std::endl;
    std::cout << "sp=" << std::hex << regs.sp << std::endl;
    std::cout << "PC : " << regs.pc << std::endl;
}

// Helpers
void CPU::pushToStack(uint16_t nn) {
    uint8_t msb = (nn & 0xFF00) >> 8;
    uint8_t lsb = uint8_t(nn & 0x00FF);
    stack[regs.sp-1] = msb;
    stack[regs.sp-2] = lsb;
    regs.sp -= 2;
}

uint16_t CPU::popFromStack() {
    uint8_t lsb = stack[regs.sp];
    uint8_t msb = stack[regs.sp+1];
    regs.sp += 2;
    return (uint16_t(msb) << 8) + lsb;
}

void CPU::add8(uint8_t r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    uint16_t sum = regs.a + r;
    if (sum == 0)
        regs.setFlag(ZERO_FLAG, 1);
    if (sum > 0x00FF)
        regs.setFlag(CARRY_FLAG, 1);
    if ((regs.a & 0x0F) + (r & 0x0F) > 0x0F)
        regs.setFlag(HALF_CARRY_FLAG, 1);
    regs.a += r;
}

void CPU::add16(uint16_t rr) {
    regs.setFlag(SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    uint32_t sum = regs.hl + rr;
    if (sum > 0x0000FFFF)
        regs.setFlag(CARRY_FLAG, 1);
    if ((regs.hl & 0x0FFF) + (rr & 0x0FFF) > 0x0FFF)
        regs.setFlag(HALF_CARRY_FLAG, 1);
    regs.hl += rr;
}

void CPU::inc(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG, 0);
    if ((r & 0x0F) == 0x0F)
        regs.setFlag(HALF_CARRY_FLAG, 1);
    r++;
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
}

void CPU::dec(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG, 1);
    if (r & 0x0F)
        regs.setFlag(HALF_CARRY_FLAG, 0);
    r--;
    if (r)
        regs.setFlag(ZERO_FLAG, 0);
}

void CPU::_and(uint8_t r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | CARRY_FLAG, 0);
    regs.setFlag(HALF_CARRY_FLAG, 1);
    regs.a &= r;
    if (regs.a == 0)
        regs.setFlag(ZERO_FLAG, 1);
}

void CPU::_or(uint8_t r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    regs.a |= r;
    if (regs.a == 0)
        regs.setFlag(ZERO_FLAG, 1);
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

void CPU::dec_bc() { // 0x0B
    regs.bc--; }

void CPU::inc_c() { // 0x0C
    inc(regs.c); }

void CPU::dec_c() { // 0x0D
    dec(regs.c); }

void CPU::ld_c_n(uint8_t n) { // 0x0E
    regs.c = n; }

void CPU::ld_de_nn(uint16_t nn) { // 0x11
    regs.de = nn;}

void CPU::ld_dep_a() { // 0x12
    memory.write8(regs.de, regs.a); }

void CPU::inc_de() { // 0x13
    regs.de++; }

void CPU::dec_d() { // 0x15
    dec(regs.d); }

void CPU::ld_d_n(uint8_t n) { // 0x16
    regs.d = n;}

void CPU::jr_n(uint8_t n) { // 0x18
    regs.pc += int8_t(n); }

void CPU::add_hl_de() { // 0x19
    add16(regs.de); }

void CPU::ld_a_dep() { // 0x1A
    regs.a = memory.read8(regs.de); }

void CPU::inc_e() { // 0x1C
    inc(regs.e); }

void CPU::dec_e() { // 0x1D
    dec(regs.e); }

void CPU::jr_nz_n(uint8_t n) { // 0x20
    if (regs.checkFlagClear(ZERO_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8;
}

void CPU::ld_hl_nn(uint16_t nn) { // 0x21
    regs.hl = nn; }

void CPU::ldi_hlp_a() { // 0x22
    memory.write8(regs.hl++, regs.a); }

void CPU::inc_hl() { // 0x23
    regs.hl++; }

void CPU::dec_h() { // 0x25
    dec(regs.h); }

void CPU::jr_z_n(uint8_t n) { //0x28
    if (regs.checkFlagSet(ZERO_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8;
}

void CPU::ldi_a_hl() { //0x2A
    regs.a = memory.read8(regs.hl++); }

void CPU::inc_l() { // 0x2C
    inc(regs.l); }

void CPU::dec_l() { // 0x2D
    dec(regs.l); }

void CPU::cpl() { // 0x2F
    regs.setFlag(SUB_FLAG | HALF_CARRY_FLAG, 1);
    regs.a = ~regs.a; }

void CPU::ld_sp_nn(uint16_t nn) { // 0x31
    regs.sp = nn; }

void CPU::ld_hlpm_a() { // 0x32
    //showState();
    memory.write8(regs.hl, regs.a); regs.hl--; }

void CPU::inc_hlp() { // 0x34
    uint8_t tmp = memory.read8(regs.hl);
    inc(tmp);
    memory.write8(regs.hl, memory.read8(regs.hl) + 1); }

void CPU::dec_hlp() { // 0x15
    uint8_t tmp = memory.read8(regs.hl);
    dec(tmp);
    memory.write8(regs.hl, memory.read8(regs.hl) - 1); }

void CPU::ld_hlp_n(uint8_t n) { // 0x36
    memory.write8(regs.hl, n); }

void CPU::inc_a() { // 0x3C
    inc(regs.a); }

void CPU::dec_a() { // 0x3D
    dec(regs.a); }

void CPU::ld_a_n(uint8_t n) { // 0x3E
    regs.a = n; }

void CPU::ld_b_b() { // 0x40
    regs.b = regs.b; }

void CPU::ld_b_c() { // 0x41
    regs.b = regs.c; }

void CPU::ld_b_d() { // 0x42
    regs.b = regs.d; }

void CPU::ld_b_e() { // 0x43
    regs.b = regs.e; }

void CPU::ld_b_h() { // 0x44
    regs.b = regs.h; }

void CPU::ld_b_l() { // 0x45
    regs.b = regs.l; }

void CPU::ld_b_hlp() { // 0x46
    regs.b = memory.read8(regs.hl); }

void CPU::ld_b_a() { // 0x47
    regs.b = regs.a; }

void CPU::ld_c_b() { // 0x48
    regs.c = regs.b; }

void CPU::ld_c_c() { // 0x49
    regs.c = regs.c; }

void CPU::ld_c_d() { // 0x4A
    regs.c = regs.d; }

void CPU::ld_c_e() { // 0x4B
    regs.c = regs.e; }

void CPU::ld_c_h() { // 0x4C
    regs.c = regs.h; }

void CPU::ld_c_l() { // 0x4D
    regs.c = regs.l; }

void CPU::ld_c_hlp() { // 0x4E
    regs.c = memory.read8(regs.hl); }

void CPU::ld_c_a() { // 0x4F
    regs.c = regs.a; }

void CPU::ld_d_b() { // 0x50
    regs.d = regs.b; }

void CPU::ld_d_c() { // 0x51
    regs.d = regs.c; }

void CPU::ld_d_d() { // 0x52
    regs.d = regs.d; }

void CPU::ld_d_e() { // 0x53
    regs.d = regs.e; }

void CPU::ld_d_h() { // 0x54
    regs.d = regs.h; }

void CPU::ld_d_l() { // 0x55
    regs.d = regs.l; }

void CPU::ld_d_hlp() { // 0x56
    regs.d = memory.read8(regs.hl); }

void CPU::ld_d_a() { // 0x57
    regs.d = regs.a; }

void CPU::ld_e_b() { // 0x58
    regs.e = regs.b; }

void CPU::ld_e_c() { // 0x59
    regs.e = regs.c; }

void CPU::ld_e_d() { // 0x5A
    regs.e = regs.d; }

void CPU::ld_e_e() { // 0x5B
    regs.e = regs.e; }

void CPU::ld_e_h() { // 0x5C
    regs.e = regs.h; }

void CPU::ld_e_l() { // 0x5D
    regs.e = regs.l; }

void CPU::ld_e_hlp() { // 0x5E
    regs.e = memory.read8(regs.hl); }

void CPU::ld_e_a() { // 0x5F
    regs.e = regs.a; }

void CPU::ld_h_b() { // 0x60
    regs.h = regs.b; }

void CPU::ld_h_c() { // 0x61
    regs.h = regs.c; }

void CPU::ld_h_d() { // 0x62
    regs.h = regs.d; }

void CPU::ld_h_e() { // 0x63
    regs.h = regs.e; }

void CPU::ld_h_h() { // 0x64
    regs.h = regs.h; }

void CPU::ld_h_l() { // 0x65
    regs.h = regs.l; }

void CPU::ld_h_hlp() { // 0x66
    regs.h = memory.read8(regs.hl); }

void CPU::ld_h_a() { // 0x67
    regs.h = regs.a; }

void CPU::ld_l_b() { // 0x68
    regs.l = regs.b; }

void CPU::ld_l_c() { // 0x69
    regs.l = regs.c; }

void CPU::ld_l_d() { // 0x6A
    regs.l = regs.d; }

void CPU::ld_l_e() { // 0x6B
    regs.l = regs.e; }

void CPU::ld_l_h() { // 0x6C
    regs.l = regs.h; }

void CPU::ld_l_l() { // 0x6D
    regs.l = regs.l; }

void CPU::ld_l_hlp() { // 0x6E
    regs.l = memory.read8(regs.hl); }

void CPU::ld_l_a() { // 0x6F
    regs.l = regs.a; }

void CPU::ld_hlp_b() { // 0x70
    memory.write8(regs.hl, regs.b); }

void CPU::ld_hlp_c() { // 0x71
    memory.write8(regs.hl, regs.c); }

void CPU::ld_hlp_d() { // 0x72
    memory.write8(regs.hl, regs.d); }

void CPU::ld_hlp_e() { // 0x73
    memory.write8(regs.hl, regs.e); }

void CPU::ld_hlp_h() { // 0x74
    memory.write8(regs.hl, regs.h); }

void CPU::ld_hlp_l() { // 0x75
    memory.write8(regs.hl, regs.l); }

// 0x76

void CPU::ld_hlp_a() { // 0x77
    memory.write8(regs.hl, regs.a ); }

void CPU::ld_a_b() { //0x78
    regs.a = regs.b; }

void CPU::ld_a_c() { // 0x79
    regs.a = regs.c; }

void CPU::ld_a_d() { // 0x7A
    regs.a = regs.d; }

void CPU::ld_a_e() { // 0x7B
    regs.a = regs.e; }

void CPU::ld_a_h() { // 0x7C
    regs.a = regs.h; }

void CPU::ld_a_l() { // 0x7D
    regs.a = regs.l; }

void CPU::ld_a_hlp() { // 0x7E
    regs.a = memory.read8(regs.hl); }

void CPU::ld_a_a() { // 0x7F
    regs.a = regs.a; }

void CPU::add_a() { // 0x87
    add8(regs.a); }

void CPU::and_c() { // 0xA1
    _and(regs.c); }

void CPU::and_a() { // 0xA7
    _and(regs.a); }

void CPU::xor_c() { // 0xA9
    _xor(regs.c); }

void CPU::xor_a() { // 0xAF
    _xor(regs.a); }

void CPU::or_b() { // 0xB0
    _or(regs.b); }

void CPU::or_c() { // 0xB1
    _or(regs.c); }

void CPU::ret_nz() { // 0xC0
    if (regs.checkFlagClear(ZERO_FLAG)) {
        regs.pc = popFromStack();
        cycles += 20;
    } else
        cycles += 8; }

void CPU::pop_bc() { // 0xC1
    regs.bc = popFromStack(); }

void CPU::jp_nn(uint16_t nn) { // 0xC3
    regs.pc = nn; }

void CPU::push_bc() { // 0xC5
    pushToStack(regs.bc); }

void CPU::ret_z() { // 0xC8
    if (regs.checkFlagSet(ZERO_FLAG)) {
        regs.pc = popFromStack();
        cycles += 20;
    } else
        cycles += 8; }

void CPU::ret() { //0xC9
    regs.pc = popFromStack(); }

void CPU::jp_z_nn(uint16_t nn) { // 0xCA
    if (regs.checkFlagSet(ZERO_FLAG)) {
        regs.pc = nn;
        cycles += 16;
    } else
        cycles += 12; }

void CPU::cb_manager(uint8_t n) { // 0xCB
    Instruction instr = CB_instructions[n];
    if (instr.byteLength == 0) {
        std::cout << "Missing instruction => PC : " << regs.pc - 1 << ", opcode : CB " << int(n) << std::endl;
        int a;
        std::cin >> a;
    }
    (*this.*instr.funcCallVoid)();
    cycles += instr.cycles; }

void CPU::call_nn(uint16_t nn) { // 0xCD
    pushToStack(regs.pc);
    regs.pc = nn; }

void CPU::pop_de() { // 0xD1
    regs.de = popFromStack(); }

void CPU::push_de() { // 0xD5
    pushToStack(regs.de); }

void CPU::reti() { // 0xD9
    memory.IME = true;
    regs.pc = popFromStack(); }

void CPU::ldh_n_a(uint8_t n) { // 0xE0
    memory.write8(0xFF00 + n, regs.a); }

void CPU::pop_hl() { // 0xE1
    regs.hl = popFromStack(); }

void CPU::ld_cp_a(uint8_t n) { // 0xE2
    memory.write8(0xFF00 + regs.c, regs.a); }

void CPU::push_hl() { // 0xE5
    pushToStack(regs.hl); }

void CPU::and_n(uint8_t n) { // 0xE6
    _and(n); }

void CPU::jp_hlp() { // 0xE9
    regs.pc = regs.hl; }

void CPU::ld_nnp_a(uint16_t nn) { // 0xEA
    memory.write8(nn, regs.a); }

void CPU::rst_28() { // 0xEF
    pushToStack(regs.pc);
    regs.pc = 0x0028;
}

void CPU::ldh_a_n(uint8_t n) { // 0xF0
    regs.a = memory.read8(0xFF00 + n); }

void CPU::pop_af() { // 0xF1 TODO Z N H C
    regs.af = popFromStack(); }

void CPU::di() { //0xF3
    memory.IME = false; }

void CPU::push_af() { // 0xF5
    pushToStack(regs.af); }

void CPU::ld_a_nnp(uint16_t nn) { // 0xFA
    regs.a = memory.read8(nn); }

void CPU::ei() { // 0xFB
    memory.IME = true; }

void CPU::cp_n(uint8_t n) { // 0xFE
    cp(n); }

// CB instructions
void CPU::swap(uint8_t& r) {
    uint8_t msb = (r & 0xFF00) >> 8;
    uint8_t lsb = r & 0xFF;
    r = (uint16_t(lsb) << 8) + msb;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
}

void CPU::res8(uint8_t& r, int b) {
    r &= ~(1 << b);
}

void CPU::swap_a() { // 0x37
    swap(regs.a);
}

void CPU::res_0_a() { // 0x87
    res8(regs.a, 0); }