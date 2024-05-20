#include "display.h"

Display::Display(Memory& memo) : window(sf::VideoMode(160, 144), "TinyBoy") {
    window.setSize(sf::Vector2u(640, 576));
}

void Display::renderScreen() {
    window.clear();

    image.create(160, 144, reinterpret_cast<uint8_t*>(screenBuffer));

    texture.loadFromImage(image);
    sprite.setTexture(texture);

    window.draw(sprite);

    window.display();
}



void Display::callback(bool& running) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            running = false;
        }
    }

}
