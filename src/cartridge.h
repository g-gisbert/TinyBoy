#ifndef EMULATOR_CARTRIDGE_H
#define EMULATOR_CARTRIDGE_H

#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <cstring>

struct CartridgeInfo  {
    std::string title;
    uint8_t cartridgeType;
    uint8_t romSize;
    uint8_t ramSize;

    std::string getCartridgeType() const;
    std::string getRomSize() const;
    int getRamSize() const;
};


class Cartridge {
public:

    Cartridge(char* rom, CartridgeInfo inf) : romData(rom), info(std::move(inf)) {};
    virtual ~Cartridge() { delete romData; }

    void printInfo();
    virtual uint8_t readCart(uint16_t address);
    virtual void writeCart(uint16_t address, uint8_t value) {}
protected:
    char* romData;
    CartridgeInfo info;
};


class NoMBC : public Cartridge {
public:
    NoMBC(char* rom, CartridgeInfo inf) : Cartridge(rom, std::move(inf)) {}
};

class MBC1 : public Cartridge {
public:
    MBC1(char* rom, CartridgeInfo inf, int ramSize) : Cartridge(rom, std::move(inf)), ramData(new char[ramSize]) {
        std::memset(ramData, 0, ramSize);
    }

    uint8_t readCart(uint16_t address) override;
    void writeCart(uint16_t address, uint8_t value) override;
private:
    char* ramData;

    bool ramEnabled = false;
    uint8_t romBankNumber = 0x01;
    uint8_t ramBankNumber = 0x00;
    bool bankingMode = false;

};

class MBC3 : public Cartridge {
public:
    MBC3(char* rom, CartridgeInfo inf, int ramSize) : Cartridge(rom, std::move(inf)), ramData(new char[ramSize]) {
        std::memset(ramData, 0, ramSize);
    }

    uint8_t readCart(uint16_t address) override;
    void writeCart(uint16_t address, uint8_t value) override;
private:
    char* ramData;

    bool ramEnabled = false;
    uint8_t romBankNumber = 0x01;
    uint8_t ramBankNumber = 0x00;
    bool bankingMode = false;
};

std::unique_ptr<Cartridge> loadRom(std::string& filename);

#endif //EMULATOR_CARTRIDGE_H
