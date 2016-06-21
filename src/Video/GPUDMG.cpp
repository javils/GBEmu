//
// Created by Javier Luque Sanabria on 12/4/16.
//

#include <GPUDMG.h>

bool GPUDMG::update(uint8_t cycles) {

    if (!isLCDEnable()) {
        lyCounter = 0;
        gpuClock = 0;
        ioHandler->setIOReg(IOHandler::LY, 0x0);
        gpuMode = GPUModes::SCAN_OAM;
        uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);
        stat &= 0xFC;
        stat |= gpuMode;
        ioHandler->setIOReg(IOHandler::STAT, stat);

        if (IsSetBit(stat, 5))
            ioHandler->getCPU()->requestInterrupt(LCD_STAT_INT);

        //< If we have the LCD disabled, not update ly counter with clocks
        return false;
    }

    //< Update GPU clock.
    gpuClock += cycles;

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

                    if (IsSetBit(stat, 4))
                        ioHandler->getCPU()->requestInterrupt(LCD_STAT_INT);

                    //< Now we are in VBLANK, request the correspond interrupt.
                    ioHandler->getCPU()->requestInterrupt(VBLANK_INT);
                    windowLine = 0;
                    return true;
                }
                else {
                    gpuMode = SCAN_OAM;
                    stat &= 0xFC;
                    stat |= gpuMode;

                    if (IsSetBit(stat, 5))
                        ioHandler->getCPU()->requestInterrupt(LCD_STAT_INT);
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

                uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);

                if (lyCounter > LCD::SCREEN_HEIGHT_VBLANK)
                {
                    //< Restart the process
                    gpuClock = 0;
                    lyCounter = 0;
                    gpuMode = SCAN_OAM;
                    stat &= 0xFC;
                    stat |= gpuMode;

                    if (IsSetBit(stat, 5))
                        ioHandler->getCPU()->requestInterrupt(LCD_STAT_INT);
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

                if (IsSetBit(stat, 3))
                    ioHandler->getCPU()->requestInterrupt(LCD_STAT_INT);

                renderScanLine();
            }
            break;
        }
        default:
            // IMPOSSIBLE STAY HERE
            break;
    }
    return false;
}

void GPUDMG::renderScanLine() {
    if (isLCDEnable()) {
        renderBG();
        renderWindow();
        renderSprites();
    }
}

void GPUDMG::renderBG() {
    uint8_t lcdc = ioHandler->getIOReg(IOHandler::LCDC);

    if (!IsSetBit(lcdc, 0))
        return;

    uint8_t scx = ioHandler->getIOReg(IOHandler::SCX);
    uint8_t scy = ioHandler->getIOReg(IOHandler::SCY);

    uint16_t tilesMap = (uint16_t) (IsSetBit(lcdc, 3) ? 0x9C00 : 0x9800);
    uint16_t tilesData = (uint16_t) (IsSetBit(lcdc, 4) ? 0x8000 : 0x8800);

    uint16_t tilesMapAddres;
    uint16_t tilesDataAddres;

    for (uint8_t x = 0; x < LCD::SCREEN_WIDTH; x++) {
        tilesDataAddres = tilesData;
        tilesMapAddres = tilesMap;
        uint8_t realX = scx + x;
        uint8_t realY = scy + lyCounter;
        uint8_t tileX = realX >> 3; //< x/8
        uint16_t tileY = (realY >> 3) << 5;
        tilesMapAddres += tileX + tileY;

        uint8_t tileId = ioHandler->getCPU()->readByteMem(tilesMapAddres);

        if (tilesData == 0x8800) {
            if (IsSetBit(tileId, 7))
                tileId &= 0x7F;
            else
                tileId += 128;
        }

        tilesDataAddres += (tileId << 4) + ((realY % 8) << 1);

        uint8_t byte1 = ioHandler->getCPU()->readByteMem(tilesDataAddres);
        tilesDataAddres++;
        uint8_t byte2 = ioHandler->getCPU()->readByteMem(tilesDataAddres);

        uint8_t colorBit = (uint8_t) (0x1 << (7 - (realX % 8)));

        uint8_t color = (uint8_t) ((byte2 & colorBit) ? 1 : 0);
        color <<= 1;
        color |= (uint8_t) ((byte1 & colorBit) ? 1 : 0);

        uint8_t xPos = x;

        if (xPos < 0 || xPos >= LCD::SCREEN_WIDTH)
            continue;

        lcd->setPixelColor(xPos, lyCounter, getColor(color, IOHandler::BGP));
        lcd->setColorCache(xPos, lyCounter, (uint8_t) color);

    }
}

void GPUDMG::renderWindow() {

    if (windowLine > 143)
        return;

    uint8_t lcdc = ioHandler->getIOReg(IOHandler::LCDC);

    if (!IsSetBit(lcdc, 0) || !IsSetBit(lcdc, 5))
        return;

    int16_t wx = (int16_t) (ioHandler->getIOReg(IOHandler::WX) - 7);

    if (wx > 159)
        return;

    uint8_t wy = ioHandler->getIOReg(IOHandler::WY);

    if (wy > 143 || wy > lyCounter)
        return;

    uint16_t tilesMap = (uint16_t) (IsSetBit(lcdc, 6) ? 0x9C00 : 0x9800);
    uint16_t tilesData = (uint16_t) (IsSetBit(lcdc, 4) ? 0x8000 : 0x8800);

    uint16_t tilesMapAddres;
    uint16_t tilesDataAddres;

    for (uint8_t x = 0; x < LCD::SCREEN_WIDTH; x++) {
        tilesDataAddres = tilesData;
        tilesMapAddres = tilesMap;
        int16_t realX = x;
        int16_t realY = windowLine;
        int16_t tileX = realX >> 3; //< x/8
        int16_t tileY = (realY >> 3) << 5;
        tilesMapAddres += tileX + tileY;

        uint8_t tileId = ioHandler->getCPU()->readByteMem(tilesMapAddres);

        if (tilesData == 0x8800) {
            if (IsSetBit(tileId, 7))
                tileId &= 0x7F;
            else
                tileId += 128;
        }

        tilesDataAddres += (tileId << 4) + ((realY % 8) << 1);

        uint8_t byte1 = ioHandler->getCPU()->readByteMem(tilesDataAddres);
        tilesDataAddres++;
        uint8_t byte2 = ioHandler->getCPU()->readByteMem(tilesDataAddres);

        uint8_t colorBit = (uint8_t) (0x1 << (7 - (realX % 8)));

        uint8_t color = (uint8_t) ((byte2 & colorBit) ? 1 : 0);
        color <<= 1;
        color |= (uint8_t) ((byte1 & colorBit) ? 1 : 0);

        uint8_t xPos = (uint8_t) (x + wx);

        if (xPos < 0 || xPos >= LCD::SCREEN_WIDTH)
            continue;

        lcd->setPixelColor(xPos, lyCounter, getColor(color, IOHandler::BGP));
        lcd->setColorCache(xPos, lyCounter, (uint8_t) color);
    }
    windowLine++;
}

void GPUDMG::renderSprites() {
    uint8_t lcdc = ioHandler->getIOReg(IOHandler::LCDC);

    if (IsSetBit(lcdc, 1)) {

        uint8_t height = (uint8_t) (IsSetBit(lcdc, 2) ? 16 : 8);

        for (uint8_t sprite = 0; sprite < 40; sprite++) {
            uint16_t spriteDataAddr = (uint16_t) (0xFE00 + (sprite << 2));
            int16_t spriteY = (int16_t) (ioHandler->getCPU()->readByteMem(spriteDataAddr) - 16);

            if (spriteY > lyCounter || (spriteY + height) <= lyCounter)
                continue;

            spriteDataAddr++;
            int16_t spriteX = (int16_t) (ioHandler->getCPU()->readByteMem(spriteDataAddr) - 8);
            spriteDataAddr++;
            uint8_t tileLocation = (uint8_t) (ioHandler->getCPU()->readByteMem(spriteDataAddr) &
                                              ((height == 16) ? 0xFE : 0xFF));
            spriteDataAddr++;
            uint8_t attributes = ioHandler->getCPU()->readByteMem(spriteDataAddr);
            bool behindBG = IsSetBit(attributes, 7);
            bool yFlip = IsSetBit(attributes, 6);
            bool xFlip = IsSetBit(attributes, 5);
            IOHandler::IOREGS palette = IsSetBit(attributes, 4) ? IOHandler::OBP1 : IOHandler::OBP0;

            int8_t lineSprite = lyCounter - (int8_t) spriteY;
            if (yFlip) {
                lineSprite -= height - 1;
                lineSprite *= -1;
            }

            uint16_t tileDataAddress = (uint16_t) (0x8000 + (tileLocation << 4) + (lineSprite << 1));
            uint8_t byte1 = ioHandler->getCPU()->readByteMem(tileDataAddress);
            tileDataAddress++;
            uint8_t byte2 = ioHandler->getCPU()->readByteMem(tileDataAddress);

            for (uint8_t tileX = 0; tileX < 8; tileX++) {
                uint8_t colorBit = tileX;

                if (xFlip) {
                    colorBit -= height - 1;
                    colorBit *= -1;
                }

                colorBit = (uint8_t) (0x1 << ((~colorBit) & 0x07));

                uint8_t color = (uint8_t) ((byte2 & colorBit) ? 1 : 0);
                color <<= 1;
                color |= (uint8_t) ((byte1 & colorBit) ? 1 : 0);

                //< Color 0 is transparent.
                if (color == 0x0)
                    continue;

                uint8_t col = getColor(color, palette);

                int16_t xPosition = spriteX + tileX;

                if (xPosition < 0 || xPosition >= LCD::SCREEN_WIDTH)
                    continue;

                uint8_t color_cache = lcd->getColorCache()[lyCounter][xPosition];
                //< If behindBG is active and the background/windows color != 0 (WHITE) then not show sprite.
                if (behindBG && (color_cache != 0))
                    continue;

                lcd->setPixelColor((uint8_t) xPosition, lyCounter, col);

                lcd->setColorCache((uint8_t) xPosition, lyCounter, SetBit(color_cache, 3));
            }
        }
    }
}

uint8_t GPUDMG::getColor(uint8_t colorNum, IOHandler::IOREGS paletteReg) {
    uint8_t palette = ioHandler->getIOReg(paletteReg);
    uint8_t significantBits = 0;

    //< 0 => 0, 1 bits, 1 => 2, 3 bits, 2 => 4, 5 bits, 3 => 6, 7 bits.
    switch (colorNum) {
        case 0:
            significantBits = 0x03;
            break;
        case 1:
            significantBits = 0x0C;
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

    return color;
}

void GPUDMG::DisableLCD() {
    enableLCD(false);
    setGPUMode(GPUDMG::GPUModes::SCAN_OAM);
    setGPUClock(0);
    lyCounter = 0;
}