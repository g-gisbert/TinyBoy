#include "gameBoy.h"
#include <fstream>

GameBoy::GameBoy(const std::string& filepath) : renderer(memory), cpu(memory), ppu(memory, renderer),
                            timer(memory), joypad(memory, renderer.window), running(true), pausing(false) {
    setupSequence(filepath);
}

void GameBoy::setupSequence(const std::string& filepath) {
    loadCartridge(filepath);
}

void GameBoy::loadCartridge(const std::string& filename) {
    memory.cart = loadRom(filename);
    memory.cart->printInfo();
}

void GameBoy::run() {

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    uint64_t totalTicks = 0;
    while(running) {

        renderer.callback(running);

        joypad.checkState();
        interruptStep(cpu);
        int cycles = cpu.step();
        totalTicks += cycles;

        ppu.step(cycles);
        timer.step(cycles);

        while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count()
                < totalTicks * 238.418579) {}

    }
    std::cout << "End" << std::endl;
}
