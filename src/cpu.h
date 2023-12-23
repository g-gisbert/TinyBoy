#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "registers.h"
#include "memory.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <sstream>

class CPU {
public:
    CPU(Memory& memo);
    void initMemory();
    int& step();
    void showState() const;

    // Memory
    Registers regs;
    uint8_t stack[0xFFFF];
    Memory& memory;

    int cycles;

    bool debug;
    long int nInstr;

    std::stringstream debugFile;
    std::ofstream MyFile = std::ofstream("filename.txt");

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
    void pushToStack(uint16_t nn);
    uint16_t popFromStack();

    void add8(uint8_t r);
    void add16(uint16_t rr);
    void adc(uint8_t r);
    void inc(uint8_t& r);
    void dec(uint8_t& r);
    void sub(uint8_t r);
    void sbc(uint8_t r);
    void _and(uint8_t r);
    void _xor(uint8_t r);
    void _or(uint8_t r);
    void cp(uint8_t val);

    uint8_t rlc(uint8_t& r);
    uint8_t rrc(uint8_t& r);
    uint8_t rl(uint8_t& r);
    uint8_t rr(uint8_t& r);
    uint8_t sla(uint8_t& r);
    uint8_t sra(uint8_t& r);
    uint8_t swap(uint8_t& r);
    uint8_t srl(uint8_t& r);
    void bit(uint8_t r, int b);
    void res8(uint8_t& r, int b);
    uint8_t set(uint8_t& r, int b);

    // Instructions
    void nop(); // 0x00
    void ld_bc_nn(uint16_t nn); // 0x01
    void ld_bcp_a(); // 0x02
    void inc_bc(); // 0x03
    void inc_b(); // 0x04
    void dec_b(); // 0x05
    void ld_b_n(uint8_t n); // 0x06
    void rlca(); // 0x07
    void ld_nn_sp(uint16_t nn); // 0x08
    void add_hl_bc(); // 0x09
    void ld_a_bcp(); // 0x0A
    void dec_bc(); // 0x0B
    void inc_c(); // 0x0C
    void dec_c(); // 0x0D
    void ld_c_n(uint8_t n); // 0x0E
    void rrca(); // 0x0F
    void ld_de_nn(uint16_t nn); // 0x11
    void ld_dep_a(); // 0x12
    void inc_de(); // 0x13
    void inc_d(); // 0x14
    void dec_d(); // 0x15
    void ld_d_n(uint8_t n); // 0x16
    void rla(); // 0x17
    void jr_n(uint8_t n); // 0x18
    void add_hl_de(); // 0x19
    void ld_a_dep(); // 0x1A
    void dec_de(); // 0x1B
    void inc_e(); // 0x1C
    void dec_e(); // 0x1D
    void ld_e_n(uint8_t n); // 0x1E
    void rra(); // 0x1F
    void jr_nz_n(uint8_t n); // 0x20
    void ld_hl_nn(uint16_t nn); // 0x21
    void ldi_hlp_a(); // 0x22
    void inc_hl(); // 0x23
    void inc_h(); // 0x24
    void dec_h(); // 0x25
    void ld_h_n(uint8_t n); // 0x26
    void daa(); // 0x27
    void jr_z_n(uint8_t n); // 0x28
    void add_hl_hl(); // 0x29
    void ldi_a_hl(); // 0x2A
    void dec_hl(); // 0x2B
    void inc_l(); // 0x2C
    void dec_l(); // 0x2D
    void ld_l_n(uint8_t n); // 0x2E
    void cpl(); // 0x2F
    void jr_nc_n(uint8_t n); // 0x30
    void ld_sp_nn(uint16_t nn); // 0x31
    void ld_hlpm_a(); // 0x32
    void inc_sp(); // 0x33
    void inc_hlp(); // 0x34
    void dec_hlp(); // 0x35
    void ld_hlp_n(uint8_t n); // 0x36
    void scf(); // 0x37
    void jr_c_n(uint8_t n); // 0x38
    void add_hl_sp(); // 0x39
    void ldd_a_hl(); // 0x3A
    void dec_sp(); // 0x3B
    void inc_a(); // 0x3C
    void dec_a(); // 0x3D
    void ld_a_n(uint8_t n); // 0x3E
    void ccf(); // 0x3F
    void ld_b_b(); // 0x40
    void ld_b_c(); // 0x41
    void ld_b_d(); // 0x42
    void ld_b_e(); // 0x43
    void ld_b_h(); // 0x44
    void ld_b_l(); // 0x45
    void ld_b_hlp(); // 0x46
    void ld_b_a(); // 0x47
    void ld_c_b(); // 0x48
    void ld_c_c(); // 0x49
    void ld_c_d(); // 0x4A
    void ld_c_e(); // 0x4B
    void ld_c_h(); // 0x4C
    void ld_c_l(); // 0x4D
    void ld_c_hlp(); // 0x4E
    void ld_c_a(); // 0x4F
    void ld_d_b(); // 0x50
    void ld_d_c(); // 0x51
    void ld_d_d(); // 0x52
    void ld_d_e(); // 0x53
    void ld_d_h(); // 0x54
    void ld_d_l(); // 0x55
    void ld_d_hlp(); // 0x56
    void ld_d_a(); // 0x57
    void ld_e_b(); // 0x58
    void ld_e_c(); // 0x59
    void ld_e_d(); // 0x5A
    void ld_e_e(); // 0x5B
    void ld_e_h(); // 0x5C
    void ld_e_l(); // 0x5D
    void ld_e_hlp(); // 0x5E
    void ld_e_a(); // 0x5F
    void ld_h_b(); // 0x60
    void ld_h_c(); // 0x61
    void ld_h_d(); // 0x62
    void ld_h_e(); // 0x63
    void ld_h_h(); // 0x64
    void ld_h_l(); // 0x65
    void ld_h_hlp(); // 0x66
    void ld_h_a(); // 0x67
    void ld_l_b(); // 0x68
    void ld_l_c(); // 0x69
    void ld_l_d(); // 0x6A
    void ld_l_e(); // 0x6B
    void ld_l_h(); // 0x6C
    void ld_l_l(); // 0x6D
    void ld_l_hlp(); // 0x6E
    void ld_l_a(); // 0x6F
    void ld_hlp_b(); // 0x70
    void ld_hlp_c(); // 0x71
    void ld_hlp_d(); // 0x72
    void ld_hlp_e(); // 0x73
    void ld_hlp_h(); // 0x74
    void ld_hlp_l(); // 0x75
    void halt(); // 0x76
    void ld_hlp_a(); // 0x77
    void ld_a_b(); // 0x78
    void ld_a_c(); // 0x79
    void ld_a_d(); // 0x7A
    void ld_a_e(); // 0x7B
    void ld_a_h(); // 0x7C
    void ld_a_l(); // 0x7D
    void ld_a_hlp(); // 0x7E
    void ld_a_a(); // 0x7F
    void add_b(); // 0x80
    void add_c(); // 0x81
    void add_d(); // 0x82
    void add_e(); // 0x83
    void add_h(); // 0x84
    void add_l(); // 0x85
    void add_hlp(); // 0x86
    void add_a(); // 0x87
    void adc_b(); // 0x88
    void adc_c(); // 0x89
    void adc_d(); // 0x8A
    void adc_e(); // 0x8B
    void adc_h(); // 0x8C
    void adc_l(); // 0x8D
    void adc_hlp(); // 0x8E
    void adc_a(); // 0x8F
    void sub_b(); // 0x90
    void sub_c(); // 0x91
    void sub_d(); // 0x92
    void sub_e(); // 0x93
    void sub_h(); // 0x94
    void sub_l(); // 0x95
    void sub_hlp(); // 0x96
    void sub_a(); // 0x97
    void sbc_b(); // 0x98
    void sbc_c(); // 0x99
    void sbc_d(); // 0x9A
    void sbc_e(); // 0x9B
    void sbc_h(); // 0x9C
    void sbc_l(); // 0x9D
    void sbc_hlp(); // 0x9E
    void sbc_a(); // 0x9F
    void and_b(); // 0xA0
    void and_c(); // 0xA1
    void and_d(); // 0xA2
    void and_e(); // 0xA3
    void and_h(); // 0xA4
    void and_l(); // 0xA5
    void and_hlp(); // 0xA6
    void and_a(); // 0xA7
    void xor_b(); // 0xA8
    void xor_c(); // 0xA9
    void xor_d(); // 0xAA
    void xor_e(); // 0xAB
    void xor_h(); // 0xAC
    void xor_l(); // 0xAD
    void xor_hlp(); // 0xAE
    void xor_a(); // 0xAF
    void or_b(); // 0xB0
    void or_c(); // 0xB1
    void or_d(); // 0xB2
    void or_e(); // 0xB3
    void or_h(); // 0xB4
    void or_l(); // 0xB5
    void or_hlp(); // 0xB6
    void or_a(); // 0xB7
    void cp_b(); // 0xB8
    void cp_c(); // 0xB9
    void cp_d(); // 0xBA
    void cp_e(); // 0xBB
    void cp_h(); // 0xBC
    void cp_l(); // 0xBD
    void cp_hlp(); // 0xBE
    void cp_a(); // 0xBF
    void ret_nz(); // 0xC0
    void pop_bc(); // 0xC1
    void jp_nz_nn(uint16_t nn); // 0xC2
    void jp_nn(uint16_t nn); // 0xC3
    void call_nz_nn(uint16_t nn); // 0xC4
    void push_bc(); // 0xC5
    void add_a_n(uint8_t n); // 0xC6
    void rst_00(); // 0xC7
    void ret_z(); // 0xC8
    void ret(); //0xC9
    void jp_z_nn(uint16_t nn); // 0xCA
    void cb_manager(uint8_t n); // 0xCB
    void call_z_nn(uint16_t nn); // 0xCC
    void call_nn(uint16_t nn); // 0xCD
    void adc_n(uint8_t n); // 0xCE
    void rst_08(); // 0xCF
    void ret_nc(); // 0xD0
    void pop_de(); // 0xD1
    void jp_nc_nn(uint16_t nn); // 0xD2
    void call_nc_nn(uint16_t nn); // 0xD4
    void push_de(); // 0xD5
    void sub_n(uint8_t n); // 0xD6
    void rst_10(); // 0xD7
    void ret_c(); // 0xD8
    void reti(); // 0xD9
    void jp_c_nn(uint16_t nn); // 0xDA
    void call_c_nn(uint16_t nn); // 0xDC
    void sbc_n(uint8_t n); // 0xDE
    void rst_18(); // 0xDF
    void ldh_n_a(uint8_t n); // 0xE0
    void pop_hl(); // 0xE1
    void ld_cp_a(); // 0xE2
    void push_hl(); // 0xE5
    void and_n(uint8_t n); // 0xE6
    void rst_20(); // 0xE7
    void add_sp_n(uint8_t n); // 0xE8
    void jp_hlp(); // 0xE9
    void ld_nnp_a(uint16_t nn); // 0xEA
    void xor_n(uint8_t n); // 0xEE
    void rst_28(); // 0xEF
    void ldh_a_n(uint8_t n); // 0xF0
    void pop_af(); // 0xF1
    void ld_a_cp(); // 0xF2
    void di(); // 0xF3
    void push_af(); // 0xF5
    void or_n(uint8_t n); // 0xF6
    void rst_30(); // 0xF7
    void ld_hl_spn(uint8_t n); // 0xF8
    void ld_sp_hl(); // 0xF9
    void ld_a_nnp(uint16_t nn); // 0xFA
    void ei(); // 0xFB
    void cp_n(uint8_t n); // 0xFE
    void rst_38(); // 0xFF

    // CB instructions
    void rlc_b(); // 0x00
    void rlc_c(); // 0x01
    void rlc_d(); // 0x02
    void rlc_e(); // 0x03
    void rlc_h(); // 0x04
    void rlc_l(); // 0x05
    void rlc_hlp(); // 0x06
    void rlc_a(); // 0x07
    void rrc_b(); // 0x08
    void rrc_c(); // 0x09
    void rrc_d(); // 0x0A
    void rrc_e(); // 0x0B
    void rrc_h(); // 0x0C
    void rrc_l(); // 0x0D
    void rrc_hlp(); // 0x0E
    void rrc_a(); // 0x0F
    void rl_b(); // 0x10
    void rl_c(); // 0x11
    void rl_d(); // 0x12
    void rl_e(); // 0x13
    void rl_h(); // 0x14
    void rl_l(); // 0x15
    void rl_hlp(); // 0x16
    void rl_a(); // 0x17
    void rr_b(); // 0x18
    void rr_c(); // 0x19
    void rr_d(); // 0x1A
    void rr_e(); // 0x1B
    void rr_h(); // 0x1C
    void rr_l(); // 0x1D
    void rr_hlp(); // 0x1E
    void rr_a(); // 0x1F
    void sla_b(); // 0x20
    void sla_c(); // 0x21
    void sla_d(); // 0x22
    void sla_e(); // 0x23
    void sla_h(); // 0x24
    void sla_l(); // 0x25
    void sla_hlp(); // 0x26
    void sla_a(); // 0x27
    void sra_b(); // 0x28
    void sra_c(); // 0x29
    void sra_d(); // 0x2A
    void sra_e(); // 0x2B
    void sra_h(); // 0x2C
    void sra_l(); // 0x2D
    void sra_hlp(); // 0x2E
    void sra_a(); // 0x2F
    void swap_b(); // 0x30
    void swap_c(); // 0x31
    void swap_d(); // 0x32
    void swap_e(); // 0x33
    void swap_h(); // 0x34
    void swap_l(); // 0x35
    void swap_hlp(); // 0x36
    void swap_a(); // 0x37
    void srl_b(); // 0x38
    void srl_c(); // 0x39
    void srl_d(); // 0x3A
    void srl_e(); // 0x3B
    void srl_h(); // 0x3C
    void srl_l(); // 0x3D
    void srl_hlp(); // 0x3E
    void srl_a(); // 0x3F
    void bit_0_b(); // 0x40
    void bit_0_c(); // 0x41
    void bit_0_d(); // 0x42
    void bit_0_e(); // 0x43
    void bit_0_h(); // 0x44
    void bit_0_l(); // 0x45
    void bit_0_hlp(); // 0x46
    void bit_0_a(); // 0x47
    void bit_1_b(); // 0x48
    void bit_1_c(); // 0x49
    void bit_1_d(); // 0x4A
    void bit_1_e(); // 0x4B
    void bit_1_h(); // 0x4C
    void bit_1_l(); // 0x4D
    void bit_1_hlp(); // 0x4E
    void bit_1_a(); // 0x4F
    void bit_2_b(); // 0x50
    void bit_2_c(); // 0x51
    void bit_2_d(); // 0x52
    void bit_2_e(); // 0x53
    void bit_2_h(); // 0x54
    void bit_2_l(); // 0x55
    void bit_2_hlp(); // 0x56
    void bit_2_a(); // 0x57
    void bit_3_b(); // 0x58
    void bit_3_c(); // 0x59
    void bit_3_d(); // 0x5A
    void bit_3_e(); // 0x5B
    void bit_3_h(); // 0x5C
    void bit_3_l(); // 0x5D
    void bit_3_hlp(); // 0x5E
    void bit_3_a(); // 0x5F
    void bit_4_b(); // 0x60
    void bit_4_c(); // 0x61
    void bit_4_d(); // 0x62
    void bit_4_e(); // 0x63
    void bit_4_h(); // 0x64
    void bit_4_l(); // 0x65
    void bit_4_hlp(); // 0x66
    void bit_4_a(); // 0x67
    void bit_5_b(); // 0x68
    void bit_5_c(); // 0x69
    void bit_5_d(); // 0x6A
    void bit_5_e(); // 0x6B
    void bit_5_h(); // 0x6C
    void bit_5_l(); // 0x6D
    void bit_5_hlp(); // 0x6E
    void bit_5_a(); // 0x6F
    void bit_6_b(); // 0x70
    void bit_6_c(); // 0x71
    void bit_6_d(); // 0x72
    void bit_6_e(); // 0x73
    void bit_6_h(); // 0x74
    void bit_6_l(); // 0x75
    void bit_6_hlp(); // 0x76
    void bit_6_a(); // 0x77
    void bit_7_b(); // 0x78
    void bit_7_c(); // 0x79
    void bit_7_d(); // 0x7A
    void bit_7_e(); // 0x7B
    void bit_7_h(); // 0x7C
    void bit_7_l(); // 0x7D
    void bit_7_hlp(); // 0x7E
    void bit_7_a(); // 0x7F
    void res_0_b(); // 0x80
    void res_0_c(); // 0x81
    void res_0_d(); // 0x82
    void res_0_e(); // 0x83
    void res_0_h(); // 0x84
    void res_0_l(); // 0x85
    void res_0_hlp(); // 0x86
    void res_0_a(); // 0x87
    void res_1_b(); // 0x88
    void res_1_c(); // 0x89
    void res_1_d(); // 0x8A
    void res_1_e(); // 0x8B
    void res_1_h(); // 0x8C
    void res_1_l(); // 0x8D
    void res_1_hlp(); // 0x8E
    void res_1_a(); // 0x8F
    void res_2_b(); // 0x90
    void res_2_c(); // 0x91
    void res_2_d(); // 0x92
    void res_2_e(); // 0x93
    void res_2_h(); // 0x94
    void res_2_l(); // 0x95
    void res_2_hlp(); // 0x96
    void res_2_a(); // 0x97
    void res_3_b(); // 0x98
    void res_3_c(); // 0x99
    void res_3_d(); // 0x9A
    void res_3_e(); // 0x9B
    void res_3_h(); // 0x9C
    void res_3_l(); // 0x9D
    void res_3_hlp(); // 0x9E
    void res_3_a(); // 0x9F
    void res_4_b(); // 0xA0
    void res_4_c(); // 0xA1
    void res_4_d(); // 0xA2
    void res_4_e(); // 0xA3
    void res_4_h(); // 0xA4
    void res_4_l(); // 0xA5
    void res_4_hlp(); // 0xA6
    void res_4_a(); // 0xA7
    void res_5_b(); // 0xA8
    void res_5_c(); // 0xA9
    void res_5_d(); // 0xAA
    void res_5_e(); // 0xAB
    void res_5_h(); // 0xAC
    void res_5_l(); // 0xAD
    void res_5_hlp(); // 0xAE
    void res_5_a(); // 0xAF
    void res_6_b(); // 0xB0
    void res_6_c(); // 0xB1
    void res_6_d(); // 0xB2
    void res_6_e(); // 0xB3
    void res_6_h(); // 0xB4
    void res_6_l(); // 0xB5
    void res_6_hlp(); // 0xB6
    void res_6_a(); // 0xB7
    void res_7_b(); // 0xB8
    void res_7_c(); // 0xB9
    void res_7_d(); // 0xBA
    void res_7_e(); // 0xBB
    void res_7_h(); // 0xBC
    void res_7_l(); // 0xBD
    void res_7_hlp(); // 0xBE
    void res_7_a(); // 0xBF
    void set_0_b(); // 0xC0
    void set_0_c(); // 0xC1
    void set_0_d(); // 0xC2
    void set_0_e(); // 0xC3
    void set_0_h(); // 0xC4
    void set_0_l(); // 0xC5
    void set_0_hlp(); // 0xC6
    void set_0_a(); // 0xC7
    void set_1_b(); // 0xC8
    void set_1_c(); // 0xC9
    void set_1_d(); // 0xCA
    void set_1_e(); // 0xCB
    void set_1_h(); // 0xCC
    void set_1_l(); // 0xCD
    void set_1_hlp(); // 0xCE
    void set_1_a(); // 0xCF
    void set_2_b(); // 0xD0
    void set_2_c(); // 0xD1
    void set_2_d(); // 0xD2
    void set_2_e(); // 0xD3
    void set_2_h(); // 0xD4
    void set_2_l(); // 0xD5
    void set_2_hlp(); // 0xD6
    void set_2_a(); // 0xD7
    void set_3_b(); // 0xD8
    void set_3_c(); // 0xD9
    void set_3_d(); // 0xDA
    void set_3_e(); // 0xDB
    void set_3_h(); // 0xDC
    void set_3_l(); // 0xDD
    void set_3_hlp(); // 0xDE
    void set_3_a(); // 0xDF
    void set_4_b(); // 0xE0
    void set_4_c(); // 0xE1
    void set_4_d(); // 0xE2
    void set_4_e(); // 0xE3
    void set_4_h(); // 0xE4
    void set_4_l(); // 0xE5
    void set_4_hlp(); // 0xE6
    void set_4_a(); // 0xE7
    void set_5_b(); // 0xE8
    void set_5_c(); // 0xE9
    void set_5_d(); // 0xEA
    void set_5_e(); // 0xEB
    void set_5_h(); // 0xEC
    void set_5_l(); // 0xED
    void set_5_hlp(); // 0xEE
    void set_5_a(); // 0xEF
    void set_6_b(); // 0xF0
    void set_6_c(); // 0xF1
    void set_6_d(); // 0xF2
    void set_6_e(); // 0xF3
    void set_6_h(); // 0xF4
    void set_6_l(); // 0xF5
    void set_6_hlp(); // 0xF6
    void set_6_a(); // 0xF7
    void set_7_b(); // 0xF8
    void set_7_c(); // 0xF9
    void set_7_d(); // 0xFA
    void set_7_e(); // 0xFB
    void set_7_h(); // 0xFC
    void set_7_l(); // 0xFD
    void set_7_hlp(); // 0xFE
    void set_7_a(); // 0xFF



    constexpr static Instruction instructions_set[256] = {
            {.name="NOP", .byteLength=1, .funcCallVoid=&CPU::nop, .cycles=4},                // 0x00
            {.name="LD BC, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_bc_nn, .cycles=12},   // 0x01
            {.name="LD (BC), A", .byteLength=1, .funcCallVoid=&CPU::ld_bcp_a, .cycles=8},// 0x02
            {.name="INC BC", .byteLength=1, .funcCallVoid=&CPU::inc_bc, .cycles=8},// 0x03
            {.name="INC B", .byteLength=1, .funcCallVoid=&CPU::inc_b, .cycles=4},// 0x04
            {.name="DEC B", .byteLength=1, .funcCallVoid=&CPU::dec_b, .cycles=4},// 0x05
            {.name="LD B, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_b_n, .cycles=8},// 0x06
            {.name="RLCA", .byteLength=1, .funcCallVoid=&CPU::rlca, .cycles=4},// 0x07
            {.name="LD 0x%04X, SP", .byteLength=3, .funcCall16=&CPU::ld_nn_sp, .cycles=20},// 0x08
            {.name="ADD HL, BC", .byteLength=1, .funcCallVoid=&CPU::add_hl_bc, .cycles=8},// 0x09
            {.name="LD A, (BC)", .byteLength=1, .funcCallVoid=&CPU::ld_a_bcp, .cycles=8},// 0x0A
            {.name="DEC BC", .byteLength=1, .funcCallVoid=&CPU::dec_bc, .cycles=8},// 0x0B
            {.name="INC C", .byteLength=1, .funcCallVoid=&CPU::inc_c, .cycles=4},// 0x0C
            {.name="DEC C", .byteLength=1, .funcCallVoid=&CPU::dec_c, .cycles=4},// 0x0D
            {.name="LD C, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_c_n, .cycles=8},// 0x0E
            {.name="RRCA", .byteLength=1, .funcCallVoid=&CPU::rrca, .cycles=4},// 0x0F
            {.name="???", .byteLength=1, .funcCallVoid=&CPU::nop, .cycles=0},// 0x10
            {.name="LD DE, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_de_nn, .cycles=12},// 0x11
            {.name="LD (DE), A", .byteLength=1, .funcCallVoid=&CPU::ld_dep_a, .cycles=8},// 0x12
            {.name="INC DE", .byteLength=1, .funcCallVoid=&CPU::inc_de, .cycles=8},// 0x13
            {.name="INC D", .byteLength=1, .funcCallVoid=&CPU::inc_d, .cycles=4},// 0x14
            {.name="DEC D", .byteLength=1, .funcCallVoid=&CPU::dec_d, .cycles=4},// 0x15
            {.name="LD D, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_d_n, .cycles=8},// 0x16
            {.name="RLA", .byteLength=1, .funcCallVoid=&CPU::rla, .cycles=4},// 0x17
            {.name="JR 0x%02X", .byteLength=2, .funcCall8=&CPU::jr_n, .cycles=12},// 0x18
            {.name="ADD HL, DE", .byteLength=1, .funcCallVoid=&CPU::add_hl_de, .cycles=8},// 0x19
            {.name="LD A, (DE)", .byteLength=1, .funcCallVoid=&CPU::ld_a_dep, .cycles=8},// 0x1A
            {.name="DEC DE", .byteLength=1, .funcCallVoid=&CPU::dec_de, .cycles=8},// 0x1B
            {.name="INC E", .byteLength=1, .funcCallVoid=&CPU::inc_e, .cycles=4},// 0x1C
            {.name="DEC E", .byteLength=1, .funcCallVoid=&CPU::dec_e, .cycles=4},// 0x1D
            {.name="LD E, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_e_n, .cycles=8},// 0x1E
            {.name="RRA", .byteLength=1, .funcCallVoid=&CPU::rra, .cycles=4},// 0x1F
            {.name="JR NZ, 0x%02X", .byteLength=2, .funcCall8=&CPU::jr_nz_n, .cycles=0},// 0x20
            {.name="LD HL, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_hl_nn, .cycles=12},// 0x21
            {.name="LD (HL+), A", .byteLength=1, .funcCallVoid=&CPU::ldi_hlp_a, .cycles=8},// 0x22
            {.name="INC HL", .byteLength=1, .funcCallVoid=&CPU::inc_hl, .cycles=8},// 0x23
            {.name="INC H", .byteLength=1, .funcCallVoid=&CPU::inc_h, .cycles=4},// 0x24
            {.name="DEC H", .byteLength=1, .funcCallVoid=&CPU::dec_h, .cycles=4},// 0x25
            {.name="LD H, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_h_n, .cycles=8},// 0x26
            {.name="DAA", .byteLength=1, .funcCallVoid=&CPU::daa, .cycles=4},// 0x27
            {.name="JR Z, 0x02X", .byteLength=2, .funcCall8=&CPU::jr_z_n, .cycles=0},// 0x28
            {.name="ADD HL, HL", .byteLength=1, .funcCallVoid=&CPU::add_hl_hl, .cycles=8},// 0x29
            {.name="LD A, (HL+)", .byteLength=1, .funcCallVoid=&CPU::ldi_a_hl, .cycles=8},// 0x2A
            {.name="DEC HL", .byteLength=1, .funcCallVoid=&CPU::dec_hl, .cycles=8},// 0x2B
            {.name="INC L", .byteLength=1, .funcCallVoid=&CPU::inc_l, .cycles=4},// 0x2C
            {.name="DEC L", .byteLength=1, .funcCallVoid=&CPU::dec_l, .cycles=4},// 0x2D
            {.name="LD L, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_l_n, .cycles=8},// 0x2E
            {.name="CPL", .byteLength=1, .funcCallVoid=&CPU::cpl, .cycles=4},// 0x2F
            {.name="JR NC, 0x%02X", .byteLength=2, .funcCall8=&CPU::jr_nc_n, .cycles=0},// 0x30
            {.name="LD SP, 0x04X", .byteLength=3, .funcCall16=&CPU::ld_sp_nn, .cycles=12},// 0x31
            {.name="LD (HL-), A", .byteLength=1, .funcCallVoid=&CPU::ld_hlpm_a, .cycles=8},// 0x32
            {.name="INC SP", .byteLength=1, .funcCallVoid=&CPU::inc_sp, .cycles=8},// 0x33
            {.name="INC (HL)", .byteLength=1, .funcCallVoid=&CPU::inc_hlp, .cycles=12},// 0x34
            {.name="DEC (HL)", .byteLength=1, .funcCallVoid=&CPU::dec_hlp, .cycles=12},// 0x35
            {.name="LD (HL), 0x02X", .byteLength=2, .funcCall8=&CPU::ld_hlp_n, .cycles=12},// 0x36
            {.name="SCF", .byteLength=1, .funcCallVoid=&CPU::scf, .cycles=4},// 0x37
            {.name="JR C, 0x%02X", .byteLength=2, .funcCall8=&CPU::jr_c_n, .cycles=0},// 0x38
            {.name="ADD HL, SP", .byteLength=1, .funcCallVoid=&CPU::add_hl_sp, .cycles=8},// 0x39
            {.name="LD A, (HL-)", .byteLength=1, .funcCallVoid=&CPU::ldd_a_hl, .cycles=8},// 0x3A
            {.name="DEC SP", .byteLength=1, .funcCallVoid=&CPU::dec_sp, .cycles=8},// 0x3B
            {.name="INC A", .byteLength=1, .funcCallVoid=&CPU::inc_a, .cycles=4},// 0x3C
            {.name="DEC A", .byteLength=1, .funcCallVoid=&CPU::dec_a, .cycles=4},// 0x3D
            {.name="LD A, 0x02X", .byteLength=2, .funcCall8=&CPU::ld_a_n, .cycles=8},// 0x3E
            {.name="CCF", .byteLength=1, .funcCallVoid=&CPU::ccf, .cycles=4},// 0x3F
            {.name="LD B, B", .byteLength=1, .funcCallVoid=&CPU::ld_b_b, .cycles=4},// 0x40
            {.name="LD B, C", .byteLength=1, .funcCallVoid=&CPU::ld_b_c, .cycles=4},// 0x41
            {.name="LD B, D", .byteLength=1, .funcCallVoid=&CPU::ld_b_d, .cycles=4},// 0x42
            {.name="LD B, E", .byteLength=1, .funcCallVoid=&CPU::ld_b_e, .cycles=4},// 0x43
            {.name="LD B, H", .byteLength=1, .funcCallVoid=&CPU::ld_b_h, .cycles=4},// 0x44
            {.name="LD B, L", .byteLength=1, .funcCallVoid=&CPU::ld_b_l, .cycles=4},// 0x45
            {.name="LD B, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_b_hlp, .cycles=8},// 0x46
            {.name="LD B, A", .byteLength=1, .funcCallVoid=&CPU::ld_b_a, .cycles=4},// 0x47
            {.name="LD C, B", .byteLength=1, .funcCallVoid=&CPU::ld_c_b, .cycles=4},// 0x48
            {.name="LD C, C", .byteLength=1, .funcCallVoid=&CPU::ld_c_c, .cycles=4},// 0x49
            {.name="LD C, D", .byteLength=1, .funcCallVoid=&CPU::ld_c_d, .cycles=4},// 0x4A
            {.name="LD C, E", .byteLength=1, .funcCallVoid=&CPU::ld_c_e, .cycles=4},// 0x4B
            {.name="LD C, H", .byteLength=1, .funcCallVoid=&CPU::ld_c_h, .cycles=4},// 0x4C
            {.name="LD C, L", .byteLength=1, .funcCallVoid=&CPU::ld_c_l, .cycles=4},// 0x4D
            {.name="LD C, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_c_hlp, .cycles=8},// 0x4E
            {.name="LD C, A", .byteLength=1, .funcCallVoid=&CPU::ld_c_a, .cycles=4},// 0x4F
            {.name="LD D, B", .byteLength=1, .funcCallVoid=&CPU::ld_d_b, .cycles=4},// 0x50
            {.name="LD D, C", .byteLength=1, .funcCallVoid=&CPU::ld_d_c, .cycles=4},// 0x51
            {.name="LD D, D", .byteLength=1, .funcCallVoid=&CPU::ld_d_d, .cycles=4},// 0x52
            {.name="LD D, E", .byteLength=1, .funcCallVoid=&CPU::ld_d_e, .cycles=4},// 0x53
            {.name="LD D, H", .byteLength=1, .funcCallVoid=&CPU::ld_d_h, .cycles=4},// 0x54
            {.name="LD D, L", .byteLength=1, .funcCallVoid=&CPU::ld_d_l, .cycles=4},// 0x55
            {.name="LD D, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_d_hlp, .cycles=8},// 0x56
            {.name="LD D, A", .byteLength=1, .funcCallVoid=&CPU::ld_d_a, .cycles=4},// 0x57
            {.name="LD E, B", .byteLength=1, .funcCallVoid=&CPU::ld_e_b, .cycles=4},// 0x58
            {.name="LD E, C", .byteLength=1, .funcCallVoid=&CPU::ld_e_c, .cycles=4},// 0x59
            {.name="LD E, D", .byteLength=1, .funcCallVoid=&CPU::ld_e_d, .cycles=4},// 0x5A
            {.name="LD E, E", .byteLength=1, .funcCallVoid=&CPU::ld_e_e, .cycles=4},// 0x5B
            {.name="LD E, H", .byteLength=1, .funcCallVoid=&CPU::ld_e_h, .cycles=4},// 0x5C
            {.name="LD E, L", .byteLength=1, .funcCallVoid=&CPU::ld_e_l, .cycles=4},// 0x5D
            {.name="LD E, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_e_hlp, .cycles=8},// 0x5E
            {.name="LD E, A", .byteLength=1, .funcCallVoid=&CPU::ld_e_a, .cycles=4},// 0x5F
            {.name="LD H, B", .byteLength=1, .funcCallVoid=&CPU::ld_h_b, .cycles=4},// 0x60
            {.name="LD H, C", .byteLength=1, .funcCallVoid=&CPU::ld_h_c, .cycles=4},// 0x61
            {.name="LD H, D", .byteLength=1, .funcCallVoid=&CPU::ld_h_d, .cycles=4},// 0x62
            {.name="LD H, E", .byteLength=1, .funcCallVoid=&CPU::ld_h_e, .cycles=4},// 0x63
            {.name="LD H, H", .byteLength=1, .funcCallVoid=&CPU::ld_h_h, .cycles=4},// 0x64
            {.name="LD H, L", .byteLength=1, .funcCallVoid=&CPU::ld_h_l, .cycles=4},// 0x65
            {.name="LD H, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_h_hlp, .cycles=8},// 0x66
            {.name="LD H, A", .byteLength=1, .funcCallVoid=&CPU::ld_h_a, .cycles=4},// 0x67
            {.name="LD L, B", .byteLength=1, .funcCallVoid=&CPU::ld_l_b, .cycles=4},// 0x68
            {.name="LD L, C", .byteLength=1, .funcCallVoid=&CPU::ld_l_c, .cycles=4},// 0x69
            {.name="LD L, D", .byteLength=1, .funcCallVoid=&CPU::ld_l_d, .cycles=4},// 0x6A
            {.name="LD L, E", .byteLength=1, .funcCallVoid=&CPU::ld_l_e, .cycles=4},// 0x6B
            {.name="LD L, H", .byteLength=1, .funcCallVoid=&CPU::ld_l_h, .cycles=4},// 0x6C
            {.name="LD L, L", .byteLength=1, .funcCallVoid=&CPU::ld_l_l, .cycles=4},// 0x6D
            {.name="LD L, (HL)", .byteLength=1, .funcCallVoid=&CPU::ld_l_hlp, .cycles=8},// 0x6E
            {.name="LD L, A", .byteLength=1, .funcCallVoid=&CPU::ld_l_a, .cycles=4},// 0x6F
            {.name="LD (HL), B", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_b, .cycles=8},// 0x70
            {.name="LD (HL), C", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_c, .cycles=8},// 0x71
            {.name="LD (HL), D", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_d, .cycles=8},// 0x72
            {.name="LD (HL), E", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_e, .cycles=8},// 0x73
            {.name="LD (HL), H", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_h, .cycles=8},// 0x74
            {.name="LD (HL), L", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_l, .cycles=8},// 0x75
            {.name="HALT", .byteLength=1, .funcCallVoid=&CPU::halt, .cycles=4},// 0x76
            {.name="LD (HL), A", .byteLength=1, .funcCallVoid=&CPU::ld_hlp_a, .cycles=8},// 0x77
            {.name="LD A, B", .byteLength=1, .funcCallVoid=&CPU::ld_a_b, .cycles=4},// 0x78
            {.name="LD A, C", .byteLength=1, .funcCallVoid=&CPU::ld_a_c, .cycles=4},// 0x79
            {.name="LD A, ", .byteLength=1, .funcCallVoid=&CPU::ld_a_d, .cycles=4},// 0x7A
            {.name="LD A, ", .byteLength=1, .funcCallVoid=&CPU::ld_a_e, .cycles=4},// 0x7B
            {.name="LD A, ", .byteLength=1, .funcCallVoid=&CPU::ld_a_h, .cycles=4},// 0x7C
            {.name="LD A, ", .byteLength=1, .funcCallVoid=&CPU::ld_a_l, .cycles=4},// 0x7D
            {.name="LD A, ", .byteLength=1, .funcCallVoid=&CPU::ld_a_hlp, .cycles=8},// 0x7E
            {.name="LD A, ", .byteLength=1, .funcCallVoid=&CPU::ld_a_a, .cycles=4},// 0x7F
            {.name="ADD A, B", .byteLength=1, .funcCallVoid=&CPU::add_b, .cycles=4},// 0x80
            {.name="ADD A, C", .byteLength=1, .funcCallVoid=&CPU::add_c, .cycles=4},// 0x81
            {.name="ADD A, D", .byteLength=1, .funcCallVoid=&CPU::add_d, .cycles=4},// 0x82
            {.name="ADD A, E", .byteLength=1, .funcCallVoid=&CPU::add_e, .cycles=4},// 0x83
            {.name="ADD A, H", .byteLength=1, .funcCallVoid=&CPU::add_h, .cycles=4},// 0x84
            {.name="ADD A, L", .byteLength=1, .funcCallVoid=&CPU::add_l, .cycles=4},// 0x85
            {.name="ADD A, (HL)", .byteLength=1, .funcCallVoid=&CPU::add_hlp, .cycles=8},// 0x86
            {.name="ADD A, A", .byteLength=1, .funcCallVoid=&CPU::add_a, .cycles=4},// 0x87
            {.name="ADC A, B", .byteLength=1, .funcCallVoid=&CPU::adc_b, .cycles=4},// 0x88
            {.name="ADC A, C", .byteLength=1, .funcCallVoid=&CPU::adc_c, .cycles=4},// 0x89
            {.name="ADC A, D", .byteLength=1, .funcCallVoid=&CPU::adc_d, .cycles=4},// 0x8A
            {.name="ADC A, E", .byteLength=1, .funcCallVoid=&CPU::adc_e, .cycles=4},// 0x8B
            {.name="ADC A, H", .byteLength=1, .funcCallVoid=&CPU::adc_h, .cycles=4},// 0x8C
            {.name="ADC A, L", .byteLength=1, .funcCallVoid=&CPU::adc_l, .cycles=4},// 0x8D
            {.name="ADC A, (HL)", .byteLength=1, .funcCallVoid=&CPU::adc_hlp, .cycles=8},// 0x8E
            {.name="ADC A, A", .byteLength=1, .funcCallVoid=&CPU::adc_a, .cycles=4},// 0x8F
            {.name="SUB A, B", .byteLength=1, .funcCallVoid=&CPU::sub_b, .cycles=4},// 0x90
            {.name="SUB A, C", .byteLength=1, .funcCallVoid=&CPU::sub_c, .cycles=4},// 0x91
            {.name="SUB A, D", .byteLength=1, .funcCallVoid=&CPU::sub_d, .cycles=4},// 0x92
            {.name="SUB A, E", .byteLength=1, .funcCallVoid=&CPU::sub_e, .cycles=4},// 0x93
            {.name="SUB A, H", .byteLength=1, .funcCallVoid=&CPU::sub_h, .cycles=4},// 0x94
            {.name="SUB A, L", .byteLength=1, .funcCallVoid=&CPU::sub_l, .cycles=4},// 0x95
            {.name="SUB A, (HL)", .byteLength=1, .funcCallVoid=&CPU::sub_hlp, .cycles=8},// 0x96
            {.name="SUB A, A", .byteLength=1, .funcCallVoid=&CPU::sub_a, .cycles=4},// 0x97
            {.name="SBC A, B", .byteLength=1, .funcCallVoid=&CPU::sbc_b, .cycles=4},// 0x98
            {.name="SBC A, C", .byteLength=1, .funcCallVoid=&CPU::sbc_c, .cycles=4},// 0x99
            {.name="SBC A, D", .byteLength=1, .funcCallVoid=&CPU::sbc_d, .cycles=4},// 0x9A
            {.name="SBC A, E", .byteLength=1, .funcCallVoid=&CPU::sbc_e, .cycles=4},// 0x9B
            {.name="SBC A, H", .byteLength=1, .funcCallVoid=&CPU::sbc_h, .cycles=4},// 0x9C
            {.name="SBC A, L", .byteLength=1, .funcCallVoid=&CPU::sbc_l, .cycles=4},// 0x9D
            {.name="SBC A, (HL)", .byteLength=1, .funcCallVoid=&CPU::sbc_hlp, .cycles=8},// 0x9E
            {.name="SBC A, A", .byteLength=1, .funcCallVoid=&CPU::sbc_a, .cycles=4},// 0x9F
            {.name="AND A, B", .byteLength=1, .funcCallVoid=&CPU::and_b, .cycles=4},// 0xA0
            {.name="AND A, C", .byteLength=1, .funcCallVoid=&CPU::and_c, .cycles=4},// 0xA1
            {.name="AND A, D", .byteLength=1, .funcCallVoid=&CPU::and_d, .cycles=4},// 0xA2
            {.name="AND A, E", .byteLength=1, .funcCallVoid=&CPU::and_e, .cycles=4},// 0xA3
            {.name="AND A, H", .byteLength=1, .funcCallVoid=&CPU::and_h, .cycles=4},// 0xA4
            {.name="AND A, L", .byteLength=1, .funcCallVoid=&CPU::and_l, .cycles=4},// 0xA5
            {.name="AND A, (HL)", .byteLength=1, .funcCallVoid=&CPU::and_hlp, .cycles=8},// 0xA6
            {.name="AND A, A", .byteLength=1, .funcCallVoid=&CPU::and_a, .cycles=4},// 0xA7
            {.name="XOR A, B", .byteLength=1, .funcCallVoid=&CPU::xor_b, .cycles=4},// 0xA8
            {.name="XOR A, C", .byteLength=1, .funcCallVoid=&CPU::xor_c, .cycles=4},// 0xA9
            {.name="XOR A, D", .byteLength=1, .funcCallVoid=&CPU::xor_d, .cycles=4},// 0xAA
            {.name="XOR A, E", .byteLength=1, .funcCallVoid=&CPU::xor_e, .cycles=4},// 0xAB
            {.name="XOR A, H", .byteLength=1, .funcCallVoid=&CPU::xor_h, .cycles=4},// 0xAC
            {.name="XOR A, L", .byteLength=1, .funcCallVoid=&CPU::xor_l, .cycles=4},// 0xAD
            {.name="XOR A, (HL)", .byteLength=1, .funcCallVoid=&CPU::xor_hlp, .cycles=8},// 0xAE
            {.name="XOR A, A", .byteLength=1, .funcCallVoid=&CPU::xor_a, .cycles=4},// 0xAF
            {.name="OR A, B", .byteLength=1, .funcCallVoid=&CPU::or_b, .cycles=4},// 0xB0
            {.name="OR A, C", .byteLength=1, .funcCallVoid=&CPU::or_c, .cycles=4},// 0xB1
            {.name="OR A, D", .byteLength=1, .funcCallVoid=&CPU::or_d, .cycles=4},// 0xB2
            {.name="OR A, E", .byteLength=1, .funcCallVoid=&CPU::or_e, .cycles=4},// 0xB3
            {.name="OR A, H", .byteLength=1, .funcCallVoid=&CPU::or_h, .cycles=4},// 0xB4
            {.name="OR A, L", .byteLength=1, .funcCallVoid=&CPU::or_l, .cycles=4},// 0xB5
            {.name="OR A, (HL)", .byteLength=1, .funcCallVoid=&CPU::or_hlp, .cycles=8},// 0xB6
            {.name="OR A, A", .byteLength=1, .funcCallVoid=&CPU::or_a, .cycles=4},// 0xB7
            {.name="CP B", .byteLength=1, .funcCallVoid=&CPU::cp_b, .cycles=4},// 0xB8
            {.name="CP C", .byteLength=1, .funcCallVoid=&CPU::cp_c, .cycles=4},// 0xB9
            {.name="CP D", .byteLength=1, .funcCallVoid=&CPU::cp_d, .cycles=4},// 0xBA
            {.name="CP E", .byteLength=1, .funcCallVoid=&CPU::cp_e, .cycles=4},// 0xBB
            {.name="CP H", .byteLength=1, .funcCallVoid=&CPU::cp_h, .cycles=4},// 0xBC
            {.name="CP L", .byteLength=1, .funcCallVoid=&CPU::cp_l, .cycles=4},// 0xBD
            {.name="CP (HL)", .byteLength=1, .funcCallVoid=&CPU::cp_hlp, .cycles=8},// 0xBE
            {.name="CP A", .byteLength=1, .funcCallVoid=&CPU::cp_a, .cycles=4},// 0xBF
            {.name="RET NZ", .byteLength=1, .funcCallVoid=&CPU::ret_nz, .cycles=0},// 0xC0
            {.name="POP BC", .byteLength=1, .funcCallVoid=&CPU::pop_bc, .cycles=12},// 0xC1
            {.name="JP NZ, 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_nz_nn, .cycles=0},// 0xC2
            {.name="JP 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_nn, .cycles=16},// 0xC3
            {.name="CALL NZ, 0x%04X", .byteLength=3, .funcCall16=&CPU::call_nz_nn, .cycles=0},// 0xC4
            {.name="PUSH BC", .byteLength=1, .funcCallVoid=&CPU::push_bc, .cycles=16},// 0xC5
            {.name="ADD A, 0x%02X", .byteLength=2, .funcCall8=&CPU::add_a_n, .cycles=8},// 0xC6
            {.name="RST 00", .byteLength=1, .funcCallVoid=&CPU::rst_00, .cycles=16},// 0xC7
            {.name="RET Z", .byteLength=1, .funcCallVoid=&CPU::ret_z, .cycles=0},// 0xC8
            {.name="RET", .byteLength=1, .funcCallVoid=&CPU::ret, .cycles=16},// 0xC9
            {.name="JP Z, 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_z_nn, .cycles=0},// 0xCA
            {.name="CB 0x%02X", .byteLength=2, .funcCall8=&CPU::cb_manager, .cycles=0},// 0xCB
            {.name="CALL Z, 0x%04X", .byteLength=3, .funcCall16=&CPU::call_z_nn, .cycles=0},// 0xCC
            {.name="CALL 0x%04X", .byteLength=3, .funcCall16=&CPU::call_nn, .cycles=24},// 0xCD
            {.name="ADC A, 0x%02X", .byteLength=2, .funcCall8=&CPU::adc_n, .cycles=8},// 0xCE
            {.name="RST 08", .byteLength=1, .funcCallVoid=&CPU::rst_08, .cycles=16},// 0xCF
            {.name="RET NC", .byteLength=1, .funcCallVoid=&CPU::ret_nc, .cycles=0},// 0xD0
            {.name="POP DE", .byteLength=1, .funcCallVoid=&CPU::pop_de, .cycles=12},// 0xD1
            {.name="JP NC, 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_nc_nn, .cycles=0},// 0xD2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xD3
            {.name="CALL NC, 0x%04X", .byteLength=3, .funcCall16=&CPU::call_nc_nn, .cycles=0},// 0xD4
            {.name="PUSH DE", .byteLength=1, .funcCallVoid=&CPU::push_de, .cycles=16},// 0xD5
            {.name="SUB 0x%02X", .byteLength=2, .funcCall8=&CPU::sub_n, .cycles=8},// 0xD6
            {.name="RST 10", .byteLength=1, .funcCallVoid=&CPU::rst_10, .cycles=16},// 0xD7
            {.name="RET C", .byteLength=1, .funcCallVoid=&CPU::ret_c, .cycles=0},// 0xD8
            {.name="RETI", .byteLength=1, .funcCallVoid=&CPU::reti, .cycles=16},// 0xD9
            {.name="JP C, 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_c_nn, .cycles=0},// 0xDA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDB
            {.name="CALL C, 0x%04X", .byteLength=3, .funcCall16=&CPU::call_c_nn, .cycles=0},// 0xDC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xDD
            {.name="SBC A, 0x%02X", .byteLength=2, .funcCall8=&CPU::sbc_n, .cycles=8},// 0xDE
            {.name="RST 18", .byteLength=1, .funcCallVoid=&CPU::rst_18, .cycles=16},// 0xDF
            {.name="LDH 0xFF%02X, A", .byteLength=2, .funcCall8=&CPU::ldh_n_a, .cycles=12},// 0xE0
            {.name="POP HL", .byteLength=1, .funcCallVoid=&CPU::pop_hl, .cycles=12},// 0xE1
            {.name="LD (C), A", .byteLength=1, .funcCallVoid=&CPU::ld_cp_a, .cycles=8},// 0xE2
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xE4
            {.name="PUSH HL", .byteLength=1, .funcCallVoid=&CPU::push_hl, .cycles=16},// 0xE5
            {.name="AND A, 0x%02X", .byteLength=2, .funcCall8=&CPU::and_n, .cycles=8},// 0xE6
            {.name="RST 20", .byteLength=1, .funcCallVoid=&CPU::rst_20, .cycles=16},// 0xE7
            {.name="ADD SP, 0x%02X", .byteLength=2, .funcCall8=&CPU::add_sp_n, .cycles=16},// 0xE8
            {.name="JP (HL)", .byteLength=1, .funcCallVoid=&CPU::jp_hlp, .cycles=4},// 0xE9
            {.name="LD 0x%04X, A", .byteLength=3, .funcCall16=&CPU::ld_nnp_a, .cycles=16},// 0xEA
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xEC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xED
            {.name="XOR A, 0x%02X", .byteLength=2, .funcCall8=&CPU::xor_n, .cycles=8},// 0xEE
            {.name="RST 28", .byteLength=1, .funcCallVoid=&CPU::rst_28, .cycles=16},// 0xEF
            {.name="LDH A, 0xFF%02X", .byteLength=2, .funcCall8=&CPU::ldh_a_n, .cycles=12},// 0xF0
            {.name="POP AF", .byteLength=1, .funcCallVoid=&CPU::pop_af, .cycles=12},// 0xF1
            {.name="LD A, (C)", .byteLength=1, .funcCallVoid=&CPU::ld_a_cp, .cycles=8},// 0xF2
            {.name="DI", .byteLength=1, .funcCallVoid=&CPU::di, .cycles=4},// 0xF3
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xF4
            {.name="PUSH AF", .byteLength=1, .funcCallVoid=&CPU::push_af, .cycles=16},// 0xF5
            {.name="OR 0x%02X", .byteLength=2, .funcCall8=&CPU::or_n, .cycles=8},// 0xF6
            {.name="RST 30", .byteLength=1, .funcCallVoid=&CPU::rst_30, .cycles=16},// 0xF7
            {.name="LD HL, SP + 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_hl_spn, .cycles=12},// 0xF8
            {.name="LD", .byteLength=1, .funcCallVoid=&CPU::ld_sp_hl, .cycles=8},// 0xF9
            {.name="LD A, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_a_nnp, .cycles=16},// 0xFA
            {.name="EI", .byteLength=1, .funcCallVoid=&CPU::ei, .cycles=4},// 0xFB
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFC
            {.name="???", .byteLength=0, .funcCallVoid=nullptr, .cycles=0},// 0xFD
            {.name="CP n", .byteLength=2, .funcCall8=&CPU::cp_n, .cycles=8},// 0xFE
            {.name="RST 38", .byteLength=1, .funcCallVoid=&CPU::rst_38, .cycles=16},// 0xFF
    };

    constexpr static Instruction CB_instructions[256] = {
            {.name="RLC B", .byteLength=1, .funcCallVoid=&CPU::rlc_b, .cycles=8},// 0x00
            {.name="RLC C", .byteLength=1, .funcCallVoid=&CPU::rlc_c, .cycles=8},// 0x01
            {.name="RLC D", .byteLength=1, .funcCallVoid=&CPU::rlc_d, .cycles=8},// 0x02
            {.name="RLC E", .byteLength=1, .funcCallVoid=&CPU::rlc_e, .cycles=8},// 0x03
            {.name="RLC H", .byteLength=1, .funcCallVoid=&CPU::rlc_h, .cycles=8},// 0x04
            {.name="RLC L", .byteLength=1, .funcCallVoid=&CPU::rlc_l, .cycles=8},// 0x05
            {.name="RLC (HL)", .byteLength=1, .funcCallVoid=&CPU::rlc_hlp, .cycles=16},// 0x06
            {.name="RLC A", .byteLength=1, .funcCallVoid=&CPU::rlc_a, .cycles=8},// 0x07
            {.name="RRC B", .byteLength=1, .funcCallVoid=&CPU::rrc_b, .cycles=8},// 0x08
            {.name="RRC C", .byteLength=1, .funcCallVoid=&CPU::rrc_c, .cycles=8},// 0x09
            {.name="RRC D", .byteLength=1, .funcCallVoid=&CPU::rrc_d, .cycles=8},// 0x0A
            {.name="RRC E", .byteLength=1, .funcCallVoid=&CPU::rrc_e, .cycles=8},// 0x0B
            {.name="RRC H", .byteLength=1, .funcCallVoid=&CPU::rrc_h, .cycles=8},// 0x0C
            {.name="RRC L", .byteLength=1, .funcCallVoid=&CPU::rrc_l, .cycles=8},// 0x0D
            {.name="RRC (HL)", .byteLength=1, .funcCallVoid=&CPU::rrc_hlp, .cycles=16},// 0x0E
            {.name="RRC A", .byteLength=1, .funcCallVoid=&CPU::rrc_a, .cycles=8},// 0x0F
            {.name="RL B", .byteLength=1, .funcCallVoid=&CPU::rl_b, .cycles=8},// 0x10
            {.name="RL C", .byteLength=1, .funcCallVoid=&CPU::rl_c, .cycles=8},// 0x11
            {.name="RL D", .byteLength=1, .funcCallVoid=&CPU::rl_d, .cycles=8},// 0x12
            {.name="RL E", .byteLength=1, .funcCallVoid=&CPU::rl_e, .cycles=8},// 0x13
            {.name="RL H", .byteLength=1, .funcCallVoid=&CPU::rl_h, .cycles=8},// 0x14
            {.name="RL L", .byteLength=1, .funcCallVoid=&CPU::rl_l, .cycles=8},// 0x15
            {.name="RL (HL)", .byteLength=1, .funcCallVoid=&CPU::rl_hlp, .cycles=16},// 0x16
            {.name="RL A", .byteLength=1, .funcCallVoid=&CPU::rl_a, .cycles=8},// 0x17
            {.name="RR B", .byteLength=1, .funcCallVoid=&CPU::rr_b, .cycles=8},// 0x18
            {.name="RR C", .byteLength=1, .funcCallVoid=&CPU::rr_c, .cycles=8},// 0x19
            {.name="RR D", .byteLength=1, .funcCallVoid=&CPU::rr_d, .cycles=8},// 0x1A
            {.name="RR E", .byteLength=1, .funcCallVoid=&CPU::rr_e, .cycles=8},// 0x1B
            {.name="RR H", .byteLength=1, .funcCallVoid=&CPU::rr_h, .cycles=8},// 0x1C
            {.name="RR L", .byteLength=1, .funcCallVoid=&CPU::rr_l, .cycles=8},// 0x1D
            {.name="RR (HL)", .byteLength=1, .funcCallVoid=&CPU::rr_hlp, .cycles=16},// 0x1E
            {.name="RR A", .byteLength=1, .funcCallVoid=&CPU::rr_a, .cycles=8},// 0x1F
            {.name="SLA B", .byteLength=1, .funcCallVoid=&CPU::sla_b, .cycles=8},// 0x20
            {.name="SLA C", .byteLength=1, .funcCallVoid=&CPU::sla_c, .cycles=8},// 0x21
            {.name="SLA D", .byteLength=1, .funcCallVoid=&CPU::sla_d, .cycles=8},// 0x22
            {.name="SLA E", .byteLength=1, .funcCallVoid=&CPU::sla_e, .cycles=8},// 0x23
            {.name="SLA H", .byteLength=1, .funcCallVoid=&CPU::sla_h, .cycles=8},// 0x24
            {.name="SLA L", .byteLength=1, .funcCallVoid=&CPU::sla_l, .cycles=8},// 0x25
            {.name="SLA (HL)", .byteLength=1, .funcCallVoid=&CPU::sla_hlp, .cycles=16},// 0x26
            {.name="SLA A", .byteLength=1, .funcCallVoid=&CPU::sla_a, .cycles=8},// 0x27
            {.name="SRA B", .byteLength=1, .funcCallVoid=&CPU::sra_b, .cycles=8},// 0x28
            {.name="SRA C", .byteLength=1, .funcCallVoid=&CPU::sra_c, .cycles=8},// 0x29
            {.name="SRA D", .byteLength=1, .funcCallVoid=&CPU::sra_d, .cycles=8},// 0x2A
            {.name="SRA E", .byteLength=1, .funcCallVoid=&CPU::sra_e, .cycles=8},// 0x2B
            {.name="SRA H", .byteLength=1, .funcCallVoid=&CPU::sra_h, .cycles=8},// 0x2C
            {.name="SRA L", .byteLength=1, .funcCallVoid=&CPU::sra_l, .cycles=8},// 0x2D
            {.name="SRA (HL)", .byteLength=1, .funcCallVoid=&CPU::sra_hlp, .cycles=16},// 0x2E
            {.name="SRA A", .byteLength=1, .funcCallVoid=&CPU::sra_a, .cycles=8},// 0x2F
            {.name="SWAP B", .byteLength=1, .funcCallVoid=&CPU::swap_b, .cycles=8},// 0x30
            {.name="SWAP C", .byteLength=1, .funcCallVoid=&CPU::swap_c, .cycles=8},// 0x31
            {.name="SWAP D", .byteLength=1, .funcCallVoid=&CPU::swap_d, .cycles=8},// 0x32
            {.name="SWAP E", .byteLength=1, .funcCallVoid=&CPU::swap_e, .cycles=8},// 0x33
            {.name="SWAP H", .byteLength=1, .funcCallVoid=&CPU::swap_h, .cycles=8},// 0x34
            {.name="SWAP L", .byteLength=1, .funcCallVoid=&CPU::swap_l, .cycles=8},// 0x35
            {.name="SWAP (HL)", .byteLength=1, .funcCallVoid=&CPU::swap_hlp, .cycles=16},// 0x36
            {.name="SWAP A", .byteLength=1, .funcCallVoid=&CPU::swap_a, .cycles=8},// 0x37
            {.name="SRL B", .byteLength=1, .funcCallVoid=&CPU::srl_b, .cycles=8},// 0x38
            {.name="SRL C", .byteLength=1, .funcCallVoid=&CPU::srl_c, .cycles=8},// 0x39
            {.name="SRL D", .byteLength=1, .funcCallVoid=&CPU::srl_d, .cycles=8},// 0x3A
            {.name="SRL E", .byteLength=1, .funcCallVoid=&CPU::srl_e, .cycles=8},// 0x3B
            {.name="SRL H", .byteLength=1, .funcCallVoid=&CPU::srl_h, .cycles=8},// 0x3C
            {.name="SRL L", .byteLength=1, .funcCallVoid=&CPU::srl_l, .cycles=8},// 0x3D
            {.name="SRL (HL)", .byteLength=1, .funcCallVoid=&CPU::srl_hlp, .cycles=16},// 0x3E
            {.name="SRL A", .byteLength=1, .funcCallVoid=&CPU::srl_a, .cycles=8},// 0x3F
            {.name="BIT 0, B", .byteLength=1, .funcCallVoid=&CPU::bit_0_b, .cycles=8},// 0x40
            {.name="BIT 0, C", .byteLength=1, .funcCallVoid=&CPU::bit_0_c, .cycles=8},// 0x41
            {.name="BIT 0, D", .byteLength=1, .funcCallVoid=&CPU::bit_0_d, .cycles=8},// 0x42
            {.name="BIT 0, E", .byteLength=1, .funcCallVoid=&CPU::bit_0_e, .cycles=8},// 0x43
            {.name="BIT 0, H", .byteLength=1, .funcCallVoid=&CPU::bit_0_h, .cycles=8},// 0x44
            {.name="BIT 0, L", .byteLength=1, .funcCallVoid=&CPU::bit_0_l, .cycles=8},// 0x45
            {.name="BIT 0, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_0_hlp, .cycles=16},// 0x46
            {.name="BIT 0, A", .byteLength=1, .funcCallVoid=&CPU::bit_0_a, .cycles=8},// 0x47
            {.name="BIT 1, B", .byteLength=1, .funcCallVoid=&CPU::bit_1_b, .cycles=8},// 0x48
            {.name="BIT 1, C", .byteLength=1, .funcCallVoid=&CPU::bit_1_c, .cycles=8},// 0x49
            {.name="BIT 1, D", .byteLength=1, .funcCallVoid=&CPU::bit_1_d, .cycles=8},// 0x4A
            {.name="BIT 1, E", .byteLength=1, .funcCallVoid=&CPU::bit_1_e, .cycles=8},// 0x4B
            {.name="BIT 1, H", .byteLength=1, .funcCallVoid=&CPU::bit_1_h, .cycles=8},// 0x4C
            {.name="BIT 1, L", .byteLength=1, .funcCallVoid=&CPU::bit_1_l, .cycles=8},// 0x4D
            {.name="BIT 1, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_1_hlp, .cycles=16},// 0x4E
            {.name="BIT 1, A", .byteLength=1, .funcCallVoid=&CPU::bit_1_a, .cycles=8},// 0x4F
            {.name="BIT 2, B", .byteLength=1, .funcCallVoid=&CPU::bit_2_b, .cycles=8},// 0x50
            {.name="BIT 2, C", .byteLength=1, .funcCallVoid=&CPU::bit_2_c, .cycles=8},// 0x51
            {.name="BIT 2, D", .byteLength=1, .funcCallVoid=&CPU::bit_2_d, .cycles=8},// 0x52
            {.name="BIT 2, E", .byteLength=1, .funcCallVoid=&CPU::bit_2_e, .cycles=8},// 0x53
            {.name="BIT 2, H", .byteLength=1, .funcCallVoid=&CPU::bit_2_h, .cycles=8},// 0x54
            {.name="BIT 2, L", .byteLength=1, .funcCallVoid=&CPU::bit_2_l, .cycles=8},// 0x55
            {.name="BIT 2, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_2_hlp, .cycles=16},// 0x56
            {.name="BIT 2, A", .byteLength=1, .funcCallVoid=&CPU::bit_2_a, .cycles=8},// 0x57
            {.name="BIT 3, B", .byteLength=1, .funcCallVoid=&CPU::bit_3_b, .cycles=8},// 0x58
            {.name="BIT 3, C", .byteLength=1, .funcCallVoid=&CPU::bit_3_c, .cycles=8},// 0x59
            {.name="BIT 3, D", .byteLength=1, .funcCallVoid=&CPU::bit_3_d, .cycles=8},// 0x5A
            {.name="BIT 3, E", .byteLength=1, .funcCallVoid=&CPU::bit_3_e, .cycles=8},// 0x5B
            {.name="BIT 3, H", .byteLength=1, .funcCallVoid=&CPU::bit_3_h, .cycles=8},// 0x5C
            {.name="BIT 3, L", .byteLength=1, .funcCallVoid=&CPU::bit_3_l, .cycles=8},// 0x5D
            {.name="BIT 3, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_3_hlp, .cycles=16},// 0x5E
            {.name="BIT 3, A", .byteLength=1, .funcCallVoid=&CPU::bit_3_a, .cycles=8},// 0x5F
            {.name="BIT 4, B", .byteLength=1, .funcCallVoid=&CPU::bit_4_b, .cycles=8},// 0x60
            {.name="BIT 4, C", .byteLength=1, .funcCallVoid=&CPU::bit_4_c, .cycles=8},// 0x61
            {.name="BIT 4, D", .byteLength=1, .funcCallVoid=&CPU::bit_4_d, .cycles=8},// 0x62
            {.name="BIT 4, E", .byteLength=1, .funcCallVoid=&CPU::bit_4_e, .cycles=8},// 0x63
            {.name="BIT 4, H", .byteLength=1, .funcCallVoid=&CPU::bit_4_h, .cycles=8},// 0x64
            {.name="BIT 4, L", .byteLength=1, .funcCallVoid=&CPU::bit_4_l, .cycles=8},// 0x65
            {.name="BIT 4, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_4_hlp, .cycles=16},// 0x66
            {.name="BIT 4, A", .byteLength=1, .funcCallVoid=&CPU::bit_4_a, .cycles=8},// 0x67
            {.name="BIT 5, B", .byteLength=1, .funcCallVoid=&CPU::bit_5_b, .cycles=8},// 0x68
            {.name="BIT 5, C", .byteLength=1, .funcCallVoid=&CPU::bit_5_c, .cycles=8},// 0x69
            {.name="BIT 5, D", .byteLength=1, .funcCallVoid=&CPU::bit_5_d, .cycles=8},// 0x6A
            {.name="BIT 5, E", .byteLength=1, .funcCallVoid=&CPU::bit_5_e, .cycles=8},// 0x6B
            {.name="BIT 5, H", .byteLength=1, .funcCallVoid=&CPU::bit_5_h, .cycles=8},// 0x6C
            {.name="BIT 5, L", .byteLength=1, .funcCallVoid=&CPU::bit_5_l, .cycles=8},// 0x6D
            {.name="BIT 5, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_5_hlp, .cycles=16},// 0x6E
            {.name="BIT 5, A", .byteLength=1, .funcCallVoid=&CPU::bit_5_a, .cycles=8},// 0x6F
            {.name="BIT 6, B", .byteLength=1, .funcCallVoid=&CPU::bit_6_b, .cycles=8},// 0x70
            {.name="BIT 6, C", .byteLength=1, .funcCallVoid=&CPU::bit_6_c, .cycles=8},// 0x71
            {.name="BIT 6, D", .byteLength=1, .funcCallVoid=&CPU::bit_6_d, .cycles=8},// 0x72
            {.name="BIT 6, E", .byteLength=1, .funcCallVoid=&CPU::bit_6_e, .cycles=8},// 0x73
            {.name="BIT 6, H", .byteLength=1, .funcCallVoid=&CPU::bit_6_h, .cycles=8},// 0x74
            {.name="BIT 6, L", .byteLength=1, .funcCallVoid=&CPU::bit_6_l, .cycles=8},// 0x75
            {.name="BIT 6, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_6_hlp, .cycles=16},// 0x76
            {.name="BIT 6, A", .byteLength=1, .funcCallVoid=&CPU::bit_6_a, .cycles=8},// 0x77
            {.name="BIT 7, B", .byteLength=1, .funcCallVoid=&CPU::bit_7_b, .cycles=8},// 0x78
            {.name="BIT 7, C", .byteLength=1, .funcCallVoid=&CPU::bit_7_c, .cycles=8},// 0x79
            {.name="BIT 7, D", .byteLength=1, .funcCallVoid=&CPU::bit_7_d, .cycles=8},// 0x7A
            {.name="BIT 7, E", .byteLength=1, .funcCallVoid=&CPU::bit_7_e, .cycles=8},// 0x7B
            {.name="BIT 7, H", .byteLength=1, .funcCallVoid=&CPU::bit_7_h, .cycles=8},// 0x7C
            {.name="BIT 7, L", .byteLength=1, .funcCallVoid=&CPU::bit_7_l, .cycles=8},// 0x7D
            {.name="BIT 7, (HL)", .byteLength=1, .funcCallVoid=&CPU::bit_7_hlp, .cycles=16},// 0x7E
            {.name="BIT 7, A", .byteLength=1, .funcCallVoid=&CPU::bit_7_a, .cycles=8},// 0x7F
            {.name="RES 0, B", .byteLength=1, .funcCallVoid=&CPU::res_0_b, .cycles=8},// 0x80
            {.name="RES 0, C", .byteLength=1, .funcCallVoid=&CPU::res_0_c, .cycles=8},// 0x81
            {.name="RES 0, D", .byteLength=1, .funcCallVoid=&CPU::res_0_d, .cycles=8},// 0x82
            {.name="RES 0, E", .byteLength=1, .funcCallVoid=&CPU::res_0_e, .cycles=8},// 0x83
            {.name="RES 0, H", .byteLength=1, .funcCallVoid=&CPU::res_0_h, .cycles=8},// 0x84
            {.name="RES 0, L", .byteLength=1, .funcCallVoid=&CPU::res_0_l, .cycles=8},// 0x85
            {.name="RES 0, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_0_hlp, .cycles=16},// 0x86
            {.name="RES 0, A", .byteLength=1, .funcCallVoid=&CPU::res_0_a, .cycles=8},// 0x87
            {.name="RES 1, B", .byteLength=1, .funcCallVoid=&CPU::res_1_b, .cycles=8},// 0x88
            {.name="RES 1, C", .byteLength=1, .funcCallVoid=&CPU::res_1_c, .cycles=8},// 0x89
            {.name="RES 1, D", .byteLength=1, .funcCallVoid=&CPU::res_1_d, .cycles=8},// 0x8A
            {.name="RES 1, E", .byteLength=1, .funcCallVoid=&CPU::res_1_e, .cycles=8},// 0x8B
            {.name="RES 1, H", .byteLength=1, .funcCallVoid=&CPU::res_1_h, .cycles=8},// 0x8C
            {.name="RES 1, L", .byteLength=1, .funcCallVoid=&CPU::res_1_l, .cycles=8},// 0x8D
            {.name="RES 1, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_1_hlp, .cycles=16},// 0x8E
            {.name="RES 1, A", .byteLength=1, .funcCallVoid=&CPU::res_1_a, .cycles=8},// 0x8F
            {.name="RES 2, B", .byteLength=1, .funcCallVoid=&CPU::res_2_b, .cycles=8},// 0x90
            {.name="RES 2, C", .byteLength=1, .funcCallVoid=&CPU::res_2_c, .cycles=8},// 0x91
            {.name="RES 2, D", .byteLength=1, .funcCallVoid=&CPU::res_2_d, .cycles=8},// 0x92
            {.name="RES 2, E", .byteLength=1, .funcCallVoid=&CPU::res_2_e, .cycles=8},// 0x93
            {.name="RES 2, H", .byteLength=1, .funcCallVoid=&CPU::res_2_h, .cycles=8},// 0x94
            {.name="RES 2, L", .byteLength=1, .funcCallVoid=&CPU::res_2_l, .cycles=8},// 0x95
            {.name="RES 2, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_2_hlp, .cycles=16},// 0x96
            {.name="RES 2, A", .byteLength=1, .funcCallVoid=&CPU::res_2_a, .cycles=8},// 0x97
            {.name="RES 3, B", .byteLength=1, .funcCallVoid=&CPU::res_3_b, .cycles=8},// 0x98
            {.name="RES 3, C", .byteLength=1, .funcCallVoid=&CPU::res_3_c, .cycles=8},// 0x99
            {.name="RES 3, D", .byteLength=1, .funcCallVoid=&CPU::res_3_d, .cycles=8},// 0x9A
            {.name="RES 3, E", .byteLength=1, .funcCallVoid=&CPU::res_3_e, .cycles=8},// 0x9B
            {.name="RES 3, H", .byteLength=1, .funcCallVoid=&CPU::res_3_h, .cycles=8},// 0x9C
            {.name="RES 3, L", .byteLength=1, .funcCallVoid=&CPU::res_3_l, .cycles=8},// 0x9D
            {.name="RES 3, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_3_hlp, .cycles=16},// 0x9E
            {.name="RES 3, A", .byteLength=1, .funcCallVoid=&CPU::res_3_a, .cycles=8},// 0x9F
            {.name="RES 4, B", .byteLength=1, .funcCallVoid=&CPU::res_4_b, .cycles=8},// 0xA0
            {.name="RES 4, C", .byteLength=1, .funcCallVoid=&CPU::res_4_c, .cycles=8},// 0xA1
            {.name="RES 4, D", .byteLength=1, .funcCallVoid=&CPU::res_4_d, .cycles=8},// 0xA2
            {.name="RES 4, E", .byteLength=1, .funcCallVoid=&CPU::res_4_e, .cycles=8},// 0xA3
            {.name="RES 4, H", .byteLength=1, .funcCallVoid=&CPU::res_4_h, .cycles=8},// 0xA4
            {.name="RES 4, L", .byteLength=1, .funcCallVoid=&CPU::res_4_l, .cycles=8},// 0xA5
            {.name="RES 4, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_4_hlp, .cycles=16},// 0xA6
            {.name="RES 4, A", .byteLength=1, .funcCallVoid=&CPU::res_4_a, .cycles=8},// 0xA7
            {.name="RES 5, B", .byteLength=1, .funcCallVoid=&CPU::res_5_b, .cycles=8},// 0xA8
            {.name="RES 5, C", .byteLength=1, .funcCallVoid=&CPU::res_5_c, .cycles=8},// 0xA9
            {.name="RES 5, D", .byteLength=1, .funcCallVoid=&CPU::res_5_d, .cycles=8},// 0xAA
            {.name="RES 5, E", .byteLength=1, .funcCallVoid=&CPU::res_5_e, .cycles=8},// 0xAB
            {.name="RES 5, H", .byteLength=1, .funcCallVoid=&CPU::res_5_h, .cycles=8},// 0xAC
            {.name="RES 5, L", .byteLength=1, .funcCallVoid=&CPU::res_5_l, .cycles=8},// 0xAD
            {.name="RES 5, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_5_hlp, .cycles=16},// 0xAE
            {.name="RES 5, A", .byteLength=1, .funcCallVoid=&CPU::res_5_a, .cycles=8},// 0xAF
            {.name="RES 6, B", .byteLength=1, .funcCallVoid=&CPU::res_6_b, .cycles=8},// 0xB0
            {.name="RES 6, C", .byteLength=1, .funcCallVoid=&CPU::res_6_c, .cycles=8},// 0xB1
            {.name="RES 6, D", .byteLength=1, .funcCallVoid=&CPU::res_6_d, .cycles=8},// 0xB2
            {.name="RES 6, E", .byteLength=1, .funcCallVoid=&CPU::res_6_e, .cycles=8},// 0xB3
            {.name="RES 6, H", .byteLength=1, .funcCallVoid=&CPU::res_6_h, .cycles=8},// 0xB4
            {.name="RES 6, L", .byteLength=1, .funcCallVoid=&CPU::res_6_l, .cycles=8},// 0xB5
            {.name="RES 6, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_6_hlp, .cycles=16},// 0xB6
            {.name="RES 6, A", .byteLength=1, .funcCallVoid=&CPU::res_6_a, .cycles=8},// 0xB7
            {.name="RES 7, B", .byteLength=1, .funcCallVoid=&CPU::res_7_b, .cycles=8},// 0xB8
            {.name="RES 7, C", .byteLength=1, .funcCallVoid=&CPU::res_7_c, .cycles=8},// 0xB9
            {.name="RES 7, D", .byteLength=1, .funcCallVoid=&CPU::res_7_d, .cycles=8},// 0xBA
            {.name="RES 7, E", .byteLength=1, .funcCallVoid=&CPU::res_7_e, .cycles=8},// 0xBB
            {.name="RES 7, H", .byteLength=1, .funcCallVoid=&CPU::res_7_h, .cycles=8},// 0xBC
            {.name="RES 7, L", .byteLength=1, .funcCallVoid=&CPU::res_7_l, .cycles=8},// 0xBD
            {.name="RES 7, (HL)", .byteLength=1, .funcCallVoid=&CPU::res_7_hlp, .cycles=16},// 0xBE
            {.name="RES 7, A", .byteLength=1, .funcCallVoid=&CPU::res_7_a, .cycles=8},// 0xBF
            {.name="SET 0, B", .byteLength=1, .funcCallVoid=&CPU::set_0_b, .cycles=8},// 0xC0
            {.name="SET 0, C", .byteLength=1, .funcCallVoid=&CPU::set_0_c, .cycles=8},// 0xC1
            {.name="SET 0, D", .byteLength=1, .funcCallVoid=&CPU::set_0_d, .cycles=8},// 0xC2
            {.name="SET 0, E", .byteLength=1, .funcCallVoid=&CPU::set_0_e, .cycles=8},// 0xC3
            {.name="SET 0, H", .byteLength=1, .funcCallVoid=&CPU::set_0_h, .cycles=8},// 0xC4
            {.name="SET 0, L", .byteLength=1, .funcCallVoid=&CPU::set_0_l, .cycles=8},// 0xC5
            {.name="SET 0, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_0_hlp, .cycles=16},// 0xC6
            {.name="SET 0, A", .byteLength=1, .funcCallVoid=&CPU::set_0_a, .cycles=8},// 0xC7
            {.name="SET 1, B", .byteLength=1, .funcCallVoid=&CPU::set_1_b, .cycles=8},// 0xC8
            {.name="SET 1, C", .byteLength=1, .funcCallVoid=&CPU::set_1_c, .cycles=8},// 0xC9
            {.name="SET 1, D", .byteLength=1, .funcCallVoid=&CPU::set_1_d, .cycles=8},// 0xCA
            {.name="SET 1, E", .byteLength=1, .funcCallVoid=&CPU::set_1_e, .cycles=8},// 0xCB
            {.name="SET 1, H", .byteLength=1, .funcCallVoid=&CPU::set_1_h, .cycles=8},// 0xCC
            {.name="SET 1, L", .byteLength=1, .funcCallVoid=&CPU::set_1_l, .cycles=8},// 0xCD
            {.name="SET 1, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_1_hlp, .cycles=16},// 0xCE
            {.name="SET 1, A", .byteLength=1, .funcCallVoid=&CPU::set_1_a, .cycles=8},// 0xCF
            {.name="SET 2, B", .byteLength=1, .funcCallVoid=&CPU::set_2_b, .cycles=8},// 0xD0
            {.name="SET 2, C", .byteLength=1, .funcCallVoid=&CPU::set_2_c, .cycles=8},// 0xD1
            {.name="SET 2, D", .byteLength=1, .funcCallVoid=&CPU::set_2_d, .cycles=8},// 0xD2
            {.name="SET 2, E", .byteLength=1, .funcCallVoid=&CPU::set_2_e, .cycles=8},// 0xD3
            {.name="SET 2, H", .byteLength=1, .funcCallVoid=&CPU::set_2_h, .cycles=8},// 0xD4
            {.name="SET 2, L", .byteLength=1, .funcCallVoid=&CPU::set_2_l, .cycles=8},// 0xD5
            {.name="SET 2, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_2_hlp, .cycles=16},// 0xD6
            {.name="SET 2, A", .byteLength=1, .funcCallVoid=&CPU::set_2_a, .cycles=8},// 0xD7
            {.name="SET 3, B", .byteLength=1, .funcCallVoid=&CPU::set_3_b, .cycles=8},// 0xD8
            {.name="SET 3, C", .byteLength=1, .funcCallVoid=&CPU::set_3_c, .cycles=8},// 0xD9
            {.name="SET 3, D", .byteLength=1, .funcCallVoid=&CPU::set_3_d, .cycles=8},// 0xDA
            {.name="SET 3, E", .byteLength=1, .funcCallVoid=&CPU::set_3_e, .cycles=8},// 0xDB
            {.name="SET 3, H", .byteLength=1, .funcCallVoid=&CPU::set_3_h, .cycles=8},// 0xDC
            {.name="SET 3, L", .byteLength=1, .funcCallVoid=&CPU::set_3_l, .cycles=8},// 0xDD
            {.name="SET 3, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_3_hlp, .cycles=16},// 0xDE
            {.name="SET 3, A", .byteLength=1, .funcCallVoid=&CPU::set_3_a, .cycles=8},// 0xDF
            {.name="SET 4, B", .byteLength=1, .funcCallVoid=&CPU::set_4_b, .cycles=8},// 0xE0
            {.name="SET 4, C", .byteLength=1, .funcCallVoid=&CPU::set_4_c, .cycles=8},// 0xE1
            {.name="SET 4, D", .byteLength=1, .funcCallVoid=&CPU::set_4_d, .cycles=8},// 0xE2
            {.name="SET 4, E", .byteLength=1, .funcCallVoid=&CPU::set_4_e, .cycles=8},// 0xE3
            {.name="SET 4, H", .byteLength=1, .funcCallVoid=&CPU::set_4_h, .cycles=8},// 0xE4
            {.name="SET 4, L", .byteLength=1, .funcCallVoid=&CPU::set_4_l, .cycles=8},// 0xE5
            {.name="SET 4, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_4_hlp, .cycles=16},// 0xE6
            {.name="SET 4, A", .byteLength=1, .funcCallVoid=&CPU::set_4_a, .cycles=8},// 0xE7
            {.name="SET 5, B", .byteLength=1, .funcCallVoid=&CPU::set_5_b, .cycles=8},// 0xE8
            {.name="SET 5, C", .byteLength=1, .funcCallVoid=&CPU::set_5_c, .cycles=8},// 0xE9
            {.name="SET 5, D", .byteLength=1, .funcCallVoid=&CPU::set_5_d, .cycles=8},// 0xEA
            {.name="SET 5, E", .byteLength=1, .funcCallVoid=&CPU::set_5_e, .cycles=8},// 0xEB
            {.name="SET 5, H", .byteLength=1, .funcCallVoid=&CPU::set_5_h, .cycles=8},// 0xEC
            {.name="SET 5, L", .byteLength=1, .funcCallVoid=&CPU::set_5_l, .cycles=8},// 0xED
            {.name="SET 5, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_5_hlp, .cycles=16},// 0xEE
            {.name="SET 5, A", .byteLength=1, .funcCallVoid=&CPU::set_5_a, .cycles=8},// 0xEF
            {.name="SET 6, B", .byteLength=1, .funcCallVoid=&CPU::set_6_b, .cycles=8},// 0xF0
            {.name="SET 6, C", .byteLength=1, .funcCallVoid=&CPU::set_6_c, .cycles=8},// 0xF1
            {.name="SET 6, D", .byteLength=1, .funcCallVoid=&CPU::set_6_d, .cycles=8},// 0xF2
            {.name="SET 6, E", .byteLength=1, .funcCallVoid=&CPU::set_6_e, .cycles=8},// 0xF3
            {.name="SET 6, H", .byteLength=1, .funcCallVoid=&CPU::set_6_h, .cycles=8},// 0xF4
            {.name="SET 6, L", .byteLength=1, .funcCallVoid=&CPU::set_6_l, .cycles=8},// 0xF5
            {.name="SET 6, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_6_hlp, .cycles=16},// 0xF6
            {.name="SET 6, A", .byteLength=1, .funcCallVoid=&CPU::set_6_a, .cycles=8},// 0xF7
            {.name="SET 7, B", .byteLength=1, .funcCallVoid=&CPU::set_7_b, .cycles=8},// 0xF8
            {.name="SET 7, C", .byteLength=1, .funcCallVoid=&CPU::set_7_c, .cycles=8},// 0xF9
            {.name="SET 7, D", .byteLength=1, .funcCallVoid=&CPU::set_7_d, .cycles=8},// 0xFA
            {.name="SET 7, E", .byteLength=1, .funcCallVoid=&CPU::set_7_e, .cycles=8},// 0xFB
            {.name="SET 7, H", .byteLength=1, .funcCallVoid=&CPU::set_7_h, .cycles=8},// 0xFC
            {.name="SET 7, L", .byteLength=1, .funcCallVoid=&CPU::set_7_l, .cycles=8},// 0xFD
            {.name="SET 7, (HL)", .byteLength=1, .funcCallVoid=&CPU::set_7_hlp, .cycles=16},// 0xFE
            {.name="SET 7, A", .byteLength=1, .funcCallVoid=&CPU::set_7_a, .cycles=8},// 0xFF
    };

};


#endif //EMULATOR_CPU_H
