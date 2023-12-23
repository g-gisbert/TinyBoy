#include "display.h"

Display::Display(Memory& memo) : window(sf::VideoMode(160, 144), "Full GameBoy Emulation"),
                                 debugWindow(sf::VideoMode(200, 100), "Debug"){
    window.setSize(sf::Vector2u(640, 576));
    debugWindow.setSize(sf::Vector2u(800, 400));
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



void Display::callback() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            //window.close();
            //running = false;
            //pausing = !pausing;

        }
    }

}

void Display::showDebug() {
    debugWindow.clear();

    image.create(160, 144, reinterpret_cast<uint8_t*>(screenBuffer));
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    window.draw(sprite);

    debugWindow.display();
}