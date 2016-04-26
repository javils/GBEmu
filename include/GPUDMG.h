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
    }

    enum COLORS {
        WHITE = 0,
        LIGHT_GREY,
        DARK_GREY,
        BLACK
    };
    void update(uint8_t cycles);

    void DisableLCD();
    void renderScanLine();

    void renderBGAndWindow();

    void renderSprites();
private:
    COLORS getColor(uint8_t colorNum, IOHandler::IOREGS paletteReg);
};
#endif //GBEMU_GPUDMG_H
