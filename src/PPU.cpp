#include "PPU.h"


/*void PPU::step(int cycles) {

    internalCycles += cycles;
    bool newLine = false;

    if(internalCycles >= 456) {
        memory.LY()++;
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
    else if (internalCycles <= 172+80)
        mode = PIXEL_TRANSFER;
    else if (internalCycles <= 204+172+80) {
        mode = H_BLANK;
        newLine = true;
    }


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
}*/


/*void PPU::step(int cycles) {

    internalCycles += cycles;
    switch (mode) {
        case 0: // HBLANK
            if(internalCycles >= 204){
                internalCycles -= 204;
                mode = 2;

                memory.LY() += 1;
                //compare_ly_lyc();
                uint8_t lyc = memory.read8(0xFF45);
                memory.STAT() &= ~(0x1 << 2);
                memory.STAT() |= (int(lyc == memory.LY()) << 2);
                if (lyc == memory.LY())
                    memory.IF() |= (0x01 << 1);

                if(memory.LY() == 144){
                    mode = 1;
                    display.renderScreen();
                    memory.IF() |= (0x01 << 0);
                    if(memory.STAT() & (0x01 << 4))
                        memory.IF() |= (0x01 << 1);
                } else if(memory.STAT() & (0x01 << 5))
                    memory.IF() |= (0x01 << 1);

                memory.write8(0xff41, (memory.read8(0xff41) & 0xFC) | (mode & 3));
            }
            break;
        case 1: // VBLANK
            if(internalCycles >= 456) {
                internalCycles -= 456;
                memory.LY() += 1;
                //compare_ly_lyc();
                uint8_t lyc = memory.read8(0xFF45);
                memory.STAT() &= ~(0x1 << 2);
                memory.STAT() |= (int(lyc == memory.LY()) << 2);
                if (lyc == memory.LY())
                    memory.IF() |= (0x01 << 1);

                if(memory.LY() == 153){
                    memory.LY() = 0;
                    mode = 2;
                    memory.write8(0xff41, (memory.read8(0xff41) & 0xFC) | (mode & 3));
                    if(memory.STAT() & (0x01 << 5))
                        memory.IF() |= (0x01 << 1);
                }
            }

            break;
        case 2: // OAM
            if(internalCycles >= 80){
                internalCycles -=80;
                mode = 3;
                memory.write8(0xff41, (memory.read8(0xff41) & 0xFC) | (mode & 3));
            }
            break;
        case 3: // VRAM
            if(internalCycles >= 172){
                internalCycles-=172;
                mode = 0;
                if (memory.LCDC() & 0x01) {
                    printBackground(memory.LY());
                    // window();
                }
                if (memory.LCDC() & 0x02)
                    printSprites(memory.LY());
                memory.write8(0xff41, (memory.read8(0xff41) & 0xFC) | (mode & 3));

                if(memory.STAT() & (0x01 << 3))
                    memory.IF() |= (0x01 << 1);
            }
            break;
    }
}*/
inline void PPU::changeMode(int m) {
    memory.STAT() &= 0xFC;
    memory.STAT() |= m;
    mode = m;
}

void PPU::step(int cycles) {
    internalCycles += cycles;
    uint8_t& LY = memory.LY();

    switch (mode) {
        case H_BLANK:
            if (internalCycles >= 210) {
                internalCycles -= 210;
                printScreen(LY);

                LY++;

                if (LY == 144) {
                    changeMode(V_BLANK);
                    memory.IF() |= (0x01 << 0);
                } else {
                    changeMode(OAM_SEARCH);
                }
            }
            break;
        case V_BLANK:
            if (internalCycles >= 456) {
                internalCycles -= 456;
                LY++;

                if (LY == 154) {
                    display.renderScreen();
                    LY = 0;
                    changeMode(OAM_SEARCH);
                }
            }
            break;
        case OAM_SEARCH:
            if (internalCycles >= 80) {
                internalCycles -= 80;
                changeMode(PIXEL_TRANSFER);
            }
            break;
        case PIXEL_TRANSFER:
            if (internalCycles >= 166) {
                internalCycles -= 166;

                if (memory.STAT() & (1 << 3))
                    memory.IF() |= (0x01 << 1);

                if ((memory.LYC() == memory.LY()) && (memory.STAT() & (1 << 6)))
                    memory.IF() |= (0x01 << 1);

                memory.STAT() &= ~(1 << 2);
                memory.STAT() |= (memory.LYC() == memory.LY() << 2);

                changeMode(H_BLANK);
            }
            break;
    }
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

void PPU::printScreen(int LY) {
    if (memory.LCDC() & 0x01) {
        printBackground(memory.LY());
        if (memory.LCDC() & 0x20)
            printWindow(memory.LY());
    }
    if (memory.LCDC() & 0x02)
        printSprites(memory.LY());
}

void PPU::printBackground(int LY) {
    uint16_t tileDataOffset = (memory.LCDC() & 0x10) ? 0x8000 : 0x9000;
    uint16_t tileMapOffset = (memory.LCDC() & 0x08) ? 0x9C00 : 0x9800;
    int startingTile = memory.SCX() / 8;
    int offsetX = memory.SCX() % 8;

    for (int tile = startingTile; tile < startingTile + 21; ++tile) {
        uint8_t tileNumber = memory.read8(tileMapOffset+32*((((LY + memory.SCY())/8))%32)+tile%32);
        uint16_t tn = tileNumber;
        if (!(memory.LCDC() & 0x10))
            tn = int8_t(tileNumber);

        uint8_t lsbTile = memory.read8(tileDataOffset+16*tn+2*((LY+memory.SCY())%8));
        uint8_t msbTile = memory.read8(tileDataOffset+16*tn+2*((LY+memory.SCY())%8)+1);
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

void PPU::printWindow(int LY) {
    if (memory.WY() > LY)
        return;

    uint16_t tileDataOffset = (memory.LCDC() & 0x10) ? 0x8000 : 0x9000;
    uint16_t tileMapOffset = (memory.LCDC() & 0x40) ? 0x9C00 : 0x9800;
    int offsetX = memory.WX()-7;

    for (int tile = 0; tile < 20; ++tile) {
        uint8_t tileNumber = memory.read8(tileMapOffset+32*((((LY + memory.WY())/8))%32)+tile%32);
        uint16_t tn = tileNumber;
        if (!(memory.LCDC() & 0x10))
            tn = int8_t(tileNumber);

        uint8_t lsbTile = memory.read8(tileDataOffset+16*tn+2*((LY+memory.WY())%8));
        uint8_t msbTile = memory.read8(tileDataOffset+16*tn+2*((LY+memory.WY())%8)+1);
        for (int pixel = 0; pixel < 8; ++pixel) {
            uint8_t id = ( (msbTile & (1 << (7-pixel))) >> (7-pixel) )*2 + ( (lsbTile & (1 << (7-pixel))) >> (7-pixel) );
            int relativePosition = 8*tile + pixel + offsetX;
            if (relativePosition >= 0 && relativePosition < 160) {
                display.screenBuffer[160 * LY + relativePosition] = colors[(memory.BGP() & (0x03 << (2 * id))) >> (2 * id)];
            }
        }
    }
}

void PPU::printSprites(int LY) {
    for (int i = 0; i < 160; i += 4) {
        uint8_t y = memory.OAM[i] - 9;

        bool doubleSprite = memory.LCDC() & 0x04;
        if (doubleSprite && (y < LY - 8 || y >= LY + 8) || !doubleSprite && (y < LY || y >= LY + 8))
            continue;

        int x = int(memory.OAM[i+1]) - 8;
        uint8_t attributes = memory.OAM[i+3];
        bool priority = attributes & 0x80;
        bool yFlip = attributes & 0x40;
        bool xFlip = attributes & 0x20;
        uint8_t palette = (attributes &  0x10) ? memory.OBP1() : memory.OBP0();
        uint8_t tileId = memory.OAM[i+2] + 1 * yFlip * doubleSprite;

        int address = 0x8000+16*tileId+2*(yFlip*(y-LY) + !yFlip*(7-y+LY));
        uint8_t lsbTile = memory.read8(address);
        uint8_t msbTile = memory.read8(address+1);
        for (int pixel = 0; pixel < 8; ++pixel) {
            uint8_t id = ( (msbTile & (1 << (7-pixel))) >> (7-pixel) )*2 + ( (lsbTile & (1 << (7-pixel))) >> (7-pixel) );
            int relativePosition = x + !xFlip*pixel + xFlip*(7 - pixel);
            if (id != 0 && !(BGW1_3[x + pixel] && priority) && relativePosition >= 0)
                display.screenBuffer[160 * LY + relativePosition] = colors[(palette & (0x03 << (2*id))) >> (2*id)];
        }
    }
}