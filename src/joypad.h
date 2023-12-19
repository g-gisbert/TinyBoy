#ifndef EMULATOR_JOYPAD_H
#define EMULATOR_JOYPAD_H

#include "memory.h"
#include "interrupts.h"
#include <SFML/Window.hpp>

enum JOYP_INPUT {
    A, B, SELECT, START, RIGHT, LEFT, UP, DOWN
};

class Joypad {
public:
    Joypad(Memory& mem, sf::Window& win) : memory(mem), window(win) {}

    void checkState();

private:
    Memory& memory;
    sf::Window& window;
};


#endif //EMULATOR_JOYPAD_H
