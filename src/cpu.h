#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "registers.h"
#include "cartridge.h"
#include <cstdio>
#include <iostream>

class CPU {
public:
    CPU();
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
    void jp_nn(uint16_t nn); // 195 (0xC3)


    constexpr static Instruction instructions_set[256] = {
            {.name="NOP", .byteLength=1, .funcCallVoid=nop},                // 0
            {.name="LD BC, 0x%02X", .byteLength=3, .funcCall16=ld_bc_nn},   // 1
            {"???", 0,           nullptr},// 2
            {"???", 0,           nullptr},// 3
            {"???", 0,           nullptr},// 4
            {"???", 0,           nullptr},// 5
            {"???", 0,           nullptr},// 6
            {"???", 0,           nullptr},// 7
            {"???", 0,           nullptr},// 8
            {"???", 0,           nullptr},// 9
            {"???", 0,           nullptr},// 10
            {"???", 0,           nullptr},// 11
            {"???", 0, nullptr},// 12
            {"???", 0, nullptr},// 13
            {"???", 0, nullptr},// 14
            {"???", 0, nullptr},// 15
            {"???", 0, nullptr},// 16
            {"???", 0, nullptr},// 17
            {"???", 0, nullptr},// 18
            {"???", 0, nullptr},// 19
            {"???", 0, nullptr},// 20
            {"???", 0, nullptr},// 21
            {"???", 0, nullptr},// 22
            {"???", 0, nullptr},// 23
            {"???", 0, nullptr},// 24
            {"???", 0, nullptr},// 25
            {"???", 0, nullptr},// 26
            {"???", 0, nullptr},// 27
            {"???", 0, nullptr},// 28
            {"???", 0, nullptr},// 29
            {"???", 0, nullptr},// 30
            {"???", 0, nullptr},// 31
            {"???", 0, nullptr},// 32
            {"???", 0, nullptr},// 33
            {"???", 0, nullptr},// 34
            {"???", 0, nullptr},// 35
            {"???", 0, nullptr},// 36
            {"???", 0, nullptr},// 37
            {"???", 0, nullptr},// 38
            {"???", 0, nullptr},// 39
            {"???", 0, nullptr},// 40
            {"???", 0, nullptr},// 41
            {"???", 0, nullptr},// 42
            {"???", 0, nullptr},// 43
            {"???", 0, nullptr},// 44
            {"???", 0, nullptr},// 45
            {"???", 0, nullptr},// 46
            {"???", 0, nullptr},// 47
            {"???", 0, nullptr},// 48
            {"???", 0, nullptr},// 49
            {"???", 0, nullptr},// 50
            {"???", 0, nullptr},// 51
            {"???", 0, nullptr},// 52
            {"???", 0, nullptr},// 53
            {"???", 0, nullptr},// 54
            {"???", 0, nullptr},// 55
            {"???", 0, nullptr},// 56
            {"???", 0, nullptr},// 57
            {"???", 0, nullptr},// 58
            {"???", 0, nullptr},// 59
            {"???", 0, nullptr},// 60
            {"???", 0, nullptr},// 61
            {"???", 0, nullptr},// 62
            {"???", 0, nullptr},// 63
            {"???", 0, nullptr},// 64
            {"???", 0, nullptr},// 65
            {"???", 0, nullptr},// 66
            {"???", 0, nullptr},// 67
            {"???", 0, nullptr},// 68
            {"???", 0, nullptr},// 69
            {"???", 0, nullptr},// 70
            {"???", 0, nullptr},// 71
            {"???", 0, nullptr},// 72
            {"???", 0, nullptr},// 73
            {"???", 0, nullptr},// 74
            {"???", 0, nullptr},// 75
            {"???", 0, nullptr},// 76
            {"???", 0, nullptr},// 77
            {"???", 0, nullptr},// 78
            {"???", 0, nullptr},// 79
            {"???", 0, nullptr},// 80
            {"???", 0, nullptr},// 81
            {"???", 0, nullptr},// 82
            {"???", 0, nullptr},// 83
            {"???", 0, nullptr},// 84
            {"???", 0, nullptr},// 85
            {"???", 0, nullptr},// 86
            {"???", 0, nullptr},// 87
            {"???", 0, nullptr},// 88
            {"???", 0, nullptr},// 89
            {"???", 0, nullptr},// 90
            {"???", 0, nullptr},// 91
            {"???", 0, nullptr},// 92
            {"???", 0, nullptr},// 93
            {"???", 0, nullptr},// 94
            {"???", 0, nullptr},// 95
            {"???", 0, nullptr},// 96
            {"???", 0, nullptr},// 97
            {"???", 0, nullptr},// 98
            {"???", 0, nullptr},// 99
            {"???", 0, nullptr},// 100
            {"???", 0, nullptr},// 101
            {"???", 0, nullptr},// 102
            {"???", 0, nullptr},// 103
            {"???", 0, nullptr},// 104
            {"???", 0, nullptr},// 105
            {"???", 0, nullptr},// 106
            {"???", 0, nullptr},// 107
            {"???", 0, nullptr},// 108
            {"???", 0, nullptr},// 109
            {"???", 0, nullptr},// 110
            {"???", 0, nullptr},// 111
            {"???", 0, nullptr},// 112
            {"???", 0, nullptr},// 113
            {"???", 0, nullptr},// 114
            {"???", 0, nullptr},// 115
            {"???", 0, nullptr},// 116
            {"???", 0, nullptr},// 117
            {"???", 0, nullptr},// 118
            {"???", 0, nullptr},// 119
            {"???", 0, nullptr},// 120
            {"???", 0, nullptr},// 121
            {"???", 0, nullptr},// 122
            {"???", 0, nullptr},// 123
            {"???", 0, nullptr},// 124
            {"???", 0, nullptr},// 125
            {"???", 0, nullptr},// 126
            {"???", 0, nullptr},// 127
            {"???", 0, nullptr},// 128
            {"???", 0, nullptr},// 129
            {"???", 0, nullptr},// 130
            {"???", 0, nullptr},// 131
            {"???", 0, nullptr},// 132
            {"???", 0, nullptr},// 133
            {"???", 0, nullptr},// 134
            {"???", 0, nullptr},// 135
            {"???", 0, nullptr},// 136
            {"???", 0, nullptr},// 137
            {"???", 0, nullptr},// 138
            {"???", 0, nullptr},// 139
            {"???", 0, nullptr},// 140
            {"???", 0, nullptr},// 141
            {"???", 0, nullptr},// 142
            {"???", 0, nullptr},// 143
            {"???", 0, nullptr},// 144
            {"???", 0, nullptr},// 145
            {"???", 0, nullptr},// 146
            {"???", 0, nullptr},// 147
            {"???", 0, nullptr},// 148
            {"???", 0, nullptr},// 149
            {"???", 0, nullptr},// 150
            {"???", 0, nullptr},// 151
            {"???", 0, nullptr},// 152
            {"???", 0, nullptr},// 153
            {"???", 0, nullptr},// 154
            {"???", 0, nullptr},// 155
            {"???", 0, nullptr},// 156
            {"???", 0, nullptr},// 157
            {"???", 0, nullptr},// 158
            {"???", 0, nullptr},// 159
            {"???", 0, nullptr},// 160
            {"???", 0, nullptr},// 161
            {"???", 0, nullptr},// 162
            {"???", 0, nullptr},// 163
            {"???", 0, nullptr},// 164
            {"???", 0, nullptr},// 165
            {"???", 0, nullptr},// 166
            {"???", 0, nullptr},// 167
            {"???", 0, nullptr},// 168
            {"???", 0, nullptr},// 169
            {"???", 0, nullptr},// 170
            {"???", 0, nullptr},// 171
            {"???", 0, nullptr},// 172
            {"???", 0, nullptr},// 173
            {"???", 0, nullptr},// 174
            {"???", 0, nullptr},// 175
            {"???", 0, nullptr},// 176
            {"???", 0, nullptr},// 177
            {"???", 0, nullptr},// 178
            {"???", 0, nullptr},// 179
            {"???", 0, nullptr},// 180
            {"???", 0, nullptr},// 181
            {"???", 0, nullptr},// 182
            {"???", 0, nullptr},// 183
            {"???", 0, nullptr},// 184
            {"???", 0, nullptr},// 185
            {"???", 0, nullptr},// 186
            {"???", 0, nullptr},// 187
            {"???", 0, nullptr},// 188
            {"???", 0, nullptr},// 189
            {"???", 0, nullptr},// 190
            {"???", 0, nullptr},// 191
            {"???", 0, nullptr},// 192
            {"???", 0, nullptr},// 193
            {"???", 0, nullptr},// 194
            {.name="JP 0x%04X", .byteLength=3, .funcCall16=jp_nn},// 195
            {"???", 0, nullptr},// 196
            {"???", 0, nullptr},// 197
            {"???", 0, nullptr},// 198
            {"???", 0, nullptr},// 199
            {"???", 0, nullptr},// 200
            {"???", 0, nullptr},// 201
            {"???", 0, nullptr},// 202
            {"???", 0, nullptr},// 203
            {"???", 0, nullptr},// 204
            {"???", 0, nullptr},// 205
            {"???", 0, nullptr},// 206
            {"???", 0, nullptr},// 207
            {"???", 0, nullptr},// 208
            {"???", 0, nullptr},// 209
            {"???", 0, nullptr},// 210
            {"???", 0, nullptr},// 211
            {"???", 0, nullptr},// 212
            {"???", 0, nullptr},// 213
            {"???", 0, nullptr},// 214
            {"???", 0, nullptr},// 215
            {"???", 0, nullptr},// 216
            {"???", 0, nullptr},// 217
            {"???", 0, nullptr},// 218
            {"???", 0, nullptr},// 219
            {"???", 0, nullptr},// 220
            {"???", 0, nullptr},// 221
            {"???", 0, nullptr},// 222
            {"???", 0, nullptr},// 223
            {"???", 0, nullptr},// 224
            {"???", 0, nullptr},// 225
            {"???", 0, nullptr},// 226
            {"???", 0, nullptr},// 227
            {"???", 0, nullptr},// 228
            {"???", 0, nullptr},// 229
            {"???", 0, nullptr},// 230
            {"???", 0, nullptr},// 231
            {"???", 0, nullptr},// 232
            {"???", 0, nullptr},// 233
            {"???", 0, nullptr},// 234
            {"???", 0, nullptr},// 235
            {"???", 0, nullptr},// 236
            {"???", 0, nullptr},// 237
            {"???", 0, nullptr},// 238
            {"???", 0, nullptr},// 239
            {"???", 0, nullptr},// 240
            {"???", 0, nullptr},// 241
            {"???", 0, nullptr},// 242
            {"???", 0, nullptr},// 243
            {"???", 0, nullptr},// 244
            {"???", 0, nullptr},// 245
            {"???", 0, nullptr},// 246
            {"???", 0, nullptr},// 247
            {"???", 0, nullptr},// 248
            {"???", 0, nullptr},// 249
            {"???", 0, nullptr},// 250
            {"???", 0, nullptr},// 251
            {"???", 0, nullptr},// 252
            {"???", 0, nullptr},// 253
            {"???", 0, nullptr},// 254
            {"???", 0, nullptr},// 255
    };
};


#endif //EMULATOR_CPU_H
