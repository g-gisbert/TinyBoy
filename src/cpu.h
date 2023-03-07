#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "registers.h"
#include "memory.h"
#include <cstdio>
#include <iostream>
#include <cstring>

class CPU {
public:
    CPU(Memory& memo);
    void initMemory();
    int& step();
    void showState();

    // Memory
    Registers regs;
    uint8_t stack[0xFFFF];
    Memory& memory;

    int cycles;

    bool debug;

    // Instructions
    struct Instruction {

        const char* name;
        uint8_t byteLength;
        union {
            void (CPU::*funcCallVoid)();
            void (CPU::*funcCall8)(uint8_t);
            void (CPU::*funcCall16)(uint16_t);
        };
        int cycles;
    };

    // Helpers
    void add8(uint8_t r);
    void add16(uint16_t rr);
    void inc(uint8_t& r);
    void dec(uint8_t& r);
    void _and(uint8_t r);
    void _or(uint8_t r);
    void _xor(uint8_t r);
    void cp(uint8_t val);

    void swap(uint8_t& r);
    void res8(uint8_t& r, int b);

    // Instructions
    void nop(); // 0x00
    void ld_bc_nn(uint16_t nn); // 0x01
    void dec_b(); // 0x05
    void ld_b_n(uint8_t n); // 0x06
    void dec_bc(); // 0x0B
    void dec_c(); // 0x0D
    void ld_c_n(uint8_t n); // 0x0E
    void ld_de_nn(uint16_t nn); // 0x11
    void ld_dep_a(); // 0x12
    void inc_de(); // 0x13
    void ld_d_n(uint8_t n); // 0x16
    void add_hl_de(); // 0x19
    void ld_a_dep(); // 0x1A
    void jr_nz_n(uint8_t n); // 0x20
    void ld_hl_nn(uint16_t nn); // 0x21
    void inc_hl(); // 0x23
    void jr_z_n(uint8_t n); // 0x28
    void ldi_a_hl(); // 0x2A
    void cpl(); // 0x2F
    void ld_sp_nn(uint16_t nn); // 0x31
    void ld_hlpm_a(); // 0x32
    void ld_hlp_n(uint8_t n); // 0x36
    void ld_a_n(uint8_t n); // 0x3E
    void ld_b_a(); // 0x47
    void ld_c_a(); // 0x4F
    void ld_d_hlp(); // 0x56
    void ld_e_hlp(); // 0x5E
    void ld_e_a(); // 0x5F
    void ld_a_b(); // 0x78
    void ld_a_c(); // 0x79
    void add_a(); // 0x87
    void and_c(); // 0xA1
    void and_a(); // 0xA7
    void xor_c(); // 0xA9
    void xor_a(); // 0xAF
    void or_b(); // 0xB0
    void or_c(); // 0xB1
    void jp_nn(uint16_t nn); // 0xC3
    void ret(); //0xC9
    void cb_manager(uint8_t n); // 0xCB
    void call_nn(uint16_t nn); // 0xCD
    void push_de(); // 0xD5
    void ldh_n_a(uint8_t n); // 0xE0
    void pop_hl(); // 0xE1
    void ld_cp_a(uint8_t n); // 0xE2
    void push_hl(); // 0xE5
    void and_n(uint8_t n); // 0xE6
    void jp_hlp(); // 0xE9
    void ld_nnp_a(uint16_t nn); // 0xEA
    void rst_28(); // 0xEF
    void ldh_a_n(uint8_t n); // 0xF0
    void di(); // 0xF3
    void ei(); // 0xFB
    void cp_n(uint8_t n); // 0xFE

    // CB instructions
    void swap_a(); // 0x37
    void res_0_a(); // 0x87

    constexpr static Instruction instructions_set[256] = {
            {.name="NOP", .byteLength=1, .funcCallVoid=&CPU::nop, .cycles=4},                // 0x00
            {.name="LD BC, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_bc_nn, .cycles=12},   // 0x01
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x03
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x02
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x04
            {.name="DEC B", .byteLength=1, .funcCallVoid=&CPU::dec_b, .cycles=4},// 0x05
            {.name="LD B, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_b_n, .cycles=8},// 0x06
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x07
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x08
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x09
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x0A
            {.name="DEC BC", .byteLength=1, .funcCallVoid=&CPU::dec_bc, .cycles=8},// 0x0B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x0C
            {.name="DEC C", .byteLength=1, .funcCallVoid=&CPU::dec_c, .cycles=4},// 0x0D
            {.name="LD C, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_c_n, .cycles=8},// 0x0E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x0F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x10
            {.name="LD DE, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_de_nn, .cycles=12},// 0x11
            {.name="LD (DE), A", .byteLength=1, .funcCallVoid=&CPU::ld_dep_a, .cycles=8},// 0x12
            {.name="INC DE", .byteLength=1, .funcCallVoid=&CPU::inc_de, .cycles=8},// 0x13
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x14
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x15
            {.name="LD D, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_d_n, .cycles=8},// 0x16
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x17
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x18
            {.name="ADD HL, DE", .byteLength=1, .funcCallVoid=&CPU::add_hl_de, .cycles=8},// 0x19
            {.name="LD A, (DE)", .byteLength=1, .funcCallVoid=&CPU::ld_a_dep, .cycles=8},// 0x1A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1F
            {.name="JR NZ, 0x%02X", .byteLength=2, .funcCall8=&CPU::jr_nz_n, .cycles=0},// 0x20
            {.name="LD HL, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_hl_nn, .cycles=12},// 0x21
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x22
            {.name="INC HL", .byteLength=1, .funcCallVoid=&CPU::inc_hl, .cycles=8},// 0x23
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x24
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x25
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x26
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x27
            {.name="JR Z, 0x02X", .byteLength=2, .funcCall8=&CPU::jr_z_n, .cycles=0},// 0x28
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x29
            {.name="LD A, (HL+)", .byteLength=1, .funcCallVoid=&CPU::ldi_a_hl, .cycles=8},// 0x2A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2E
            {.name="CPL", .byteLength=1, .funcCallVoid=&CPU::cpl, .cycles=4},// 0x2F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x30
            {.name="LD SP, 0x04X", .byteLength=3, .funcCall16=&CPU::ld_sp_nn, .cycles=12},// 0x31
            {.name="LD (HL-), A", .byteLength=1, .funcCallVoid=&CPU::ld_hlpm_a, .cycles=8},// 0x32
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x33
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x34
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x35
            {.name="LD (HL), 0x02X", .byteLength=2, .funcCall8=&CPU::ld_hlp_n, .cycles=12},// 0x36
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x37
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x38
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x39
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3D
            {.name="LD A, 0x02X", .byteLength=2, .funcCall8=&CPU::ld_a_n, .cycles=8},// 0x3E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x40
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x41
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x42
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x43
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x44
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x45
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x46
            {.name="LD B, A", .byteLength=1, .funcCallVoid=&CPU::ld_b_a, .cycles=4},// 0x47
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x48
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x49
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4E
            {.name="LD C, A", .byteLength=1, .funcCallVoid=&CPU::ld_c_a, .cycles=4},// 0x4F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x50
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x51
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x52
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x53
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x54
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x55
            {.name="LD D, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_d_hlp, .cycles=8},// 0x56
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x57
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x58
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x59
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5D
            {.name="LD E, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_e_hlp, .cycles=8},// 0x5E
            {.name="LD E, A", .byteLength=1, .funcCallVoid=&CPU::ld_e_a, .cycles=4},// 0x5F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x60
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x61
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x62
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x63
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x64
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x65
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x66
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x67
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x68
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x69
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x70
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x71
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x72
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x73
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x74
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x75
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x76
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x77
            {.name="LD A, B", .byteLength=1, .funcCallVoid=&CPU::ld_a_b, .cycles=4},// 0x78
            {.name="LD A, C", .byteLength=1, .funcCallVoid=&CPU::ld_a_c, .cycles=4},// 0x79
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x80
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x81
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x82
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x83
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x84
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x85
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x86
            {.name="ADD A, A", .byteLength=1, .funcCallVoid=&CPU::add_a, .cycles=4},// 0x87
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x88
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x89
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x90
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x91
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x92
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x93
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x94
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x95
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x96
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x97
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x98
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x99
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA0
            {.name="AND A, C", .byteLength=1, .funcCallVoid=&CPU::and_c, .cycles=4},// 0xA1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA6
            {.name="AND A, A", .byteLength=1, .funcCallVoid=&CPU::and_a, .cycles=4},// 0xA7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA8
            {.name="XOR A, C", .byteLength=1, .funcCallVoid=&CPU::xor_c, .cycles=4},// 0xA9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAE
            {.name="XOR A, A", .byteLength=1, .funcCallVoid=&CPU::xor_a, .cycles=4},// 0xAF
            {.name="OR A, B", .byteLength=1, .funcCallVoid=&CPU::or_b, .cycles=4},// 0xB0
            {.name="OR A, C", .byteLength=1, .funcCallVoid=&CPU::or_c, .cycles=4},// 0xB1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBF
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC2
            {.name="JP 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_nn, .cycles=16},// 0xC3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC8
            {.name="RET", .byteLength=1, .funcCallVoid=&CPU::ret, .cycles=16},// 0xC9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCA
            {.name="CB 0x%02X", .byteLength=2, .funcCall8=&CPU::cb_manager, .cycles=0},// 0xCB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCC
            {.name="CALL 0x%04X", .byteLength=3, .funcCall16=&CPU::call_nn, .cycles=24},// 0xCD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCF
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD4
            {.name="PUSH DE", .byteLength=1, .funcCallVoid=&CPU::push_de, .cycles=16},// 0xD5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDF
            {.name="LDH 0xFF%02X, A", .byteLength=2, .funcCall8=&CPU::ldh_n_a, .cycles=12},// 0xE0
            {.name="POP HL", .byteLength=1, .funcCallVoid=&CPU::pop_hl, .cycles=12},// 0xE1
            {.name="LD (C), A", .byteLength=2, .funcCall8=&CPU::ld_cp_a, .cycles=8},// 0xE2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE4
            {.name="PUSH HL", .byteLength=1, .funcCallVoid=&CPU::push_hl, .cycles=16},// 0xE5
            {.name="AND A, 0x%02X", .byteLength=2, .funcCall8=&CPU::and_n, .cycles=8},// 0xE6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE8
            {.name="JP (HL)", .byteLength=1, .funcCallVoid=&CPU::jp_hlp, .cycles=4},// 0xE9
            {.name="LD 0x%04X, A", .byteLength=3, .funcCall16=&CPU::ld_nnp_a, .cycles=16},// 0xEA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xED
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEE
            {.name="RST 28", .byteLength=1, .funcCallVoid=&CPU::rst_28, .cycles=16},// 0xEF
            {.name="LDH A, 0xFF%02X", .byteLength=2, .funcCall8=&CPU::ldh_a_n, .cycles=12},// 0xF0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF2
            {.name="DI", .byteLength=1, .funcCallVoid=&CPU::di, .cycles=4},// 0xF3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFA
            {.name="EI", .byteLength=1, .funcCallVoid=&CPU::ei, .cycles=4},// 0xFB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFD
            {.name="CP n", .byteLength=2, .funcCall8=&CPU::cp_n, .cycles=8},// 0xFE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFF
    };

    constexpr static Instruction CB_instructions[256] = {
            {.name="NOP", .byteLength=1, .funcCallVoid=&CPU::nop, .cycles=4},// 0x00
            {.name="LD BC, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_bc_nn, .cycles=12},// 0x01
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x03
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x02
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x04
            {.name="DEC B", .byteLength=1, .funcCallVoid=&CPU::dec_b, .cycles=4},// 0x05
            {.name="LD B, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_b_n, .cycles=8},// 0x06
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x07
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x08
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x09
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x0A
            {.name="DEC BC", .byteLength=1, .funcCallVoid=&CPU::dec_bc, .cycles=8},// 0x0B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x0C
            {.name="DEC C", .byteLength=1, .funcCallVoid=&CPU::dec_c, .cycles=4},// 0x0D
            {.name="LD C, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_c_n, .cycles=8},// 0x0E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x0F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x10
            {.name="LD DE, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_de_nn, .cycles=12},// 0x11
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x12
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x13
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x14
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x15
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x16
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x17
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x18
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x19
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x1F
            {.name="JR NZ, 0x%02X", .byteLength=2, .funcCall8=&CPU::jr_nz_n, .cycles=0},// 0x20
            {.name="LD HL, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_hl_nn, .cycles=12},// 0x21
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x22
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x23
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x24
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x25
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x26
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x27
            {.name="JR Z, 0x02X", .byteLength=2, .funcCall8=&CPU::jr_z_n, .cycles=0},// 0x28
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x29
            {.name="LD A, (HL+)", .byteLength=1, .funcCallVoid=&CPU::ldi_a_hl, .cycles=8},// 0x2A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x2E
            {.name="CPL", .byteLength=1, .funcCallVoid=&CPU::cpl, .cycles=4},// 0x2F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x30
            {.name="LD SP, 0x04X", .byteLength=3, .funcCall16=&CPU::ld_sp_nn, .cycles=12},// 0x31
            {.name="LD (HL-), A", .byteLength=1, .funcCallVoid=&CPU::ld_hlpm_a, .cycles=8},// 0x32
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x33
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x34
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x35
            {.name="LD (HL), 0x02X", .byteLength=2, .funcCall8=&CPU::ld_hlp_n, .cycles=12},// 0x36
            {.name="SWAP A", .byteLength=1, .funcCallVoid=&CPU::swap_a, .cycles=8},// 0x37
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x38
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x39
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3D
            {.name="LD A, 0x02X", .byteLength=2, .funcCall8=&CPU::ld_a_n, .cycles=8},// 0x3E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x3F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x40
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x41
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x42
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x43
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x44
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x45
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x46
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x47
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x48
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x49
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x4F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x50
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x51
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x52
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x53
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x54
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x55
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x56
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x57
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x58
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x59
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x5F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x60
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x61
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x62
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x63
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x64
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x65
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x66
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x67
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x68
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x69
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x6F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x70
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x71
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x72
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x73
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x74
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x75
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x76
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x77
            {.name="RES 0, A", .byteLength=1, .funcCallVoid=&CPU::ld_a_b, .cycles=4},// 0x78
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x79
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x7F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x80
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x81
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x82
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x83
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x84
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x85
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x86
            {.name="???", .byteLength=1, .funcCallVoid=&CPU::res_0_a, .cycles=8},// 0x87
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x88
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x89
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x8F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x90
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x91
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x92
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x93
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x94
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x95
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x96
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x97
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x98
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x99
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9A
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9B
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9C
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9D
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9E
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0x9F
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA6
            {.name="AND A, A", .byteLength=1, .funcCallVoid=&CPU::and_a, .cycles=4},// 0xA7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xA9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xAE
            {.name="XOR A, A", .byteLength=1, .funcCallVoid=&CPU::xor_a, .cycles=4},// 0xAF
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB0
            {.name="OR A, C", .byteLength=1, .funcCallVoid=&CPU::or_c, .cycles=4},// 0xB1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xB9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xBF
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC2
            {.name="JP 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_nn, .cycles=16},// 0xC3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xC8
            {.name="RET", .byteLength=1, .funcCallVoid=&CPU::ret, .cycles=16},// 0xC9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCA
            {.name="CB 0x%02X", .byteLength=2, .funcCall8=&CPU::cb_manager, .cycles=0},// 0xCB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCC
            {.name="CALL 0x%04X", .byteLength=3, .funcCall16=&CPU::call_nn, .cycles=24},// 0xCD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xCF
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDD
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDF
            {.name="LDH 0xFF%02X, A", .byteLength=2, .funcCall8=&CPU::ldh_n_a, .cycles=12},// 0xE0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE1
            {.name="LD (C), A", .byteLength=2, .funcCall8=&CPU::ld_cp_a, .cycles=8},// 0xE2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE5
            {.name="AND A, 0x%02X", .byteLength=2, .funcCall8=&CPU::and_n, .cycles=8},// 0xE6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE9
            {.name="LD 0x%04X, A", .byteLength=3, .funcCall16=&CPU::ld_nnp_a, .cycles=16},// 0xEA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xED
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEF
            {.name="LDH A, 0xFF%02X", .byteLength=2, .funcCall8=&CPU::ldh_a_n, .cycles=12},// 0xF0
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF1
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF2
            {.name="DI", .byteLength=1, .funcCallVoid=&CPU::di, .cycles=4},// 0xF3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF4
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF5
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF6
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF7
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF8
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF9
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFA
            {.name="EI", .byteLength=1, .funcCallVoid=&CPU::ei, .cycles=4},// 0xFB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFD
            {.name="CP n", .byteLength=2, .funcCall8=&CPU::cp_n, .cycles=8},// 0xFE
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFF
    };

};


#endif //EMULATOR_CPU_H
