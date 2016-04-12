//
// Created by Javier Luque Sanabria on 12/4/16.
//

#ifndef GBEMU_VIDEO_H
#define GBEMU_VIDEO_H

#include "IOHandler.h"
#include "Z80.h"

class GPU {
public:

    /*
     * All the possible states of the gpu.
     */
    enum GPUModes {
        HBLANK = 0,
        VBLANK,
        SCAN_OAM,
        SCAN_VRAM
    };

    inline void setIOHandler(IOHandler * ioHandler) { this->ioHandler = ioHandler; };
    inline GPUModes getGPUMode() { return gpuMode; }

    inline bool isLCDEnable() { return lcdEnable; };
    inline void enableLCD(bool enable) { lcdEnable = enable; };

    inline uint8_t getLyCounter() { return lyCounter; }

    inline void checkLYWithLYC() {
        uint8_t ly = ioHandler->getIOReg(IOHandler::LY);
        uint8_t lyc = ioHandler->getIOReg(IOHandler::LYC);

        uint8_t stat = ioHandler->getIOReg(IOHandler::STAT);
        if (ly == lyc) {
            stat |= 0x04;

            //< If are equals, and flag is enabled, request interrupt
            if (stat & 0x40)
                ioHandler->getCPU()->requestInterrupt(LCD_STAT_INT);
        }
        else {
            stat &= ~0x04;
        }

        ioHandler->setIOReg(IOHandler::STAT, stat);
    }

protected:
    IOHandler * ioHandler;

    uint8_t lyCounter;
    uint16_t gpuClock;
    GPUModes gpuMode;

    bool lcdEnable;

    virtual void update(uint8_t cycles) = 0;
};
#endif //GBEMU_VIDEO_H
