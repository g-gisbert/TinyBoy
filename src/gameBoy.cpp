#include "gameBoy.h"

GameBoy::GameBoy() {
    setupSequence();
}

void GameBoy::setupSequence() {
    loadCartridge(std::string{"../roms/zelda.gb"});
}

void GameBoy::loadCartridge(std::string filename) {
    cart.loadRom(filename);
    cart.printInfo();
}