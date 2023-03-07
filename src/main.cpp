#include <iostream>
#include "gameBoy.h"
#include "registers.h"

int main()
{
    GameBoy emulation;
    emulation.run();

    return 0;
}

/** TODO
 * 256 instructions to fill
 * PPU
 */
