#ifndef EMULATOR_REGISTERS_H
#define EMULATOR_REGISTERS_H

#include <memory>

enum {
    ZERO_FLAG = 1,
    SUB_FLAG = 2,
    HALF_CARRY_FLAG = 4,
    CARRY_FLAG = 8
};

struct Registers {
    union {
        struct {
            uint8_t f;
            uint8_t a;
        };
        uint16_t af;
    };
    union {
        struct {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };
    union {
        struct {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };
    union {
        struct {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };

    uint16_t sp;
    uint16_t pc;

    void setFlag(int flag, bool value);
    bool checkFlagSet(int flag) const;
    bool checkFlagClear(int flag) const;

};

#endif //EMULATOR_REGISTERS_H
