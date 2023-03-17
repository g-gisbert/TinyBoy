#include "display.h"

Display::Display(Memory& memo) : window(sf::VideoMode(160, 144), "Full GameBoy Emulation") {

}

void Display::renderScreen() {
    window.clear();

    /*for (int i = 0; i < 160*144; ++i) {
        screenBuffer[i] = Pixel{static_cast<uint8_t>(i%160), static_cast<uint8_t>(i/160), 75, 255};
    }*/


    image.create(160, 144, reinterpret_cast<uint8_t*>(screenBuffer));
    //image.create(160, 144, screenBuffer);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // Draw the sprite onto the window
    window.draw(sprite);

    // Display the window
    window.display();
}

void Display::step(bool& running) {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
            running = false;
        }
    }

}