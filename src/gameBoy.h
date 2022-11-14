#ifndef EMULATOR_GAMEBOY_H
#define EMULATOR_GAMEBOY_H

#include "cpu.h"
#include "cartridge.h"
#include <string>


class GameBoy {
public:
    GameBoy();
    void run();

private:
    void setupSequence();
    void loadCartridge(std::string filename);

    CPU cpu;
    Cartridge cart;

};


#endif //EMULATOR_GAMEBOY_H
