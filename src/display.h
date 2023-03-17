#ifndef EMULATOR_DISPLAY_H
#define EMULATOR_DISPLAY_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "memory.h"

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

constexpr Pixel colors[4] = {Pixel{10,10,10,255}, Pixel{92,92,92,255},
                             Pixel{174,174,174,255}, Pixel{255, 255, 255, 255}};

class Display {
public:
    Display(Memory& memo);

    void renderScreen();
    void step(bool& running);


    sf::RenderWindow window;
    Pixel screenBuffer[160*144];
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};


#endif //EMULATOR_DISPLAY_H
