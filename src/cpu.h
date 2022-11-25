#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "registers.h"
#include "cartridge.h"
#include <cstdio>
#include <iostream>

class CPU {
public:
    CPU();
    void initMemory();
    void step();
    //fetch, read, write
    uint8_t read8(uint16_t address);
    uint16_t read16(uint16_t address);
    void write8(uint16_t address);
    void write16(uint16_t address);

    // Memory
    Registers regs;
    Cartridge cart;

    // Instructions
    struct Instruction {

        const char* name;
        uint8_t byteLength;
        union {
            void (CPU::*funcCallVoid)();
            void (CPU::*funcCall8)(uint8_t);
            void (CPU::*funcCall16)(uint16_t);
        };

    };

    void nop(); // 0 (0x00)
    void ld_bc_nn(uint16_t nn); // 1 (0x01)
    void ld_c_n(uint8_t n);
    void ld_hl_nn(uint16_t nn); // 0x21
    void jp_nn(uint16_t nn); // 195 (0xC3)
    void xor_a(); // 0xAF


    constexpr static Instruction instructions_set[256] = {
            {.name="NOP", .byteLength=1, .funcCallVoid=&CPU::nop},                // 0x00
            {.name="LD BC, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_bc_nn},   // 0x01
            {"???", 0,           nullptr},// 0x02
            {"???", 0,           nullptr},// 0x03
            {"???", 0,           nullptr},// 0x04
            {"???", 0,           nullptr},// 0x05
            {"???", 0,           nullptr},// 0x06
            {"???", 0,           nullptr},// 0x07
            {"???", 0,           nullptr},// 0x08
            {"???", 0,           nullptr},// 0x09
            {"???", 0,           nullptr},// 0x0A
            {"???", 0,           nullptr},// 0x0B
            {"???", 0,           nullptr},// 0x0C
            {"???", 0, nullptr},// 0x0D
            {.name="LD C, 0x%02X", .byteLength=2, .funcCall8=&CPU::ld_c_n},// 0x0E
            {"???", 0, nullptr},// 0x0F
            {"???", 0, nullptr},// 0x10
            {"???", 0, nullptr},// 0x11
            {"???", 0, nullptr},// 0x12
            {"???", 0, nullptr},// 0x13
            {"???", 0, nullptr},// 0x14
            {"???", 0, nullptr},// 0x15
            {"???", 0, nullptr},// 0x16
            {"???", 0, nullptr},// 0x17
            {"???", 0, nullptr},// 0x18
            {"???", 0, nullptr},// 0x19
            {"???", 0, nullptr},// 0x1A
            {"???", 0, nullptr},// 0x1B
            {"???", 0, nullptr},// 0x1C
            {"???", 0, nullptr},// 0x1D
            {"???", 0, nullptr},// 0x1E
            {"???", 0, nullptr},// 0x1F
            {"???", 0, nullptr},// 0x20
            {.name="LD HL, 0x%04X", .byteLength=3, .funcCall16=&CPU::ld_hl_nn},// 0x21
            {"???", 0, nullptr},// 0x22
            {"???", 0, nullptr},// 0x23
            {"???", 0, nullptr},// 0x24
            {"???", 0, nullptr},// 0x25
            {"???", 0, nullptr},// 0x26
            {"???", 0, nullptr},// 0x27
            {"???", 0, nullptr},// 0x28
            {"???", 0, nullptr},// 0x29
            {"???", 0, nullptr},// 0x2A
            {"???", 0, nullptr},// 0x2B
            {"???", 0, nullptr},// 0x2C
            {"???", 0, nullptr},// 0x2D
            {"???", 0, nullptr},// 0x2E
            {"???", 0, nullptr},// 0x2F
            {"???", 0, nullptr},// 0x30
            {"???", 0, nullptr},// 0x31
            {"???", 0, nullptr},// 0x32
            {"???", 0, nullptr},// 0x33
            {"???", 0, nullptr},// 0x34
            {"???", 0, nullptr},// 0x35
            {"???", 0, nullptr},// 0x36
            {"???", 0, nullptr},// 0x37
            {"???", 0, nullptr},// 0x38
            {"???", 0, nullptr},// 0x39
            {"???", 0, nullptr},// 0x3A
            {"???", 0, nullptr},// 0x3B
            {"???", 0, nullptr},// 0x3C
            {"???", 0, nullptr},// 0x3D
            {"???", 0, nullptr},// 0x3E
            {"???", 0, nullptr},// 0x3F
            {"???", 0, nullptr},// 0x40
            {"???", 0, nullptr},// 0x41
            {"???", 0, nullptr},// 0x42
            {"???", 0, nullptr},// 0x43
            {"???", 0, nullptr},// 0x44
            {"???", 0, nullptr},// 0x45
            {"???", 0, nullptr},// 0x46
            {"???", 0, nullptr},// 0x47
            {"???", 0, nullptr},// 0x48
            {"???", 0, nullptr},// 0x49
            {"???", 0, nullptr},// 0x4A
            {"???", 0, nullptr},// 0x4B
            {"???", 0, nullptr},// 0x4C
            {"???", 0, nullptr},// 0x4D
            {"???", 0, nullptr},// 0x4E
            {"???", 0, nullptr},// 0x4F
            {"???", 0, nullptr},// 0x50
            {"???", 0, nullptr},// 0x51
            {"???", 0, nullptr},// 0x52
            {"???", 0, nullptr},// 0x53
            {"???", 0, nullptr},// 0x54
            {"???", 0, nullptr},// 0x55
            {"???", 0, nullptr},// 0x56
            {"???", 0, nullptr},// 0x57
            {"???", 0, nullptr},// 0x58
            {"???", 0, nullptr},// 0x59
            {"???", 0, nullptr},// 0x5A
            {"???", 0, nullptr},// 0x5B
            {"???", 0, nullptr},// 0x5C
            {"???", 0, nullptr},// 0x5D
            {"???", 0, nullptr},// 0x5E
            {"???", 0, nullptr},// 0x5F
            {"???", 0, nullptr},// 0x60
            {"???", 0, nullptr},// 0x61
            {"???", 0, nullptr},// 0x62
            {"???", 0, nullptr},// 0x63
            {"???", 0, nullptr},// 0x64
            {"???", 0, nullptr},// 0x65
            {"???", 0, nullptr},// 0x66
            {"???", 0, nullptr},// 0x67
            {"???", 0, nullptr},// 0x68
            {"???", 0, nullptr},// 0x69
            {"???", 0, nullptr},// 0x6A
            {"???", 0, nullptr},// 0x6B
            {"???", 0, nullptr},// 0x6C
            {"???", 0, nullptr},// 0x6D
            {"???", 0, nullptr},// 0x6E
            {"???", 0, nullptr},// 0x6F
            {"???", 0, nullptr},// 0x70
            {"???", 0, nullptr},// 0x71
            {"???", 0, nullptr},// 0x72
            {"???", 0, nullptr},// 0x73
            {"???", 0, nullptr},// 0x74
            {"???", 0, nullptr},// 0x75
            {"???", 0, nullptr},// 0x76
            {"???", 0, nullptr},// 0x77
            {"???", 0, nullptr},// 0x78
            {"???", 0, nullptr},// 0x79
            {"???", 0, nullptr},// 0x7A
            {"???", 0, nullptr},// 0x7B
            {"???", 0, nullptr},// 0x7C
            {"???", 0, nullptr},// 0x7D
            {"???", 0, nullptr},// 0x7E
            {"???", 0, nullptr},// 0x7F
            {"???", 0, nullptr},// 0x80
            {"???", 0, nullptr},// 0x81
            {"???", 0, nullptr},// 0x82
            {"???", 0, nullptr},// 0x83
            {"???", 0, nullptr},// 0x84
            {"???", 0, nullptr},// 0x85
            {"???", 0, nullptr},// 0x86
            {"???", 0, nullptr},// 0x87
            {"???", 0, nullptr},// 0x88
            {"???", 0, nullptr},// 0x89
            {"???", 0, nullptr},// 0x8A
            {"???", 0, nullptr},// 0x8B
            {"???", 0, nullptr},// 0x8C
            {"???", 0, nullptr},// 0x8D
            {"???", 0, nullptr},// 0x8E
            {"???", 0, nullptr},// 0x8F
            {"???", 0, nullptr},// 0x90
            {"???", 0, nullptr},// 0x91
            {"???", 0, nullptr},// 0x92
            {"???", 0, nullptr},// 0x93
            {"???", 0, nullptr},// 0x94
            {"???", 0, nullptr},// 0x95
            {"???", 0, nullptr},// 0x96
            {"???", 0, nullptr},// 0x97
            {"???", 0, nullptr},// 0x98
            {"???", 0, nullptr},// 0x99
            {"???", 0, nullptr},// 0x9A
            {"???", 0, nullptr},// 0x9B
            {"???", 0, nullptr},// 0x9C
            {"???", 0, nullptr},// 0x9D
            {"???", 0, nullptr},// 0x9E
            {"???", 0, nullptr},// 0x9F
            {"???", 0, nullptr},// 0xA0
            {"???", 0, nullptr},// 0xA1
            {"???", 0, nullptr},// 0xA2
            {"???", 0, nullptr},// 0xA3
            {"???", 0, nullptr},// 0xA4
            {"???", 0, nullptr},// 0xA5
            {"???", 0, nullptr},// 0xA6
            {"???", 0, nullptr},// 0xA7
            {"???", 0, nullptr},// 0xA8
            {"???", 0, nullptr},// 0xA9
            {"???", 0, nullptr},// 0xAA
            {"???", 0, nullptr},// 0xAB
            {"???", 0, nullptr},// 0xAC
            {"???", 0, nullptr},// 0xAD
            {"???", 0, nullptr},// 0xAE
            {.name="XOR A, A", .byteLength=1, .funcCallVoid=&CPU::xor_a},// 0xAF
            {"???", 0, nullptr},// 0xB0
            {"???", 0, nullptr},// 0xB1
            {"???", 0, nullptr},// 0xB2
            {"???", 0, nullptr},// 0xB3
            {"???", 0, nullptr},// 0xB4
            {"???", 0, nullptr},// 0xB5
            {"???", 0, nullptr},// 0xB6
            {"???", 0, nullptr},// 0xB7
            {"???", 0, nullptr},// 0xB8
            {"???", 0, nullptr},// 0xB9
            {"???", 0, nullptr},// 0xBA
            {"???", 0, nullptr},// 0xBB
            {"???", 0, nullptr},// 0xBC
            {"???", 0, nullptr},// 0xBD
            {"???", 0, nullptr},// 0xBE
            {"???", 0, nullptr},// 0xBF
            {"???", 0, nullptr},// 0xC0
            {"???", 0, nullptr},// 0xC1
            {"???", 0, nullptr},// 0xC2
            {.name="JP 0x%04X", .byteLength=3, .funcCall16=&CPU::jp_nn},// 0xC3
            {"???", 0, nullptr},// 0xC4
            {"???", 0, nullptr},// 0xC5
            {"???", 0, nullptr},// 0xC6
            {"???", 0, nullptr},// 0xC7
            {"???", 0, nullptr},// 0xC8
            {"???", 0, nullptr},// 0xC9
            {"???", 0, nullptr},// 0xCA
            {"???", 0, nullptr},// 0xCB
            {"???", 0, nullptr},// 0xCC
            {"???", 0, nullptr},// 0xCD
            {"???", 0, nullptr},// 0xCE
            {"???", 0, nullptr},// 0xCF
            {"???", 0, nullptr},// 0xD0
            {"???", 0, nullptr},// 0xD1
            {"???", 0, nullptr},// 0xD2
            {"???", 0, nullptr},// 0xD3
            {"???", 0, nullptr},// 0xD4
            {"???", 0, nullptr},// 0xD5
            {"???", 0, nullptr},// 0xD6
            {"???", 0, nullptr},// 0xD7
            {"???", 0, nullptr},// 0xD8
            {"???", 0, nullptr},// 0xD9
            {"???", 0, nullptr},// 0xDA
            {"???", 0, nullptr},// 0xDB
            {"???", 0, nullptr},// 0xDC
            {"???", 0, nullptr},// 0xDD
            {"???", 0, nullptr},// 0xDE
            {"???", 0, nullptr},// 0xDF
            {"???", 0, nullptr},// 0xE0
            {"???", 0, nullptr},// 0xE1
            {"???", 0, nullptr},// 0xE2
            {"???", 0, nullptr},// 0xE3
            {"???", 0, nullptr},// 0xE4
            {"???", 0, nullptr},// 0xE5
            {"???", 0, nullptr},// 0xE6
            {"???", 0, nullptr},// 0xE7
            {"???", 0, nullptr},// 0xE8
            {"???", 0, nullptr},// 0xE9
            {"???", 0, nullptr},// 0xEA
            {"???", 0, nullptr},// 0xEB
            {"???", 0, nullptr},// 0xEC
            {"???", 0, nullptr},// 0xED
            {"???", 0, nullptr},// 0xEE
            {"???", 0, nullptr},// 0xEF
            {"???", 0, nullptr},// 0xF0
            {"???", 0, nullptr},// 0xF1
            {"???", 0, nullptr},// 0xF2
            {"???", 0, nullptr},// 0xF3
            {"???", 0, nullptr},// 0xF4
            {"???", 0, nullptr},// 0xF5
            {"???", 0, nullptr},// 0xF6
            {"???", 0, nullptr},// 0xF7
            {"???", 0, nullptr},// 0xF8
            {"???", 0, nullptr},// 0xF9
            {"???", 0, nullptr},// 0xFA
            {"???", 0, nullptr},// 0xFB
            {"???", 0, nullptr},// 0xFC
            {"???", 0, nullptr},// 0xFD
            {"???", 0, nullptr},// 0xFE
            {"???", 0, nullptr},// 0xFF
    };
};


#endif //EMULATOR_CPU_H
