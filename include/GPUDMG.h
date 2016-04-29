//
// Created by Javier Luque Sanabria on 12/4/16.
//

#ifndef GBEMU_GPUDMG_H
#define GBEMU_GPUDMG_H

#include "GPU.h"
#include "Utils.h"

class GPUDMG : public GPU {
public:
    GPUDMG () {
        lcd = new LCD();
        lyCounter = 0;
        windowLine = 0;
    }

    bool update(uint8_t cycles);

    void DisableLCD();
    void renderScanLine();

    void renderBG();

    void renderWindow();
    void renderSprites();
private:
    uint8_t getColor(uint8_t colorNum, IOHandler::IOREGS paletteReg);
};
#endif //GBEMU_GPUDMG_H
