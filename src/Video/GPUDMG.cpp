//
// Created by Javier Luque Sanabria on 12/4/16.
//

#include <GPUDMG.h>

void GPUDMG::update(uint8_t cycles) {

    if (!isLCDEnable()) {
        lyCounter = 0;
        ioHandler->setIOReg(IOHandler::LY, 0x0);
        gpuMode = GPUModes::SCAN_OAM;
        uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);
        stat &= 0xFC;
        stat |= gpuMode;
        ioHandler->setIOReg(IOHandler::STAT, stat);
        //< If we have the LCD disabled, not update ly counter with clocks
        return;
    }

    //< Update GPU clock.
    gpuClock += cycles;

    //TODO: Probably when change the lower values from stat, need check the interrupt flag and request the interrupt.
    switch(gpuMode) {
        case HBLANK: {
            if (gpuClock >= 204) {
                gpuClock -= 204;
                lyCounter++;
                ioHandler->setIOReg(IOHandler::LY, lyCounter);
                checkLYWithLYC();

                uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);

                if (lyCounter == 144)   //< End of the visible screen
                {
                    gpuMode = VBLANK;
                    stat &= 0xFC;
                    stat |= gpuMode;
                    //< Now we are in VBLANK, request the correspond interrupt.
                    ioHandler->getCPU()->requestInterrupt(VBLANK_INT);
                }
                else {
                    gpuMode = SCAN_OAM;
                }

                //< Update the stat register
                ioHandler->setIOReg(IOHandler::STAT, stat);
            }
            break;
        }
        case VBLANK: {
            if (gpuClock >= 456)
            {
                gpuClock -= 456;
                lyCounter++;
                ioHandler->setIOReg(IOHandler::LY, lyCounter);
                checkLYWithLYC();

                if (lyCounter > 153)
                {
                    //< Restart the process
                    gpuClock = 0;
                    lyCounter = 0;
                    gpuMode = SCAN_OAM;
                }
            }
            break;
        }
        case SCAN_OAM: {
            if (gpuClock >= 82) {
                gpuClock -= 82;
                gpuMode = SCAN_VRAM;
                uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);
                stat &= 0xFC;
                stat |= gpuMode;
                ioHandler->setIOReg(IOHandler::STAT, stat);
            }
            break;
        }
        case SCAN_VRAM: {
            if (gpuClock >= 172) {
                gpuClock -= 172;
                gpuMode = HBLANK;
                uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);
                stat &= 0xFC;
                stat |= gpuMode;
                ioHandler->setIOReg(IOHandler::STAT, stat);
                renderScanLine();
            }
            break;
        }
        default:
            // IMPOSSIBLE STAY HERE
            break;
    }
}

void GPUDMG::renderScanLine() {
    uint8_t lcdc = ioHandler->getIOReg(IOHandler::LCDC);

    uint8_t scx = ioHandler->getIOReg(IOHandler::SCX);
    uint8_t scy = ioHandler->getIOReg(IOHandler::SCY);

    if (IsSetBit(lcdc, 0)) {
        uint16_t bgTileMapBase = (uint16_t) (IsSetBit(lcdc, 3) ? 0x9800 : 0x9C00);  //< Each byte is a tile id.
        uint16_t bgTileDataBase = (uint16_t) (IsSetBit(lcdc, 4) ? 0x8000 : 0x8800); //< Each tile here is 16 bits.

        uint8_t y = lyCounter + scy;
        uint8_t tileId;

        uint8_t yAdjusted, xAdjusted;
        uint16_t bgTileMapFinalAddr;
        uint16_t bgTileDataFinalAddr;
        uint8_t byte1, byte2;
        uint8_t bitColour;
        uint8_t colorNum;

        for (uint8_t x = 0; x < LCD::SCREEN_WIDTH; x++)
        {
            //< Get the tile ID that correspond to the actual x, y position.
            yAdjusted = ((y >> 3) << 6);
            xAdjusted = (x >> 3);
            bgTileMapFinalAddr = (bgTileMapBase + yAdjusted + xAdjusted);
            tileId = ioHandler->getCPU()->readByteMem(bgTileMapFinalAddr);

            //< If data is picked in 0x8800, the tile id is -127 to 127, in other case 0 to 255
            if (bgTileDataBase == 0x8000) {
                bgTileDataFinalAddr = bgTileDataBase + (tileId << 4);
            }
            else {
                //< Signed tileId
                bgTileDataFinalAddr = (uint16_t) (bgTileDataBase + (((int8_t)(tileId) + 128) << 4));
            }

            //< Need Sum the line of the tile and multiply by 2 because a tile use 2 bytes.
            bgTileDataFinalAddr += (y % 8) << 1;

            //< Get first the most significant bit, that is pixel0.
            bitColour = (uint8_t) (0x80 >> (x % 8));
            byte1 = ioHandler->getCPU()->readByteMem(bgTileDataFinalAddr);
            byte2 = ioHandler->getCPU()->readByteMem((uint16_t) (bgTileDataFinalAddr + 1));

            colorNum = getBitVal(byte1, bitColour);
            colorNum <<= 1;    //< Shift one to left to concatenate the other bit.
            colorNum |= getBitVal(byte2, bitColour);

            COLORS color = getColor(colorNum);

            LCDColor lcdColor;

            switch (color) {
                case WHITE:
                    lcdColor.setColor(0xFF, 0xFF, 0xFF);
                    break;
                case LIGHT_GREY:
                    lcdColor.setColor(0xCC, 0xCC, 0xCC);
                    break;
                case DARK_GREY:
                    lcdColor.setColor(0x77, 0x77, 0x77);
                    break;
                default:
                    lcdColor.setColor(0x00, 0x00, 0x00);
                    break;
            }

            lcd->setPixelColor(lyCounter, x, lcdColor);
        }
    }
}

GPUDMG::COLORS GPUDMG::getColor(uint8_t colorNum) {
    COLORS result = WHITE;
    uint8_t palette = ioHandler->getIOReg(IOHandler::BGP);
    uint8_t significantBits = 0;

    //< 0 => 0, 1 bits, 1 => 2, 3 bits, 2 => 4, 5 bits, 3 => 6, 7 bits.
    switch (colorNum) {
        case 0:
            significantBits = 0x02;
            break;
        case 1:
            significantBits = 0x12;
            break;
        case 2:
            significantBits = 0x30;
            break;
        case 3:
            significantBits = 0xC0;
            break;
        default:
            break;
    }

    uint8_t color = (palette & significantBits) >> (colorNum << 1);

    return (COLORS) color;
}