#ifndef EMULATOR_JOYPAD_H
#define EMULATOR_JOYPAD_H

#include "memory.h"
#include "interrupts.h"
#include <SFML/Window.hpp>

enum JOYPAD_INPUT : uint8_t {
    A = (1 << 0),
    B = (1 << 1),
    SELECT = (1 << 2),
    START = (1 << 3),
    RIGHT = (1 << 4),
    LEFT = (1 << 5),
    UP = (1 << 6),
    DOWN = (1 << 7)
};

class Joypad {
public:
    Joypad(Memory& mem, sf::Window& win) : memory(mem), window(win), previousState(0xFF) {}

    void checkState();
    bool checkButtonPressed(uint8_t newState);

private:
    Memory& memory;
    sf::Window& window;

    uint8_t previousState;
};


#endif //EMULATOR_JOYPAD_H
