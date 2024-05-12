#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include "cartridge.h"
#include <cstring>
#include <memory>

struct Memory {

    Memory() {
        std::memset(VRAM, 0, 0x2000);
        std::memset(WRAM, 0, 0x2000);
        std::memset(OAM, 0, 0xA0);
        std::memset(IORegisters, 0, 0x80);
        std::memset(HRAM, 0, 0x7F);
        JOYP() = 0xCF;
        IE_ = 0x00;
        IME = false;
    }

    std::unique_ptr<Cartridge> cart; // 0x0000 - 0x7FFF
    uint8_t VRAM[0x2000]; // 0x8000 - 0x9FFF
    uint8_t WRAM[0x2000]; // 0xC000 - 0xDFFF
    uint8_t OAM[0xA0]; // 0xFE00 - 0xFE9F
    uint8_t IORegisters[0x80]; // 0xFF00 - 0xFF7F
    uint8_t HRAM[0x7F]; // 0xFF80 - 0xFFFE
    uint8_t IE_; // 0xFFFF
    bool IME;
    uint8_t tmpDebug = 0x90;


    void DMATransfer(uint16_t startAddress);
    void debugState();

    // fetch, read, write
    uint8_t read8(uint16_t address);
    uint16_t read16(uint16_t address);
    void write8(uint16_t address, uint8_t value);
    void write16(uint16_t address, uint16_t value);

    // Aliases
    uint8_t& JOYP() { return IORegisters[0x0]; }
    uint8_t& DIV() { return IORegisters[0x04]; }
    uint8_t& TIMA() { return IORegisters[0x05]; }
    uint8_t& TMA() { return IORegisters[0x06]; }
    uint8_t& TAC() { return IORegisters[0x07]; }
    uint8_t& LCDC() { return IORegisters[0x40]; }
    uint8_t& STAT() { return IORegisters[0x41]; }
    uint8_t& SCY() { return IORegisters[0x42]; }
    uint8_t& SCX() { return IORegisters[0x43]; }
    uint8_t& LY() {  return IORegisters[0x44]; }
    uint8_t& LYC() { return IORegisters[0x45]; }
    uint8_t& BGP() { return IORegisters[0x47]; }
    uint8_t& OBP0() { return IORegisters[0x48]; }
    uint8_t& OBP1() { return IORegisters[0x49]; }
    uint8_t& WY() { return IORegisters[0x4A]; }
    uint8_t& WX() { return IORegisters[0x4B]; }
    uint8_t& IF() { return IORegisters[0x0F]; }
    uint8_t& IE() { return IE_; }

};

#endif // EMULATOR_MEMORY_H
