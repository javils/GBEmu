//
// Created by Javier Luque Sanabria on 8/4/16.
//

#ifndef GBEMU_IOHANDLERDMG_H
#define GBEMU_IOHANDLERDMG_H

#include "IOHandler.h"
#include "Timer.h"
#include "MemoryDMG.h"
#include "GPUDMG.h"
#include "Input.h"

class MemoryDMG;
class Timer;
class GPUDMG;
/**
 * This class helps to handle all the IO operations. For example if we write in Timer register DIV
 * we have to reset this register.
 */
class IOHandlerDMG : public IOHandler {
public:
    IOHandlerDMG ();
    void setIOReg(IOREGS regIO, uint8_t value) { IOPorts[regIO - 0xFF00] = value; };
    uint8_t getIOReg(IOREGS regIO) { return IOPorts[regIO - 0xFF00]; };
    void writeIOReg(IOREGS regIO, uint8_t value);
    uint8_t readIOReg(IOREGS regIO);

    inline void setTimer(Timer * timer) { this->timer = timer; }
    inline void setGPU(GPUDMG * gpu) { this->gpuDMG = gpu; }
    inline void setInput(Input * input) { this->input = input; }

    inline GPUDMG *getGPU() { return gpuDMG; }
private:
    array<uint8_t, 0x4D> IOPorts;               //< 0xFF00 to 0xFF4B

    Timer * timer;
    GPUDMG * gpuDMG;
    Input * input;
};
#endif //GBEMU_IOHANDLERDMG_H
