#include "PPU.h"

void v(uint8_t a) {

}

void PPU::step(int cycles) {

    /*uint8_t a = memory.LY();
    a += 1;
    //std::cout << +a << std::endl;
    memory.LY() = a;
    v(a);*/

    internalCycles += cycles;
    bool newLine = false;

    if(internalCycles >= 456) {
        memory.LY()++;
        newLine = true;
        internalCycles -= 456;
    }
    if (memory.LY() == 154) {
        display.renderScreen();
        memory.LY() = 0;
    }
    uint8_t LY = memory.LY();

    int modeBefore = mode;
    if (internalCycles <= 80)
        mode = OAM_SEARCH;
    else if (internalCycles <= 168)
        mode = PIXEL_TRANSFER;
    else if (internalCycles <= 208)
        mode = H_BLANK;


    if (LY >= 144)  {
        mode = V_BLANK;
    }
    if (LY == 144)  {
        memory.IF() |= 0x01;
    }


    if (LY < 144 && newLine) {
        if (memory.LCDC() & 0x01) {
            printBackground(LY);
            // window();
        }
        if (memory.LCDC() & 0x02)
            printSprites(LY);
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

void PPU::printBackground(int LY) {
    uint16_t tileDataOffset = (memory.LCDC() & 0x10) ? 0x8000 : 0x9000;
    uint16_t tileMapOffset = (memory.LCDC() & 0x08) ? 0x9C00 : 0x9800;
    int startingTile = memory.SCX() / 8;
    startingTile = 0;
    int offsetX = memory.SCX() % 8;
    offsetX = 0;
    int offsetTileY = memory.SCY() / 8;
    int offsetLineY = memory.SCY() % 8;

    for (int tile = startingTile; tile < startingTile + 21; ++tile) {
        //uint8_t tileNumber = memory.read8(tileMapOffset+32*(((LY/8)+offsetTileY)%32)+tile%32);
        uint8_t tileNumber = memory.read8(tileMapOffset+32*(LY/8)+tile%32);
        uint16_t tn = tileNumber;
        if (!(memory.LCDC() & 0x10))
            tn = int8_t(tileNumber);

        uint8_t lsbTile = memory.read8(tileDataOffset+16*tn+2*(LY%8));
        uint8_t msbTile = memory.read8(tileDataOffset+16*tn+2*(LY%8)+1);
        for (int pixel = 0; pixel < 8; ++pixel) {
            uint8_t id = ( (msbTile & (1 << (7-pixel))) >> (7-pixel) )*2 + ( (lsbTile & (1 << (7-pixel))) >> (7-pixel) );
            int relativePosition = 8*(tile-startingTile) + pixel - offsetX;
            if (relativePosition >= 0 && relativePosition < 160) {
                display.screenBuffer[160 * LY + relativePosition] = colors[(memory.BGP() & (0x03 << (2 * id))) >> (2 * id)];
                BGW1_3[relativePosition] = id;
            }
        }
    }
}

void PPU::printSprites(uint8_t LY) {
    for (int i = 0; i < 160; i += 4) {
        uint8_t y = memory.OAM[i] - 9;

        if (memory.LCDC() & 0x04) {
            if (y < LY-8 || y >= LY + 8)
                continue;
        } else {
            if (y < LY || y >= LY + 8)
                continue;
        }

        uint8_t x = memory.OAM[i+1] - 8;
        uint8_t tileId = memory.OAM[i+2];
        uint8_t attributes = memory.OAM[i+3];
        bool priority = attributes & 0x80;
        bool yFlip = attributes & 0x40;
        bool xFlip = attributes & 0x20;
        uint8_t palette = (attributes &  0x10) ? memory.OBP1() : memory.OBP0();

        uint8_t lsbTile = memory.read8(0x8000+16*tileId+2*(yFlip*(y-LY) + !yFlip*(7-y+LY)));
        uint8_t msbTile = memory.read8(0x8000+16*tileId+2*(yFlip*(y-LY) + !yFlip*(7-y+LY))+1);
        for (int pixel = 0; pixel < 8; ++pixel) {
            uint8_t id = ( (msbTile & (1 << (7-pixel))) >> (7-pixel) )*2 + ( (lsbTile & (1 << (7-pixel))) >> (7-pixel) );
            if (id != 0 && !(BGW1_3[x + pixel] && priority))
                display.screenBuffer[160 * LY + x + !xFlip*pixel + xFlip*(7 - pixel)] = colors[(palette & (0x03 << (2*id))) >> (2*id)];
        }
    }
}