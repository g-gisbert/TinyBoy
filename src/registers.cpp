#include "registers.h"

void Registers::setFlag(int flag, bool value) {
    if (flag & ZERO_FLAG) {
        f = (value << 7) | f;
        f = ((!value << 7) ^ 0xff) & f;
    }
    if (flag & SUB_FLAG) {
        f = (value << 6) | f;
        f = ((!value << 6) ^ 0xff) & f;
    }
    if (flag & HALF_CARRY_FLAG) {
        f = (value << 5) | f;
        f = ((!value << 5) ^ 0xff) & f;
    }
    if (flag & CARRY_FLAG) {
        f = (value << 4) | f;
        f = ((!value << 4) ^ 0xff) & f;
    }
}

bool Registers::checkFlagSet(int flag) const {
    return !checkFlagClear(flag);
}
bool Registers::checkFlagClear(int flag) const {
    if (flag == ZERO_FLAG) {
        return !bool(f & (1 << 7));
    }
    if (flag == SUB_FLAG) {
        return !bool(f & (1 << 6));
    }
    if (flag == HALF_CARRY_FLAG) {
        return !bool(f & (1 << 5));
    }
    if (flag == CARRY_FLAG) {
        return !bool(f & (1 << 4));
    }
    return false;
}

