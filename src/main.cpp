#include <iostream>
#include "gameBoy.h"

int main()
{
    GameBoy emulation;
    //emulation.run();

    emulation.cpu.regs.af = 0x1a8f;

    std::printf("a : 0x%02X\n", emulation.cpu.regs.a);
    std::printf("f : 0x%02X\n", emulation.cpu.regs.f);
    std::printf("af : 0x%04X\n\n", emulation.cpu.regs.af);
    emulation.cpu.regs.a = 0x12;
    std::printf("a : 0x%02X\n", emulation.cpu.regs.a);
    std::printf("f : 0x%02X\n", emulation.cpu.regs.f);
    std::printf("af : 0x%04X\n\n", emulation.cpu.regs.af);
    emulation.cpu.regs.f = 0xAE;
    std::printf("a : 0x%02X\n", emulation.cpu.regs.a);
    std::printf("f : 0x%02X\n", emulation.cpu.regs.f);
    std::printf("af : 0x%04X\n", emulation.cpu.regs.af);

    return 0;
}

/** TODO
 * cartridge in CPU
 * 256 instructions to fill
 */
