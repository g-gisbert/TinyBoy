#include "gameBoy.h"

GameBoy::GameBoy() : running(true), renderer(memory), cpu(memory), ppu(memory, renderer) {
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
        renderer.step(running);
        interruptStep(cpu);
        int& cycles = cpu.step();
        ppu.step(cycles);
    }
    std::cout << "End" << std::endl;
}
