//
// Created by Javier Luque Sanabria on 8/4/16.
//

#ifndef GBEMU_TIMER_H
#define GBEMU_TIMER_H

#include "Z80.h"

class Z80;
class Timer {

public:

    Timer() : divCycles(0), TIMACycles(0) { }

    void update(uint16_t clock);
    inline void resetDIVCycles() { divCycles = 0; };
    inline void resetTIMACycles() { TIMACycles = 0; };
    inline void setCPU(Z80 * cpu) { this->cpu = cpu; };
private:
    void selectFrequency();

    Z80 * cpu;

    uint16_t divCycles;
    uint8_t TIMACycles;
    uint16_t frequency;
};

#endif //GBEMU_TIMER_H
