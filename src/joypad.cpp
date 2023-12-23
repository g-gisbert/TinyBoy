#include "joypad.h"


bool Joypad::checkButtonPressed(uint8_t newState) {
    uint8_t lsbA = previousState & 0x0F;
    uint8_t lsbB = newState & 0x0F;
    return ((lsbA & ~lsbB) != 0);
}

void Joypad::checkState() {

    uint8_t joypadState = memory.JOYP() | 0x0F;

    if (!(joypadState & 0x20)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            joypadState = joypadState & ~(A);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
            joypadState = joypadState & ~(B);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
            joypadState = joypadState & ~(SELECT);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            joypadState = joypadState & ~(START);
        }
    }
    if (!(joypadState & 0x10)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            joypadState = joypadState & ~(RIGHT >> 4);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            joypadState = joypadState & ~(LEFT >> 4);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            joypadState = joypadState & ~(UP >> 4);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            joypadState = joypadState & ~(DOWN >> 4);
        }
    }

    if (checkButtonPressed(joypadState))
        memory.IF() |= (0x01 << 4);
    memory.JOYP() = joypadState;
    previousState = joypadState;
}