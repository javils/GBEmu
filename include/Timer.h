//
// Created by Javier Luque Sanabria on 8/4/16.
//

#ifndef GBEMU_TIMER_H
#define GBEMU_TIMER_H

#include "Z80.h"
#include "IOHandlerDMG.h"

class Z80;
class IOHandler;
class Timer {

public:

    Timer() : divCycles(0), TIMACycles(0) { }

    void update(uint16_t clock);
    inline void resetDIVCycles() { divCycles = 0; };
    inline void resetTIMACycles() { TIMACycles = 0; };
    inline void setIOHandler(IOHandler * ioHandler) { this->ioHandler = ioHandler; };
private:
    void selectFrequency();

    IOHandler * ioHandler;

    uint16_t divCycles;
    uint8_t TIMACycles;
    uint16_t frequency;
};

#endif //GBEMU_TIMER_H
