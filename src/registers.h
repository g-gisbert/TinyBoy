#ifndef EMULATOR_REGISTERS_H
#define EMULATOR_REGISTERS_H

#include <memory>

struct Registers {
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;

};

template<typename T>
void setBit(T& reg, int bit, int val) {

}

#endif //EMULATOR_REGISTERS_H
