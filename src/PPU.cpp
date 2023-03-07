#include "PPU.h"

void PPU::step(int& cycles) {

    if(cycles >= 456) {
        memory.LY()++;
        cycles -= 456;
    }

    if (cycles <= 80)
        mode = OAM_SEARCH;
    else if (cycles <= 168)
        mode = PIXEL_TRANSFER;
    else if (cycles <= 208)
        mode = H_BLANK;

    if (memory.LY() >= 144) {
        mode = V_BLANK;
    }
    if (memory.LY() == 154)
        memory.LY() = 0;


}