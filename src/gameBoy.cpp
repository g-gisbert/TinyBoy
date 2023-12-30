#include "gameBoy.h"
#include <fstream>

GameBoy::GameBoy() : renderer(memory), cpu(memory), ppu(memory, renderer), timer(memory),
                     joypad(memory, renderer.window), running(true), pausing(false) {
    setupSequence();
}

void GameBoy::setupSequence() {
    loadCartridge(std::string{"../roms/zelda.gb"});
}

void GameBoy::loadCartridge(std::string filename) {
    memory.cart = loadRom(filename);
    memory.cart->printInfo();
}

void GameBoy::run() {

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    uint64_t totalTicks = 0;
    while(running) {
        //std::cout << "renderer b" << std::endl;
        renderer.callback(running);
        //std::cout << "renderer a" << std::endl;
        if (pausing) {
            std::ofstream MyFile("filename.txt");
            std::string visu = cpu.debugFile.str();
            std::transform(visu.begin(), visu.end(), visu.begin(), ::toupper);
            MyFile << visu;
            continue;
        }
        //std::cout << "joypad b" << std::endl;
        joypad.checkState();
        //std::cout << "joypad a" << std::endl;
        //std::cout << "interrupt b" << std::endl;
        interruptStep(cpu);
        //std::cout << "interrupt a" << std::endl;
        int cycles = cpu.step();
        totalTicks += cycles;

        //std::cout << "ppu b" << std::endl;
        ppu.step(cycles);
        //std::cout << "ppu a" << std::endl;
        //std::cout << "timer b" << std::endl;
        timer.step(cycles);
        //std::cout << "timer a" << std::endl;

        while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count()
                < totalTicks * 238.418579) {}

    }
    std::cout << "End" << std::endl;
}
