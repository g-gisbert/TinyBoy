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

/*constexpr Pixel colors[4] = {Pixel{255, 255, 255, 255}, Pixel{174,174,174,255},
                             Pixel{92,92,92,255}, Pixel{10,10,10,255}};*/

/*constexpr Pixel colors[4] = {Pixel{255, 200, 255, 255}, Pixel{185,115,185,255},
                             Pixel{120,51,120,255}, Pixel{28,12,28,255}};*/

constexpr Pixel colors[4] = {Pixel{224, 248, 208, 255}, Pixel{136,192,112,255},
                             Pixel{52,104,86,255}, Pixel{8,24,32,255}};

class Display {
public:
    Display(Memory& memo);

    void renderScreen();
    void showDebug();
    void callback();


    sf::RenderWindow window;
    sf::RenderWindow debugWindow;
    Pixel screenBuffer[160*144];
    Pixel debugBuffer[16*8*24*8];
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};


#endif //EMULATOR_DISPLAY_H
