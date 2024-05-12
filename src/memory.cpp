#include "memory.h"

// Fetch Memory
uint16_t Memory::read16(uint16_t address) {
    uint8_t lsb = read8(address);
    uint8_t msb = read8(address+1);
    return (uint16_t(msb) << 8) + lsb;
}

uint8_t Memory::read8(uint16_t address) {
    //std::cout <<  "readBug before" <<std::endl;
    uint8_t val = 0;
    if(address < 0x8000) { // ROM
        val = cart->readCart(address);
    } else if (address < 0xA000) { // VRAM
        val =  VRAM[address - 0x8000];
    } else if (address < 0xC000) { // extern RAM
        val =  cart->readCart(address);
    } else if (address < 0xE000) { // WRAM
        val =  WRAM[address - 0xC000];
    } else if (address < 0xFE00) { // unused
    } else if (address < 0xFEA0) { // OAM
        val =  OAM[address - 0xFE00];
    } else if (address < 0xFF00) { // unused
    } else if (address < 0xFF80) { // I/O Registers
        val =  IORegisters[address - 0xFF00];
    } else if (address < 0xFFFE) { // HRAM
        val =  HRAM[address - 0xFF80];
    } else if (address == 0xFFFF) { // IME
        val = IE_;
    }
    //std::cout <<  "readBug after" <<std::endl;
    return val;
}

void Memory::write16(uint16_t address, uint16_t value) {
    write8(address, value & 0x00FF);
    write8(address+1, (value & 0xFF00) >> 8);
}

void Memory::write8(uint16_t address, uint8_t value) {
    //std::cout <<  "writeBug before" <<std::endl;

    if(address < 0x8000) { // ROM
        cart->writeCart(address, value);
    } else if (address < 0xA000) { // VRAM
        VRAM[address - 0x8000] = value;
    } else if (address < 0xC000) { // extern RAM
        cart->writeCart(address, value);
    } else if (address < 0xE000) { // WRAM
        WRAM[address - 0xC000] = value;
    } else if (address < 0xFE00) { // unused
    } else if (address < 0xFEA0) { // OAM
        OAM[address - 0xFE00] = value;
    } else if (address < 0xFF00) { // unused
    } else if (address == 0xFF46) { // DMA Transfer
        DMATransfer(value);
    } else if (address < 0xFF80) { // I/O Registers
        IORegisters[address - 0xFF00] = value;
    } else if (address < 0xFFFE) { // HRAM
        HRAM[address - 0xFF80] = value;
    } else if (address == 0xFFFF) { // IME
        IE_ = value;
    }
    //std::cout <<  "writeBug after" <<std::endl;
}

void Memory::DMATransfer(uint16_t startAddress) {
    for (uint16_t i = 0; i < 0xA0; ++i) {
        uint16_t addressSource = (startAddress << 8) + i;
        uint16_t addressDest = 0xFE00 + i;
        write8(addressDest, read8(addressSource));
    }
    //std::cout << "DMA transferred : " << startAddress;
}

void Memory::debugState() {

}