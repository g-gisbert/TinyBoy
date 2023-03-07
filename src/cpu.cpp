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

    uint8_t opcode = memory.cart.readCart(regs.pc++);

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
    std::cout << "PC : " << std::hex << regs.pc << std::endl;
    return cycles;
}

void CPU::showState() {
    std::cout << "### Registers ###" << std::endl;
    std::cout << "af=" << std::hex << regs.af << std::endl;
    std::cout << "bc=" << std::hex << regs.bc << std::endl;
    std::cout << "de=" << std::hex << regs.de << std::endl;
    std::cout << "hl=" << std::hex << regs.hl << std::endl;
    std::cout << "sp=" << std::hex << regs.sp << std::endl;
    std::cout << "PC : " << regs.pc << std::endl;
}

// Helpers
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
    std::cout << "r : " << int(r) << std::endl;
    std::cout << "a : " << int(regs.a) << std::endl;
    regs.a &= r;
    std::cout << "a : " << int(regs.a) << std::endl;
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

void CPU::ld_d_n(uint8_t n) { // 0x16
    regs.d = n;}

void CPU::add_hl_de() { // 0x19
    add16(regs.de); }

void CPU::ld_a_dep() { // 0x1A
    regs.a = memory.read8(regs.de); }

void CPU::jr_nz_n(uint8_t n) { // 0x20
    if (regs.checkFlagClear(ZERO_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8;
}

void CPU::ld_hl_nn(uint16_t nn) { // 0x21
    regs.hl = nn; }

void CPU::inc_hl() { // 0x23
    regs.hl++; }

void CPU::jr_z_n(uint8_t n) { //0x28
    if (regs.checkFlagSet(ZERO_FLAG)) {
        regs.pc += int8_t(n);
        cycles += 12;
    } else
        cycles += 8;
}

void CPU::ldi_a_hl() { //0x2A
    regs.a = memory.read8(regs.hl++); }

void CPU::cpl() { // 0x2F
    regs.setFlag(SUB_FLAG | HALF_CARRY_FLAG, 1);
    regs.a = ~regs.a; }

void CPU::ld_sp_nn(uint16_t nn) { // 0x31
    regs.sp = nn; }

void CPU::ld_hlpm_a() { // 0x32
    memory.write8(regs.hl, regs.a); regs.hl--; }

void CPU::ld_hlp_n(uint8_t n) { // 0x36
    memory.write8(regs.hl, n); }

void CPU::ld_a_n(uint8_t n) { // 0x3E
    regs.a = n; }

void CPU::ld_b_a() { // 0x47
    regs.b = regs.a; }

void CPU::ld_c_a() { // 0x4F
    regs.c = regs.a; }

void CPU::ld_d_hlp() { // 0x56
    regs.d = memory.read8(regs.hl); }

void CPU::ld_e_hlp() { // 0x5E
    regs.e = memory.read8(regs.hl); }

void CPU::ld_e_a() { // 0x5F
    regs.e = regs.a; }

void CPU::ld_a_b() { //0x78
    regs.a = regs.b; }

void CPU::ld_a_c() { // 0x79
    regs.a = regs.c; }

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

void CPU::jp_nn(uint16_t nn) { // 0xC3
    regs.pc = nn; }

void CPU::ret() { //0xC9
    uint8_t lsb = stack[regs.sp];
    uint8_t msb = stack[regs.sp+1];
    regs.pc = (uint16_t(msb) << 8) + lsb;
    regs.sp +=2; }

void CPU::cb_manager(uint8_t n) { // 0xCB
    Instruction instr = CB_instructions[n];
    (*this.*instr.funcCallVoid)();
    cycles += instr.cycles; }

void CPU::call_nn(uint16_t nn) { // 0xCD
    uint8_t msb = (regs.pc & 0xFF00) >> 8;
    uint8_t lsb = regs.pc & 0xFF;
    stack[regs.sp-1] = msb;
    stack[regs.sp-2] = lsb;
    regs.pc = nn;
    regs.sp -= 2; }

void CPU::push_de() { // 0xD5
    stack[regs.sp-1] = regs.d;
    stack[regs.sp-2] = regs.e;
    regs.sp -= 2;
}

void CPU::ldh_n_a(uint8_t n) { // 0xE0
    memory.write8(0xFF00 + n, regs.a); }

void CPU::pop_hl() { // 0xE1
    uint8_t lsb = stack[regs.sp];
    uint8_t msb = stack[regs.sp+1];
    regs.hl = (uint16_t(msb) << 8) + lsb;
    regs.sp +=2; }

void CPU::ld_cp_a(uint8_t n) { // 0xE2
    memory.write8(0xFF00 + regs.c, regs.a); }

void CPU::push_hl() { // 0xE5
    stack[regs.sp-1] = regs.h;
    stack[regs.sp-2] = regs.l;
    regs.sp -= 2;}

void CPU::and_n(uint8_t n) { // 0xE6
    _and(n); }

void CPU::jp_hlp() { // 0xE9
    regs.pc = regs.hl; }

void CPU::ld_nnp_a(uint16_t nn) { // 0xEA
    memory.write8(nn, regs.a); }

void CPU::rst_28() { // 0xEF
    uint8_t msb = (regs.pc & 0xFF00) >> 8;
    uint8_t lsb = regs.pc & 0xFF;
    stack[regs.sp-1] = msb;
    stack[regs.sp-2] = lsb;
    regs.sp -= 2;
    regs.pc = 0x0028;
}

void CPU::ldh_a_n(uint8_t n) { // 0xF0
    regs.a = memory.read8(0xFF00 + n); }

void CPU::di() { //0xF3
    memory.IME = false; }

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