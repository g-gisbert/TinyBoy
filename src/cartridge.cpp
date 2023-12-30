#include "cartridge.h"

std::unique_ptr<Cartridge> loadRom(std::string& filename) {
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
    char* romData = new char [size];
    rom.read(romData, size);

    CartridgeInfo info;
    info.title = std::string(&romData[0x0134], &romData[0x0143]);
    info.cartridgeType = romData[0x0147];
    info.romSize = romData[0x0148];
    info.ramSize = romData[0x0149];

    switch (info.cartridgeType) {
        case 0x00:
            return std::make_unique<NoMBC>(romData, info);
        case 0x01:
        case 0x02:
        case 0x03:
            return std::make_unique<MBC1>(romData, info, info.getRamSize());
        case 0x11:
        case 0x12:
        case 0x13:
            return std::make_unique<MBC3>(romData, info, info.getRamSize());
        default:
            std::cerr << "MBC type not implemented" << std::endl;
    };

}

void Cartridge::printInfo() {
    std::cout << "### Cartridge info ###" << std::endl;
    std::cout << "\tTitle : " << info.title << std::endl;
    std::cout << "\tCartridge Type : " << std::hex << int(info.cartridgeType) << " (" << info.getCartridgeType() << ")" << std::endl;
    std::cout << "\tROM Size : " << std::hex << int(info.romSize) << " (" << info.getRomSize() << ")" << std::endl;
    std::cout << "\tRAM Size : " << std::hex << int(info.ramSize) << " (0x" << info.getRamSize() << " B)" << std::endl;
}

std::string CartridgeInfo::getCartridgeType() const {
    std::string ct = "???";

    switch (cartridgeType) {
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

std::string CartridgeInfo::getRomSize() const {
    std::string rs = "???";

    switch (romSize) {
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

int CartridgeInfo::getRamSize() const {

    switch (ramSize) {
        case 0x00:
            return 0;
        case 0x02:
            return 8192;
        case 0x03:
            return 32768;
        case 0x04:
            return 131072;
        case 0x05:
            return 65536;
    }
    return 0;
}

uint8_t Cartridge::readCart(uint16_t address) {
    return romData[address];
}

uint8_t MBC1::readCart(uint16_t address) {
    if (address < 0x4000) {
        return romData[address];
    } else if (address < 0x8000) {
        return romData[address - 0x4000 + 0x4000 * romBankNumber];
    } else if (address >= 0xA000 && address < 0xC000) {
        if (!ramEnabled)
            return 0xFF;
        return ramData[address - 0xA000 + 0x2000 * ramBankNumber];
    }
    return 0xFF;
}

void v(int  a) {}

void MBC1::writeCart(uint16_t address, uint8_t value) {

    if (address < 0x2000) { // 0x0000–0x1FFF
        ramEnabled = (value & 0x0F) == 0x0A;
    } else if (address < 0x4000) { // 0x2000–0x3FFF
        romBankNumber = value & 0x1F;
        romBankNumber = romBankNumber ? romBankNumber : 0x01;
    } else if (address < 0x6000) { // 0x4000–0x5FFF
        ramBankNumber = value & 0x03;
    } else if (address >= 0xA000 && address < 0xC000) {
        if (!ramEnabled)
            return;
        ramData[address - 0xA000 + 0x2000 * ramBankNumber] = value;
    }
}

uint8_t MBC3::readCart(uint16_t address) {
    if (address < 0x4000) {
        return romData[address];
    } else if (address < 0x8000) {
        return romData[address - 0x4000 + 0x4000 * romBankNumber];
    } else if (address >= 0xA000 && address < 0xC000) {
        if (!ramEnabled)
            return 0xFF;
        return ramData[address - 0xA000 + 0x2000 * ramBankNumber];
    }
    return 0xFF;
}

void MBC3::writeCart(uint16_t address, uint8_t value) {
    if (address < 0x2000) // 0x0000–0x1FFF
        ramEnabled = (value & 0x0F) == 0x0A;
    else if (address < 0x4000){ // 0x2000–0x3FFF
        romBankNumber = value & 0x7F;
        romBankNumber = romBankNumber ? romBankNumber : 0x01;
    } else if (address < 0x6000) // 0x4000–0x5FFF
        ramBankNumber = value & 0x03;
    else if(address >= 0xA000 && address < 0xC000) {
        if (!ramEnabled)
            return;
        ramData[address - 0xA000 + 0x2000 * ramBankNumber] = value;
    }
}