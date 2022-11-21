#ifndef EMULATOR_CARTRIDGE_H
#define EMULATOR_CARTRIDGE_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>

class Cartridge {
public:
    Cartridge() = default;
    ~Cartridge() { delete romData; }
    void loadRom(std::string filename);
    void printInfo();
    uint8_t getOpcode(uint16_t pc);

private:
    std::string getCartridgeType(uint8_t x);
    std::string getRomSize(uint8_t x);
    std::string getRamSize(uint8_t x);

    char* romData;
    std::string title;
    uint8_t cartridgeType;
    uint8_t romSize;
    uint8_t ramSize;
};


#endif //EMULATOR_CARTRIDGE_H
