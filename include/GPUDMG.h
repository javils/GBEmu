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
    }

    enum COLORS {
        WHITE = 0,
        LIGHT_GREY,
        DARK_GREY,
        BLACK
    };
    void update(uint8_t cycles);
private:
    void renderScanLine();
    COLORS getColor(uint8_t colorNum);
};
#endif //GBEMU_GPUDMG_H
