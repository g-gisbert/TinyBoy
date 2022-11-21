#include "gameBoy.h"

GameBoy::GameBoy() : running(true) {
    setupSequence();
}

void GameBoy::setupSequence() {
    loadCartridge(std::string{"../roms/zelda.gb"});
}

void GameBoy::loadCartridge(std::string filename) {
    cart.loadRom(filename);
    cart.printInfo();
}

void GameBoy::run() {
    while(running) {
        cpu.step(cart.getOpcode(cpu.regs.pc));
    }
}
