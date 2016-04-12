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
                //TODO: RENDER SCANLINE
            }
            break;
        }
        default:
            // IMPOSSIBLE STAY HERE
            break;
    }


}