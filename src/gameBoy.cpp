#include "gameBoy.h"
#include <fstream>

GameBoy::GameBoy() : renderer(memory), cpu(memory), ppu(memory, renderer), timer(memory),
                     joypad(memory, renderer.window), running(true), pausing(false) {
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
        renderer.callback(pausing, memory);
        if (pausing) {
            std::ofstream MyFile("filename.txt");
            std::string visu = cpu.debugFile.str();
            std::transform(visu.begin(), visu.end(), visu.begin(), ::toupper);
            MyFile << visu;
            continue;
        }
        interruptStep(cpu);
        int& cycles = cpu.step();
        ppu.step(cycles);

        //    blarggs test - serial output
        if (memory.read8(0xff02) == 0x81) {
            char c = memory.read8(0xff01);
            printf("%c", c);
            memory.write8(0xff02, 0x0);
        }

    }
    std::cout << "End" << std::endl;
}
