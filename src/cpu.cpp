#include "cpu.h"
#include <iomanip>

CPU::CPU(Memory& memo) : memory(memo) {
    initMemory();
    nInstr = 0;
    debug = false;
}

void CPU::initMemory() {
    // Registers
    regs.af = 0x01B0;
    regs.bc = 0x0013;
    regs.de = 0x00D8;
    regs.hl = 0x014D;
    regs.pc = 0x100;
    regs.sp = 0xFFFE;

    std::memset(stack, 0, 0xFFFF);
    cycles = 0;
}

int& CPU::step() {

    cycles = 0;

    uint8_t opcode = memory.read8(regs.pc++);
    Instruction instruction = instructions_set[opcode];

    switch(instruction.byteLength) {
        case 1: {
            (*this.*instruction.funcCallVoid)();
            break;
        }
        case 2: {
            uint16_t operand = memory.read8(regs.pc);
            regs.pc++;
            (*this.*instruction.funcCall8)(operand);
            break;
        }
        case 3: {
            uint16_t operand = memory.read16(regs.pc);
            regs.pc += 2;
            (*this.*instruction.funcCall16)(operand);
            break;
        }
        default: {
            std::printf("Instruction not implemented yet! PC : %x, OPCODE : %x\n", regs.pc-1, opcode);
            showState();
        }
    }

    cycles += instruction.cycles;

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
    regs.sp -= 2;
    memory.write16(regs.sp, nn);
}

uint16_t CPU::popFromStack() {
    uint16_t value = memory.read16(regs.sp);
    regs.sp += 2;
    return value;
}

void CPU::add8(uint8_t r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    uint8_t sum8 = regs.a + r;
    uint16_t sum16 = regs.a + r;
    if (sum8 == 0)
        regs.setFlag(ZERO_FLAG, 1);
    if (sum16 > 0x00FF)
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

void CPU::adc(uint8_t r) {
    int carry = regs.checkFlagSet(CARRY_FLAG) ? 1 : 0;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    uint8_t sum8 = regs.a + r + carry;
    uint16_t sum16 = regs.a + r + carry;
    if (sum8 == 0)
        regs.setFlag(ZERO_FLAG, 1);
    if (sum16 > 0x00FF)
        regs.setFlag(CARRY_FLAG, 1);
    if ((regs.a & 0x0F) + (r & 0x0F) + carry > 0x0F)
        regs.setFlag(HALF_CARRY_FLAG, 1);
    regs.a += r + carry;
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

void CPU::sub(uint8_t r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 1);
    if (regs.a >= r)
        regs.setFlag(CARRY_FLAG, 0);
    if ((regs.a & 0x0F) >= (r & 0x0F))
        regs.setFlag(HALF_CARRY_FLAG, 0);
    regs.a -= r;
    if (regs.a)
        regs.setFlag(ZERO_FLAG, 0);
}

void CPU::sbc(uint8_t r) {
    int carry = regs.checkFlagSet(CARRY_FLAG) ? 1 : 0;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 1);
    if (regs.a >= r+carry)
        regs.setFlag(CARRY_FLAG, 0);
    if ((regs.a & 0x0F) >= (r & 0x0F) + carry)
        regs.setFlag(HALF_CARRY_FLAG, 0);
    regs.a -= (r + carry);
    if (regs.a)
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

void CPU::ld_bcp_a() { // 0x02
    memory.write8(regs.bc, regs.a); }

void CPU::inc_bc() { // 0x03
    regs.bc++; }

void CPU::inc_b() { // 0x04
    inc(regs.b); }

void CPU::dec_b() { // 0x05
    dec(regs.b); }

void CPU::ld_b_n(uint8_t n) { // 0x06
    regs.b = n; }

void CPU::rlca() { // 0x07
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    bool carry = (regs.a >= 0x80);
    if (carry)
        regs.setFlag(CARRY_FLAG, 1);
    regs.a = regs.a << 1;
    regs.a += uint8_t(carry);
}

void CPU::ld_nn_sp(uint16_t nn) { // 0x08
    memory.write16(nn, regs.sp); }

void CPU::add_hl_bc() { // 0x09
    add16(regs.bc); }

void CPU::ld_a_bcp() { // 0x0A
    regs.a = memory.read8(regs.bc); }

void CPU::dec_bc() { // 0x0B
    regs.bc--; }

void CPU::inc_c() { // 0x0C
    inc(regs.c); }

void CPU::dec_c() { // 0x0D
    dec(regs.c); }

void CPU::ld_c_n(uint8_t n) { // 0x0E
    regs.c = n; }

void CPU::rrca() { // 0x0F
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    int carry = (regs.a & 0x01) ? 1 : 0;
    if (carry)
        regs.setFlag(CARRY_FLAG, 1);
    regs.a = regs.a >> 1;
    regs.a |= (carry << 7); }

void CPU::ld_de_nn(uint16_t nn) { // 0x11
    regs.de = nn;}

void CPU::ld_dep_a() { // 0x12
    memory.write8(regs.de, regs.a); }

void CPU::inc_de() { // 0x13
    regs.de++; }

void CPU::inc_d() { // 0x14
    inc(regs.d); }

void CPU::dec_d() { // 0x15
    dec(regs.d); }

void CPU::ld_d_n(uint8_t n) { // 0x16
    regs.d = n;}

void CPU::rla() { // 0x17
    int carry = (regs.checkFlagSet(CARRY_FLAG)) ? 1 : 0;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (regs.a >= 0x80)
        regs.setFlag(CARRY_FLAG, 1);
    regs.a = regs.a << 1;
    regs.a += uint8_t(carry); }

void CPU::jr_n(uint8_t n) { // 0x18
    regs.pc += int8_t(n); }

void CPU::add_hl_de() { // 0x19
    add16(regs.de); }

void CPU::ld_a_dep() { // 0x1A
    regs.a = memory.read8(regs.de); }

void CPU::dec_de() { // 0x1B
    regs.de--; }

void CPU::inc_e() { // 0x1C
    inc(regs.e); }

void CPU::dec_e() { // 0x1D
    dec(regs.e); }

void CPU::ld_e_n(uint8_t n) { // 0x1E
    regs.e = n; }

void CPU::rra() { // 0x1F
    int carry = (regs.checkFlagSet(CARRY_FLAG)) ? 1 : 0;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (regs.a & 0x01)
        regs.setFlag(CARRY_FLAG, 1);
    regs.a = regs.a >> 1;
    regs.a += carry << 7; }

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

void CPU::inc_h() { // 0x24
    inc(regs.h); }

void CPU::dec_h() { // 0x25
    dec(regs.h); }

void CPU::ld_h_n(uint8_t n) { // 0x26
    regs.h = n; }

// Solution from : https://forums.nesdev.org/viewtopic.php?t=15944
void CPU::daa() { // 0x27
    uint16_t s = regs.a;

    if(regs.checkFlagSet(SUB_FLAG)) {
        if(regs.checkFlagSet(HALF_CARRY_FLAG)) s = (s - 0x06)&0xFF;
        if(regs.checkFlagSet(CARRY_FLAG)) s -= 0x60;
    }
    else {
        if(regs.checkFlagSet(HALF_CARRY_FLAG) || (s & 0xF) > 9) s += 0x06;
        if(regs.checkFlagSet(CARRY_FLAG) || s > 0x9F) s += 0x60;
    }

    regs.a = s;
    regs.setFlag(HALF_CARRY_FLAG, 0);

    if(regs.a) regs.setFlag(ZERO_FLAG, 0);
    else regs.setFlag(ZERO_FLAG, 1);

    if(s >= 0x100) regs.setFlag(CARRY_FLAG, 1);
}

void CPU::jr_z_n(uint8_t n) { //0x28
    if (regs.checkFlagSet(ZERO_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8;
}

void CPU::add_hl_hl() { // 0x29
    add16(regs.hl); }

void CPU::ldi_a_hl() { //0x2A
    regs.a = memory.read8(regs.hl++); }

void CPU::dec_hl() { // 0x2B
    regs.hl--; }

void CPU::inc_l() { // 0x2C
    inc(regs.l); }

void CPU::dec_l() { // 0x2D
    dec(regs.l); }

void CPU::ld_l_n(uint8_t n) { // 0x2E
    regs.l = n; }

void CPU::cpl() { // 0x2F
    regs.setFlag(SUB_FLAG | HALF_CARRY_FLAG, 1);
    regs.a = ~regs.a; }

void CPU::jr_nc_n(uint8_t n) { // 0x30
    if (regs.checkFlagClear(CARRY_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8; }

void CPU::ld_sp_nn(uint16_t nn) { // 0x31
    regs.sp = nn; }

void CPU::ld_hlpm_a() { // 0x32
    memory.write8(regs.hl, regs.a); regs.hl--; }

void CPU::inc_sp() { // 0x33
    regs.sp++; }

void CPU::inc_hlp() { // 0x34
    uint8_t tmp = memory.read8(regs.hl);
    inc(tmp);
    memory.write8(regs.hl, memory.read8(regs.hl) + 1); }

void CPU::dec_hlp() { // 0x35
    uint8_t tmp = memory.read8(regs.hl);
    dec(tmp);
    memory.write8(regs.hl, memory.read8(regs.hl) - 1); }

void CPU::ld_hlp_n(uint8_t n) { // 0x36
    memory.write8(regs.hl, n); }

void CPU::scf() {// 0x37
    regs.setFlag(SUB_FLAG | HALF_CARRY_FLAG, 0);
    regs.setFlag(CARRY_FLAG, 1); }

void CPU::jr_c_n(uint8_t n) { // 0x38
    if (regs.checkFlagSet(CARRY_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8; }

void CPU::add_hl_sp() { // 0x39
    add16(regs.sp); }

void CPU::ldd_a_hl() { //0x3A
    regs.a = memory.read8(regs.hl--); }

void CPU::dec_sp() { // 0x3B
    regs.sp--; }

void CPU::inc_a() { // 0x3C
    inc(regs.a); }

void CPU::dec_a() { // 0x3D
    dec(regs.a); }

void CPU::ld_a_n(uint8_t n) { // 0x3E
    regs.a = n; }

void CPU::ccf() { // 0x3F
    regs.setFlag(SUB_FLAG | HALF_CARRY_FLAG, 0);
    if (regs.checkFlagSet(CARRY_FLAG))
        regs.setFlag(CARRY_FLAG, 0);
    else
        regs.setFlag(CARRY_FLAG, 1);
}

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

void CPU::halt() {// 0x76
    return; }

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

void CPU::add_b() { // 0x80
    add8(regs.b); }

void CPU::add_c() { // 0x81
    add8(regs.c); }

void CPU::add_d() { // 0x82
    add8(regs.d); }

void CPU::add_e() { // 0x83
    add8(regs.e); }

void CPU::add_h() { // 0x84
    add8(regs.h); }

void CPU::add_l() { // 0x85
    add8(regs.l); }

void CPU::add_hlp() { // 0x86
    add8(memory.read8(regs.hl)); }

void CPU::add_a() { // 0x87
    add8(regs.a); }

void CPU::adc_b() { // 0x88
    adc(regs.b); }

void CPU::adc_c() { // 0x89
    adc(regs.c); }

void CPU::adc_d() { // 0x8A
    adc(regs.d); }

void CPU::adc_e() { // 0x8B
    adc(regs.e); }

void CPU::adc_h() { // 0x8C
    adc(regs.h); }

void CPU::adc_l() { // 0x8D
    adc(regs.l); }

void CPU::adc_hlp() { // 0x8E
    adc(memory.read8(regs.hl)); }

void CPU::adc_a() { // 0x8F
    adc(regs.a); }

void CPU::sub_b() { // 0x90
    sub(regs.b); }

void CPU::sub_c() { // 0x91
    sub(regs.c); }

void CPU::sub_d() { // 0x92
    sub(regs.d); }

void CPU::sub_e() { // 0x93
    sub(regs.e); }

void CPU::sub_h() { // 0x94
    sub(regs.h); }

void CPU::sub_l() { // 0x95
    sub(regs.l); }

void CPU::sub_hlp() { // 0x96
    sub(memory.read8(regs.hl)); }

void CPU::sub_a() { // 0x97
    sub(regs.a); }

void CPU::sbc_b() { // 0x98
    sbc(regs.b); }

void CPU::sbc_c() { // 0x99
    sbc(regs.c); }

void CPU::sbc_d() { // 0x9A
    sbc(regs.d); }

void CPU::sbc_e() { // 0x9B
    sbc(regs.e); }

void CPU::sbc_h() { // 0x9C
    sbc(regs.h); }

void CPU::sbc_l() { // 0x9D
    sbc(regs.l); }

void CPU::sbc_hlp() { // 0x9E
    sbc(memory.read8(regs.hl)); }

void CPU::sbc_a() { // 0x9F
    sbc(regs.a); }

void CPU::and_b() { // 0xA0
    _and(regs.b); }

void CPU::and_c() { // 0xA1
    _and(regs.c); }

void CPU::and_d() { // 0xA2
    _and(regs.d); }

void CPU::and_e() { // 0xA3
    _and(regs.e); }

void CPU::and_h() { // 0xA4
    _and(regs.h); }

void CPU::and_l() { // 0xA5
    _and(regs.l); }

void CPU::and_hlp() { // 0xA6
    _and(memory.read8(regs.hl)); }

void CPU::and_a() { // 0xA7
    _and(regs.a); }

void CPU::xor_b() { // 0xA8
    _xor(regs.b); }

void CPU::xor_c() { // 0xA9
    _xor(regs.c); }

void CPU::xor_d() { // 0xAA
    _xor(regs.d); }

void CPU::xor_e() { // 0xAB
    _xor(regs.e); }

void CPU::xor_h() { // 0xAC
    _xor(regs.h); }

void CPU::xor_l() { // 0xAD
    _xor(regs.l); }

void CPU::xor_hlp() { // 0xAE
    _xor(memory.read8(regs.hl)); }

void CPU::xor_a() { // 0xAF
    _xor(regs.a); }

void CPU::or_b() { // 0xB0
    _or(regs.b); }

void CPU::or_c() { // 0xB1
    _or(regs.c); }

void CPU::or_d() { // 0xB2
    _or(regs.d); }

void CPU::or_e() { // 0xB3
    _or(regs.e); }

void CPU::or_h() { // 0xB4
    _or(regs.h); }

void CPU::or_l() { // 0xB5
    _or(regs.l); }

void CPU::or_hlp() { // 0xB6
    _or(memory.read8(regs.hl)); }

void CPU::or_a() { // 0xB7
    _or(regs.a); }

void CPU::cp_b() { // 0xB8
    cp(regs.b); }

void CPU::cp_c() { // 0xB9
    cp(regs.c); }

void CPU::cp_d() { // 0xBA
    cp(regs.d); }

void CPU::cp_e() { // 0xBB
    cp(regs.e); }

void CPU::cp_h() { // 0xBC
    cp(regs.h); }

void CPU::cp_l() { // 0xBD
    cp(regs.l); }

void CPU::cp_hlp() { // 0xBE
    cp(memory.read8(regs.hl)); }

void CPU::cp_a() { // 0xBF
    cp(regs.a); }

void CPU::ret_nz() { // 0xC0
    if (regs.checkFlagClear(ZERO_FLAG)) {
        regs.pc = popFromStack();
        cycles += 20;
    } else
        cycles += 8; }

void CPU::pop_bc() { // 0xC1
    regs.bc = popFromStack(); }

void CPU::jp_nz_nn(uint16_t nn) { // 0xC2
    if (regs.checkFlagClear(ZERO_FLAG)) {
        regs.pc = nn;
        cycles += 16;
    } else
        cycles += 12; }

void CPU::jp_nn(uint16_t nn) { // 0xC3
    regs.pc = nn; }

void CPU::call_nz_nn(uint16_t nn) { // 0xC4
    if (regs.checkFlagClear(ZERO_FLAG)) {
        pushToStack(regs.pc);
        regs.pc = nn;
        cycles += 24;
    }
    else
        cycles +=12; }

void CPU::push_bc() { // 0xC5
    pushToStack(regs.bc); }

void CPU::add_a_n(uint8_t n) { // 0xC6
    add8(n); }

void CPU::rst_00() { // 0xC7
    pushToStack(regs.pc);
    regs.pc = 0x0000; }

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

void CPU::call_z_nn(uint16_t nn) { // 0xCC
    if (regs.checkFlagSet(ZERO_FLAG)) {
        pushToStack(regs.pc);
        regs.pc = nn;
        cycles += 24;
    }
    else
        cycles +=12; }

void CPU::call_nn(uint16_t nn) { // 0xCD
    pushToStack(regs.pc);
    regs.pc = nn; }

void CPU::adc_n(uint8_t n) { // 0xCE
    adc(n); }

void CPU::rst_08() { // 0xCF
    pushToStack(regs.pc);
    regs.pc = 0x0008; }

void CPU::ret_nc() { // 0xD0
    if (regs.checkFlagClear(CARRY_FLAG)) {
        regs.pc = popFromStack();
        cycles += 20;
    } else
        cycles += 8; }

void CPU::pop_de() { // 0xD1
    regs.de = popFromStack(); }

void CPU::jp_nc_nn(uint16_t nn) { // 0xD2
    if (regs.checkFlagClear(CARRY_FLAG)) {
        regs.pc = nn;
        cycles += 16;
    } else
        cycles += 12; }

void CPU::call_nc_nn(uint16_t nn) { // 0xD4
    if (regs.checkFlagClear(CARRY_FLAG)) {
        pushToStack(regs.pc);
        regs.pc = nn;
        cycles += 24;
    }
    else
        cycles +=12; }

void CPU::push_de() { // 0xD5
    pushToStack(regs.de); }

void CPU::sub_n(uint8_t n) { // 0xD6
    sub(n); }

void CPU::rst_10() { // 0xD7
    pushToStack(regs.pc);
    regs.pc = 0x0010; }

void CPU::ret_c() { // 0xD8
    if (regs.checkFlagSet(CARRY_FLAG)) {
        regs.pc = popFromStack();
        cycles += 20;
    } else
        cycles += 8; }

void CPU::reti() { // 0xD9
    memory.IME = true;
    regs.pc = popFromStack(); }

void CPU::jp_c_nn(uint16_t nn) { // 0xDA
    if (regs.checkFlagSet(CARRY_FLAG)) {
        regs.pc = nn;
        cycles += 16;
    } else
        cycles += 12; }

void CPU::call_c_nn(uint16_t nn) { // 0xDC
    if (regs.checkFlagSet(CARRY_FLAG)) {
        pushToStack(regs.pc);
        regs.pc = nn;
        cycles += 24;
    }
    else
        cycles +=12; }

void CPU::sbc_n(uint8_t n) { // 0xDE
    sbc(n); }

void CPU::rst_18() { // 0xDF
    pushToStack(regs.pc);
    regs.pc = 0x0018; }

void CPU::ldh_n_a(uint8_t n) { // 0xE0
    memory.write8(0xFF00 + n, regs.a); }

void CPU::pop_hl() { // 0xE1
    regs.hl = popFromStack(); }

void CPU::ld_cp_a() { // 0xE2
    memory.write8(0xFF00 + regs.c, regs.a); }

void CPU::push_hl() { // 0xE5
    pushToStack(regs.hl); }

void CPU::and_n(uint8_t n) { // 0xE6
    _and(n); }

void CPU::rst_20() { // 0xE7
    pushToStack(regs.pc);
    regs.pc = 0x0020; }

void CPU::add_sp_n(uint8_t n) { // 0xE8
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    uint32_t sum = regs.sp + int8_t(n);

    if((regs.sp & 0xFF) + (n & 0xFF) > 0x000000FF)
        regs.setFlag(CARRY_FLAG, 1);

    if(((regs.sp & 0x0F) + (n & 0x0F)) > 0x0F)
        regs.setFlag(HALF_CARRY_FLAG, 1);

    regs.sp = uint16_t(sum);
    }

void CPU::jp_hlp() { // 0xE9
    regs.pc = regs.hl; }

void CPU::ld_nnp_a(uint16_t nn) { // 0xEA
    memory.write8(nn, regs.a); }

void CPU::xor_n(uint8_t n) { // 0xEE
    _xor(n); }

void CPU::rst_28() { // 0xEF
    pushToStack(regs.pc);
    regs.pc = 0x0028; }

void CPU::ldh_a_n(uint8_t n) { // 0xF0
    regs.a = memory.read8(0xFF00 + n); }

void CPU::pop_af() { // 0xF1
    regs.af = popFromStack();
    regs.f &= 0xf0; }

void CPU::ld_a_cp() { // 0xF2
    regs.a = memory.read8(0xFF00 + regs.c); }

void CPU::di() { //0xF3
    memory.IME = false; }

void CPU::push_af() { // 0xF5
    pushToStack(regs.af); }

void CPU::or_n(uint8_t n) { // 0xF6
    _or(n); }

void CPU::rst_30() { // 0xF7
    pushToStack(regs.pc);
    regs.pc = 0x0030; }

void CPU::ld_hl_spn(uint8_t n) { // 0xF8
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    uint32_t sum = regs.sp + int8_t(n);
    if((regs.sp & 0xFF) + (n & 0xFF) > 0x000000FF)
        regs.setFlag(CARRY_FLAG, 1);

    if (((regs.sp & 0x0F) + (n & 0x0F)) > 0x0F)
        regs.setFlag(HALF_CARRY_FLAG, 1);

    regs.hl = sum & 0x0000FFFF;
}

void CPU::ld_sp_hl() { // 0xF9
    regs.sp = regs.hl; }

void CPU::ld_a_nnp(uint16_t nn) { // 0xFA
    regs.a = memory.read8(nn); }

void CPU::ei() { // 0xFB
    memory.IME = true; }

void CPU::cp_n(uint8_t n) { // 0xFE
    cp(n); }

void CPU::rst_38() { // 0xFF
    pushToStack(regs.pc);
    regs.pc = 0x0038; }


// CB instructions

uint8_t CPU::rlc(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    bool carry = (r >= 0x80);
    if (carry)
        regs.setFlag(CARRY_FLAG, 1);
    r = r << 1;
    r += uint8_t(carry);
    regs.setFlag(ZERO_FLAG, r == 0);
    return r;
}

uint8_t CPU::rrc(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    int carry = (r & 0x01) ? 1 : 0;
    if (carry)
        regs.setFlag(CARRY_FLAG, 1);
    r = r >> 1;
    r |= (carry << 7);
    regs.setFlag(ZERO_FLAG, !r);
    return r;
}
uint8_t CPU::rl(uint8_t& r) {
    int carry = (regs.checkFlagSet(CARRY_FLAG)) ? 1 : 0;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (r >= 0x80)
        regs.setFlag(CARRY_FLAG, 1);
    r = r << 1;
    r += uint8_t(carry);
    regs.setFlag(ZERO_FLAG, !r);
    return r;
}

uint8_t CPU::rr(uint8_t& r) {
    int carry = regs.checkFlagSet(CARRY_FLAG);
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if ((r & 0x01) == 1)
        regs.setFlag(CARRY_FLAG, 1);
    r = (r >> 1) | (carry << 7);
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
    return r;
}

uint8_t CPU::sla(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (r >= 0x80)
        regs.setFlag(CARRY_FLAG, 1);
    r = r << 1;
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
    return r;
}

uint8_t CPU::sra(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if ((r & 0x01) == 1)
        regs.setFlag(CARRY_FLAG, 1);
    uint8_t copy = r & 0x80;
    r = r >> 1;
    r += copy;
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
    return r;
}

uint8_t CPU::swap(uint8_t& r) {
    uint8_t msb = (r & 0xF0) >> 4;
    uint8_t lsb = r & 0x0F;
    r = (lsb << 4) + msb;
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
    return r;
}

uint8_t CPU::srl(uint8_t& r) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, 0);
    if (r & 0x01)
        regs.setFlag(CARRY_FLAG, 1);
    r = r >> 1;
    if (r == 0)
        regs.setFlag(ZERO_FLAG, 1);
    return r;
}

void CPU::bit(uint8_t r, int b) {
    regs.setFlag(ZERO_FLAG | SUB_FLAG, 0);
    regs.setFlag(HALF_CARRY_FLAG, 1);
    if ((r & (1 << b)) == 0)
        regs.setFlag(ZERO_FLAG, 1);
}

void CPU::res8(uint8_t& r, int b) {
    r &= ~(1 << b);
}

uint8_t CPU::set(uint8_t& r, int b) {
    r |= 1 << b;
    return r;
}

void CPU::rlc_b() { // 0x00
    rlc(regs.b); }

void CPU::rlc_c() { // 0x01
    rlc(regs.c); }

void CPU::rlc_d() { // 0x02
    rlc(regs.d); }

void CPU::rlc_e() { // 0x03
    rlc(regs.e); }

void CPU::rlc_h() { // 0x04
    rlc(regs.h); }

void CPU::rlc_l() { // 0x05
    rlc(regs.l); }

void CPU::rlc_hlp() { // 0x06
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, rlc(r)); }

void CPU::rlc_a() { // 0x07
    rlc(regs.a); }

void CPU::rrc_b() { // 0x08
    rrc(regs.b); }

void CPU::rrc_c() { // 0x09
    rrc(regs.c); }

void CPU::rrc_d() { // 0x0A
    rrc(regs.d); }

void CPU::rrc_e() { // 0x0B
    rrc(regs.e); }

void CPU::rrc_h() { // 0x0C
    rrc(regs.h); }

void CPU::rrc_l() { // 0x0D
    rrc(regs.l); }

void CPU::rrc_hlp() { // 0x0E
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, rrc(r)); }

void CPU::rrc_a() { // 0x0F
    rrc(regs.a); }

void CPU::rl_b() { // 0x10
    rl(regs.b); }

void CPU::rl_c() { // 0x11
    rl(regs.c); }

void CPU::rl_d() { // 0x12
    rl(regs.d); }

void CPU::rl_e() { // 0x13
    rl(regs.e); }

void CPU::rl_h() { // 0x14
    rl(regs.h); }

void CPU::rl_l() { // 0x15
    rl(regs.l); }

void CPU::rl_hlp() { // 0x16
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, rl(r)); }

void CPU::rl_a() { // 0x17
    rl(regs.a); }

void CPU::rr_b() { // 0x18
    rr(regs.b); }

void CPU::rr_c() { // 0x19
    rr(regs.c); }

void CPU::rr_d() { // 0x1A
    rr(regs.d); }

void CPU::rr_e() { // 0x1B
    rr(regs.e); }

void CPU::rr_h() { // 0x1C
    rr(regs.h); }

void CPU::rr_l() { // 0x1D
    rr(regs.l); }

void CPU::rr_hlp() { // 0x1E
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, rr(r)); }

void CPU::rr_a() { // 0x1F
    rr(regs.a); }

void CPU::sla_b() { // 0x20
    sla(regs.b); }

void CPU::sla_c() { // 0x21
    sla(regs.c); }

void CPU::sla_d() { // 0x22
    sla(regs.d); }

void CPU::sla_e() { // 0x23
    sla(regs.e); }

void CPU::sla_h() { // 0x24
    sla(regs.h); }

void CPU::sla_l() { // 0x25
    sla(regs.l); }

void CPU::sla_hlp() { // 0x26
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, sla(r)) ;}

void CPU::sla_a() { // 0x27
    sla(regs.a); }

void CPU::sra_b() { // 0x28
    sra(regs.b); }

void CPU::sra_c() { // 0x29
    sra(regs.c); }

void CPU::sra_d() { // 0x2A
    sra(regs.d); }

void CPU::sra_e() { // 0x2B
    sra(regs.e); }

void CPU::sra_h() { // 0x2C
    sra(regs.h); }

void CPU::sra_l() { // 0x2D
    sra(regs.l); }

void CPU::sra_hlp() { // 0x2E
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, sra(r)) ; }

void CPU::sra_a() { // 0x2F
    sra(regs.a); }

void CPU::swap_b() { // 0x30
    swap(regs.b); }

void CPU::swap_c() { // 0x31
    swap(regs.c); }

void CPU::swap_d() { // 0x32
    swap(regs.d); }

void CPU::swap_e() { // 0x33
    swap(regs.e); }

void CPU::swap_h() { // 0x34
    swap(regs.h); }

void CPU::swap_l() { // 0x35
    swap(regs.l); }

void CPU::swap_hlp() { // 0x36
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, swap(r)); }

void CPU::swap_a() { // 0x37
    swap(regs.a); }

void CPU::srl_b() { // 0x38
    srl(regs.b); }

void CPU::srl_c() { // 0x39
    srl(regs.c); }

void CPU::srl_d() { // 0x3A
    srl(regs.d); }

void CPU::srl_e() { // 0x3B
    srl(regs.e); }

void CPU::srl_h() { // 0x3C
    srl(regs.h); }

void CPU::srl_l() { // 0x3D
    srl(regs.l); }

void CPU::srl_hlp() { // 0x3E
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, srl(r)); }

void CPU::srl_a() { // 0x3F
    srl(regs.a); }

void CPU::bit_0_b() { // 0x40
    bit(regs.b, 0); }

void CPU::bit_0_c() { // 0x41
    bit(regs.c, 0); }

void CPU::bit_0_d() { // 0x42
    bit(regs.d, 0); }

void CPU::bit_0_e() { // 0x43
    bit(regs.e, 0); }

void CPU::bit_0_h() { // 0x44
    bit(regs.h, 0); }

void CPU::bit_0_l() { // 0x45
    bit(regs.l, 0); }

void CPU::bit_0_hlp() { // 0x46
    bit(memory.read8(regs.hl), 0); }

void CPU::bit_0_a() { // 0x47
    bit(regs.a, 0); }

void CPU::bit_1_b() { // 0x48
    bit(regs.b, 1); }

void CPU::bit_1_c() { // 0x49
    bit(regs.c, 1); }

void CPU::bit_1_d() { // 0x4A
    bit(regs.d, 1); }

void CPU::bit_1_e() { // 0x4B
    bit(regs.e, 1); }

void CPU::bit_1_h() { // 0x4C
    bit(regs.h, 1); }

void CPU::bit_1_l() { // 0x4D
    bit(regs.l, 1); }

void CPU::bit_1_hlp() { // 0x4E
    bit(memory.read8(regs.hl), 1); }

void CPU::bit_1_a() { // 0x4F
    bit(regs.a, 1); }

void CPU::bit_2_b() { // 0x50
    bit(regs.b, 2); }

void CPU::bit_2_c() { // 0x51
    bit(regs.c, 2); }

void CPU::bit_2_d() { // 0x52
    bit(regs.d, 2); }

void CPU::bit_2_e() { // 0x53
    bit(regs.e, 2); }

void CPU::bit_2_h() { // 0x54
    bit(regs.h, 2); }

void CPU::bit_2_l() { // 0x55
    bit(regs.l, 2); }

void CPU::bit_2_hlp() { // 0x56
    bit(memory.read8(regs.hl), 2); }

void CPU::bit_2_a() { // 0x57
    bit(regs.a, 2); }

void CPU::bit_3_b() { // 0x58
    bit(regs.b, 3); }

void CPU::bit_3_c() { // 0x59
    bit(regs.c, 3); }

void CPU::bit_3_d() { // 0x5A
    bit(regs.d, 3); }

void CPU::bit_3_e() { // 0x5B
    bit(regs.e, 3); }

void CPU::bit_3_h() { // 0x5C
    bit(regs.h, 3); }

void CPU::bit_3_l() { // 0x5D
    bit(regs.l, 3); }

void CPU::bit_3_hlp() { // 0x5E
    bit(memory.read8(regs.hl), 3); }

void CPU::bit_3_a() { // 0x5F
    bit(regs.a, 3); }

void CPU::bit_4_b() { // 0x60
    bit(regs.b, 4); }

void CPU::bit_4_c() { // 0x61
    bit(regs.c, 4); }

void CPU::bit_4_d() { // 0x62
    bit(regs.d, 4); }

void CPU::bit_4_e() { // 0x63
    bit(regs.e, 4); }

void CPU::bit_4_h() { // 0x64
    bit(regs.h, 4); }

void CPU::bit_4_l() { // 0x65
    bit(regs.l, 4); }

void CPU::bit_4_hlp() { // 0x66
    bit(memory.read8(regs.hl), 4); }

void CPU::bit_4_a() { // 0x67
    bit(regs.a, 4); }

void CPU::bit_5_b() { // 0x68
    bit(regs.b, 5); }

void CPU::bit_5_c() { // 0x69
    bit(regs.c, 5); }

void CPU::bit_5_d() { // 0x6A
    bit(regs.d, 5); }

void CPU::bit_5_e() { // 0x6B
    bit(regs.e, 5); }

void CPU::bit_5_h() { // 0x6C
    bit(regs.h, 5); }

void CPU::bit_5_l() { // 0x6D
    bit(regs.l, 5); }

void CPU::bit_5_hlp() { // 0x6E
    bit(memory.read8(regs.hl), 5); }

void CPU::bit_5_a() { // 0x6F
    bit(regs.a, 5); }

void CPU::bit_6_b() { // 0x70
    bit(regs.b, 6); }

void CPU::bit_6_c() { // 0x71
    bit(regs.c, 6); }

void CPU::bit_6_d() { // 0x72
    bit(regs.d, 6); }

void CPU::bit_6_e() { // 0x73
    bit(regs.e, 6); }

void CPU::bit_6_h() { // 0x74
    bit(regs.h, 6); }

void CPU::bit_6_l() { // 0x75
    bit(regs.l, 6); }

void CPU::bit_6_hlp() { // 0x76
    bit(memory.read8(regs.hl), 6); }

void CPU::bit_6_a() { // 0x77
    bit(regs.a, 6); }

void CPU::bit_7_b() { // 0x78
    bit(regs.b, 7); }

void CPU::bit_7_c() { // 0x79
    bit(regs.c, 7); }

void CPU::bit_7_d() { // 0x7A
    bit(regs.d, 7); }

void CPU::bit_7_e() { // 0x7B
    bit(regs.e, 7); }

void CPU::bit_7_h() { // 0x7C
    bit(regs.h, 7); }

void CPU::bit_7_l() { // 0x7D
    bit(regs.l, 7); }

void CPU::bit_7_hlp() { // 0x7E
    bit(memory.read8(regs.hl), 7); }

void CPU::bit_7_a() { // 0x7F
    bit(regs.a, 7); }

void CPU::res_0_b() { // 0x80
    res8(regs.b, 0); }
    
void CPU::res_0_c() { // 0x81
    res8(regs.c, 0); }
    
void CPU::res_0_d() { // 0x82
    res8(regs.d, 0); }
    
void CPU::res_0_e() { // 0x83
    res8(regs.e, 0); }
    
void CPU::res_0_h() { // 0x84
    res8(regs.h, 0); }
    
void CPU::res_0_l() { // 0x85
    res8(regs.l, 0); }
    
void CPU::res_0_hlp() { // 0x86
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 0)); }
    
void CPU::res_0_a() { // 0x87
    res8(regs.a, 0); }
    
void CPU::res_1_b() { // 0x88
    res8(regs.b, 1); }
    
void CPU::res_1_c() { // 0x89
    res8(regs.c, 1); }
    
void CPU::res_1_d() { // 0x8A
    res8(regs.d, 1); }
    
void CPU::res_1_e() { // 0x8B
    res8(regs.e, 1); }
    
void CPU::res_1_h() { // 0x8C
    res8(regs.h, 1); }
    
void CPU::res_1_l() { // 0x8D
    res8(regs.l, 1); }
    
void CPU::res_1_hlp() { // 0x8E
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 1)); }
    
void CPU::res_1_a() { // 0x8F
    res8(regs.a, 1); }
    
void CPU::res_2_b() { // 0x90
    res8(regs.b, 2); }
    
void CPU::res_2_c() { // 0x91
    res8(regs.c, 2); }
    
void CPU::res_2_d() { // 0x92
    res8(regs.d, 2); }
    
void CPU::res_2_e() { // 0x93
    res8(regs.e, 2); }
    
void CPU::res_2_h() { // 0x94
    res8(regs.h, 2); }
    
void CPU::res_2_l() { // 0x95
    res8(regs.l, 2); }
    
void CPU::res_2_hlp() { // 0x96
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 2)); }
    
void CPU::res_2_a() { // 0x97
    res8(regs.a, 2); }
    
void CPU::res_3_b() { // 0x98
    res8(regs.b, 3); }
    
void CPU::res_3_c() { // 0x99
    res8(regs.c, 3); }
    
void CPU::res_3_d() { // 0x9A
    res8(regs.d, 3); }
    
void CPU::res_3_e() { // 0x9B
    res8(regs.e, 3); }
    
void CPU::res_3_h() { // 0x9C
    res8(regs.h, 3); }
    
void CPU::res_3_l() { // 0x9D
    res8(regs.l, 3); }
    
void CPU::res_3_hlp() { // 0x9E
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 3)); }
    
void CPU::res_3_a() { // 0x9F
    res8(regs.a, 3); }
    
void CPU::res_4_b() { // 0xA0
    res8(regs.b, 4); }
    
void CPU::res_4_c() { // 0xA1
    res8(regs.c, 4); }
    
void CPU::res_4_d() { // 0xA2
    res8(regs.d, 4); }
    
void CPU::res_4_e() { // 0xA3
    res8(regs.e, 4); }
    
void CPU::res_4_h() { // 0xA4
    res8(regs.h, 4); }
    
void CPU::res_4_l() { // 0xA5
    res8(regs.l, 4); }
    
void CPU::res_4_hlp() { // 0xA6
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 4)); }
    
void CPU::res_4_a() { // 0xA7
    res8(regs.a, 4); }
    
void CPU::res_5_b() { // 0xA8
    res8(regs.b, 5); }
    
void CPU::res_5_c() { // 0xA9
    res8(regs.c, 5); }
    
void CPU::res_5_d() { // 0xAA
    res8(regs.d, 5); }
    
void CPU::res_5_e() { // 0xAB
    res8(regs.e, 5); }
    
void CPU::res_5_h() { // 0xAC
    res8(regs.h, 5); }
    
void CPU::res_5_l() { // 0xAD
    res8(regs.l, 5); }
    
void CPU::res_5_hlp() { // 0xAE
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 5)); }
    
void CPU::res_5_a() { // 0xAF
    res8(regs.a, 5); }
    
void CPU::res_6_b() { // 0xB0
    res8(regs.b, 6); }
    
void CPU::res_6_c() { // 0xB1
    res8(regs.c, 6); }
    
void CPU::res_6_d() { // 0xB2
    res8(regs.d, 6); }
    
void CPU::res_6_e() { // 0xB3
    res8(regs.e, 6); }
    
void CPU::res_6_h() { // 0xB4
    res8(regs.h, 6); }
    
void CPU::res_6_l() { // 0xB5
    res8(regs.l, 6); }
    
void CPU::res_6_hlp() { // 0xB6
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 6)); }
    
void CPU::res_6_a() { // 0xB7
    res8(regs.a, 6); }
    
void CPU::res_7_b() { // 0xB8
    res8(regs.b, 7); }
    
void CPU::res_7_c() { // 0xB9
    res8(regs.c, 7); }
    
void CPU::res_7_d() { // 0xBA
    res8(regs.d, 7); }
    
void CPU::res_7_e() { // 0xBB
    res8(regs.e, 7); }
    
void CPU::res_7_h() { // 0xBC
    res8(regs.h, 7); }
    
void CPU::res_7_l() { // 0xBD
    res8(regs.l, 7); }
    
void CPU::res_7_hlp() { // 0xBE
    memory.write8(regs.hl, memory.read8(regs.hl) & ~(1 << 7)); }
    
void CPU::res_7_a() { // 0xBF
    res8(regs.a, 7); }

void CPU::set_0_b() { // 0xC0
    set(regs.b, 0); }

void CPU::set_0_c() { // 0xC1
    set(regs.c, 0); }

void CPU::set_0_d() { // 0xC2
    set(regs.d, 0); }

void CPU::set_0_e() { // 0xC3
    set(regs.e, 0); }

void CPU::set_0_h() { // 0xC4
    set(regs.h, 0); }

void CPU::set_0_l() { // 0xC5
    set(regs.l, 0); }

void CPU::set_0_hlp() { // 0xC6
    memory.write8(regs.hl, memory.read8(regs.hl) | (1 << 0)); }

void CPU::set_0_a() { // 0xC7
    set(regs.a, 0); }

void CPU::set_1_b() { // 0xC8
    set(regs.b, 1); }

void CPU::set_1_c() { // 0xC9
    set(regs.c, 1); }

void CPU::set_1_d() { // 0xCA
    set(regs.d, 1); }

void CPU::set_1_e() { // 0xCB
    set(regs.e, 1); }

void CPU::set_1_h() { // 0xCC
    set(regs.h, 1); }

void CPU::set_1_l() { // 0xCD
    set(regs.l, 1); }

void CPU::set_1_hlp() { // 0xCE
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 1)); }

void CPU::set_1_a() { // 0xCF
    set(regs.a, 1); }

void CPU::set_2_b() { // 0xD0
    set(regs.b, 2); }

void CPU::set_2_c() { // 0xD1
    set(regs.c, 2); }

void CPU::set_2_d() { // 0xD2
    set(regs.d, 2); }

void CPU::set_2_e() { // 0xD3
    set(regs.e, 2); }

void CPU::set_2_h() { // 0xD4
    set(regs.h, 2); }

void CPU::set_2_l() { // 0xD5
    set(regs.l, 2); }

void CPU::set_2_hlp() { // 0xD6
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 2)); }

void CPU::set_2_a() { // 0xD7
    set(regs.a, 2); }

void CPU::set_3_b() { // 0xD8
    set(regs.b, 3); }

void CPU::set_3_c() { // 0xD9
    set(regs.c, 3); }

void CPU::set_3_d() { // 0xDA
    set(regs.d, 3); }

void CPU::set_3_e() { // 0xDB
    set(regs.e, 3); }

void CPU::set_3_h() { // 0xDC
    set(regs.h, 3); }

void CPU::set_3_l() { // 0xDD
    set(regs.l, 3); }

void CPU::set_3_hlp() { // 0xDE
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 3)); }

void CPU::set_3_a() { // 0xDF
    set(regs.a, 3); }

void CPU::set_4_b() { // 0xE0
    set(regs.b, 4); }

void CPU::set_4_c() { // 0xE1
    set(regs.c, 4); }

void CPU::set_4_d() { // 0xE2
    set(regs.d, 4); }

void CPU::set_4_e() { // 0xE3
    set(regs.e, 4); }

void CPU::set_4_h() { // 0xE4
    set(regs.h, 4); }

void CPU::set_4_l() { // 0xE5
    set(regs.l, 4); }

void CPU::set_4_hlp() { // 0xE6
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 4)); }

void CPU::set_4_a() { // 0xE7
    set(regs.a, 4); }

void CPU::set_5_b() { // 0xE8
    set(regs.b, 5); }

void CPU::set_5_c() { // 0xE9
    set(regs.c, 5); }

void CPU::set_5_d() { // 0xEA
    set(regs.d, 5); }

void CPU::set_5_e() { // 0xEB
    set(regs.e, 5); }

void CPU::set_5_h() { // 0xEC
    set(regs.h, 5); }

void CPU::set_5_l() { // 0xED
    set(regs.l, 5); }

void CPU::set_5_hlp() { // 0xEE
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 5)); }

void CPU::set_5_a() { // 0xEF
    set(regs.a, 5); }

void CPU::set_6_b() { // 0xF0
    set(regs.b, 6); }

void CPU::set_6_c() { // 0xF1
    set(regs.c, 6); }

void CPU::set_6_d() { // 0xF2
    set(regs.d, 6); }

void CPU::set_6_e() { // 0xF3
    set(regs.e, 6); }

void CPU::set_6_h() { // 0xF4
    set(regs.h, 6); }

void CPU::set_6_l() { // 0xF5
    set(regs.l, 6); }

void CPU::set_6_hlp() { // 0xF6
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 6)); }

void CPU::set_6_a() { // 0xF7
    set(regs.a, 6); }

void CPU::set_7_b() { // 0xF8
    set(regs.b, 7); }

void CPU::set_7_c() { // 0xF9
    set(regs.c, 7); }

void CPU::set_7_d() { // 0xFA
    set(regs.d, 7); }

void CPU::set_7_e() { // 0xFB
    set(regs.e, 7); }

void CPU::set_7_h() { // 0xFC
    set(regs.h, 7); }

void CPU::set_7_l() { // 0xFD
    set(regs.l, 7); }

void CPU::set_7_hlp() { // 0xFE
    uint8_t r = memory.read8(regs.hl);
    memory.write8(regs.hl, set(r, 7)); }

void CPU::set_7_a() { // 0xFF
    set(regs.a, 7); }
