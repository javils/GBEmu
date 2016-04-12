//
// Created by Javier Luque Sanabria on 12/4/16.
//

#ifndef GBEMU_GPUDMG_H
#define GBEMU_GPUDMG_H

#include "GPU.h"

class GPUDMG : public GPU {
public:
    void update(uint8_t cycles);
};
#endif //GBEMU_GPUDMG_H
