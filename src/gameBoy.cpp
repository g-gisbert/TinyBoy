#include "gameBoy.h"

GameBoy::GameBoy() : running(true), cpu(memory), ppu(memory) {
    setupSequence();
}

void GameBoy::setupSequence() {
    loadCartridge(std::string{"../roms/tetris.gb"});
}

void GameBoy::loadCartridge(std::string filename) {
    memory.cart.loadRom(filename);
    memory.cart.printInfo();
}

void GameBoy::run() {
    while(running) {
        int& cycles = cpu.step();
        ppu.step(cycles);
    }
}
