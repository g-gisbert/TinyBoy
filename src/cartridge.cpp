#include "cartridge.h"

void Cartridge::loadRom(std::string& filename) {
    // Open file
    std::ifstream rom(filename, std::ios::binary);
    if(!rom.is_open()) {
        std::cerr << "Error : unable to open file : " << filename << std::endl;
    }

    // Size
    rom.seekg(0, std::ios::end);
    size_t size = rom.tellg();
    rom.seekg(0, std::ios::beg);

    // Copy data
    romData = new char[size];
    rom.read(romData, size);

    title = std::string(&romData[0x0134], &romData[0x0143]);
    cartridgeType = romData[0x0147];
    romSize = romData[0x0148];
    ramSize = romData[0x0149];

}

void Cartridge::printInfo() {
    std::cout << "### Cartridge info ###" << std::endl;
    std::cout << "\tTitle : " << title << std::endl;
    std::cout << "\tCartridge Type : " << std::hex << int(cartridgeType) << " (" << getCartridgeType(cartridgeType) << ")" << std::endl;
    std::cout << "\tROM Size : " << std::hex << int(romSize) << " (" << getRomSize(romSize) << ")" << std::endl;
    std::cout << "\tRAM Size : " << std::hex << int(ramSize) << " (" << getRamSize(ramSize) << ")" << std::endl;
}

std::string Cartridge::getCartridgeType(uint8_t x) {
    std::string ct = "???";

    switch (x) {
        case 0x00:
            ct = "ROM ONLY";
            break;
        case 0x01:
            ct = "MBC1";
            break;
        case 0x02:
            ct = "MBC1+RAM";
            break;
        case 0x03:
            ct = "MBC1+RAM+BATTERY";
            break;
        case 0x05:
            ct = "MBC2";
            break;
        case 0x06:
            ct = "MBC2+BATTERY";
            break;
        case 0x08:
            ct = "ROM+RAM";
            break;
        case 0x09:
            ct = "ROM+RAM+BATTERY";
            break;
        case 0x0B:
            ct = "MMM01";
            break;
        case 0x0C:
            ct = "MMM01+RAM";
            break;
        case 0x0D:
            ct = "MMM01+RAM+BATTERY";
            break;
        case 0x0F:
            ct = "MBC3+TIMER+BATTERY";
            break;
        case 0x10:
            ct = "MBC3+TIMER+RAM+BATTERY";
            break;
        case 0x11:
            ct = "MBC3";
            break;
        case 0x12:
            ct = "MBC3+RAM";
            break;
        case 0x13:
            ct = "MBC3+RAM+BATTERY";
            break;
        case 0x19:
            ct = "MBC5";
            break;
        case 0x1A:
            ct = "MBC5+RAM";
            break;
        case 0x1B:
            ct = "MBC5+RAM+BATTERY";
            break;
        case 0x1C:
            ct = "MBC5+RUMBLE";
            break;
        case 0x1D:
            ct = "MBC5+RUMBLE+RAM";
            break;
        case 0x1E:
            ct = "MBC5+RUMBLE+RAM+BATTERY";
            break;
        case 0x20:
            ct = "MBC6";
            break;
        case 0x22:
            ct = "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
            break;
        case 0xFC:
            ct = "POCKET CAMERA";
            break;
        case 0xFD:
            ct = "BANDAI TAMA5";
            break;
        case 0xFE:
            ct = "HuC3";
            break;
        case 0xFF:
            ct = "HuC1+RAM+BATTERY";
            break;
    }

    return ct;
}

std::string Cartridge::getRomSize(uint8_t x) {
    std::string rs = "???";

    switch (x) {
        case 0x00:
            rs = "32 KiB";
            break;
        case 0x01:
            rs = "64 KiB";
            break;
        case 0x02:
            rs = "128 KiB";
            break;
        case 0x03:
            rs = "256 KiB";
            break;
        case 0x04:
            rs = "512 KiB";
            break;
        case 0x05:
            rs = "1 MiB";
            break;
        case 0x06:
            rs = "2 MiB";
            break;
        case 0x07:
            rs = "4 MiB";
            break;
        case 0x08:
            rs = "8 MiB";
            break;
        case 0x52:
            rs = "1.1 MiB";
            break;
        case 0x53:
            rs = "1.2 MiB";
            break;
        case 0x54:
            rs = "1.5 MiB";
            break;
    }

    return rs;
}

std::string Cartridge::getRamSize(uint8_t x) {
    std::string rs = "???";

    switch (x) {
        case 0x00:
            rs = "0";
            break;
        case 0x02:
            rs = "8 KiB";
            break;
        case 0x03:
            rs = "32 KiB";
            break;
        case 0x04:
            rs = "128 KiB";
            break;
        case 0x05:
            rs = "64 KiB";
            break;
    }
    return rs;
}

uint8_t Cartridge::readCart(uint16_t pc) {
    return romData[pc];
}
