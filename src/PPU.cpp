#include "PPU.h"

void PPU::step(int& cycles) {

    bool newLine = false;

    if(cycles >= 456) {
        memory.LY()++;
        newLine = true;
        cycles -= 456;
    }
    if (memory.LY() == 154) {
        display.renderScreen();
        renderDebug();
        memory.LY() = 0;
    }
    uint8_t LY = memory.LY();

    int modeBefore = mode;
    if (cycles <= 80)
        mode = OAM_SEARCH;
    else if (cycles <= 168)
        mode = PIXEL_TRANSFER;
    else if (cycles <= 208)
        mode = H_BLANK;



    if (LY >= 144)  {
        mode = V_BLANK;
    }
    if (LY == 144)  {
        memory.IF() |= 0x01;
    }


    if (LY < 144 && newLine) {
        //for (int col = 0; col < 160; ++col) { // TODO select correct bank for map + data / (offset)
        //std::cout << std::dec << "LY : " << int(LY) << std::endl;
        for (int tile = 0; tile < 20; ++tile) { // TODO tile 9 scrollX/Y
            uint8_t tileNumber = memory.read8(0x9800+32*(LY/8)+tile);
            uint8_t lsbTile = memory.read8(0x8000+16*tileNumber+2*(LY%8));
            uint8_t msbTile = memory.read8(0x8000+16*tileNumber+2*(LY%8)+1);
            for (int pixel = 0; pixel < 8; ++pixel) {
                uint8_t id = ( (msbTile & (1 << (7-pixel))) >> (7-pixel) )*2 + ( (lsbTile & (1 << (7-pixel))) >> (7-pixel) );
                display.screenBuffer[160 * LY + 8*tile + pixel] = colors[id]; // TODO palette
            }
        }

        for (int i = 0; i < 160; i += 4) { // sprites
            uint8_t y = memory.OAM[i] - 8;

            if (y < LY || y >= LY + 8)
                continue;

            uint8_t x = memory.OAM[i+1] - 8;
            uint8_t tileId = memory.OAM[i+2];
            uint8_t attributes = memory.OAM[i+3]; // todo

            uint8_t lsbTile = memory.read8(0x8000+16*tileId+2*(7-y+LY));
            uint8_t msbTile = memory.read8(0x8000+16*tileId+2*(7-y+LY)+1);
            for (int pixel = 0; pixel < 8; ++pixel) {
                uint8_t id = ( (msbTile & (1 << (7-pixel))) >> (7-pixel) )*2 + ( (lsbTile & (1 << (7-pixel))) >> (7-pixel) );
                display.screenBuffer[160 * LY + x + pixel] = colors[id]; // TODO palette
            }
        }

        //std::cout << std::endl;
            //display.screenBuffer[160 * LY + col] = memory.read8(0x9800+32*LY+col);
    }


    statHandle(modeBefore);

}

void PPU::statHandle(int mb) {
    uint8_t& STAT = memory.STAT();

    // Update mode b0-1
    if (mode != mb) {
        STAT &= 0xFC;
        STAT |= mode;

        if (mode == H_BLANK && (STAT & (0x01 << 3)))
            memory.IF() |= (0x01 << 1);
        if (mode == V_BLANK && (STAT & (0x01 << 4)))
            memory.IF() |= (0x01 << 1);
        if (mode == OAM_SEARCH && (STAT & (0x01 << 5)))
            memory.IF() |= (0x01 << 1);
    }

    // lyc = ly b2
    if (memory.LYC() == memory.LY()) {
        STAT |= (0x1 << 2);
        if (STAT & (0x01 << 6))
            memory.IF() |= (0x01 << 1);
    } else {
        STAT &= ~(0x1 << 2);
    }
}

void PPU::renderDebug() {

}