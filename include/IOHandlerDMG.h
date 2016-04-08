//
// Created by Javier Luque Sanabria on 8/4/16.
//

#ifndef GBEMU_IOHANDLERDMG_H
#define GBEMU_IOHANDLERDMG_H

#include "IOHandler.h"
#include "Timer.h"
#include "MemoryDMG.h"

class MemoryDMG;

/**
 * This class helps to handle all the IO operations. For example if we write in Timer register DIV
 * we have to reset this register.
 */
class IOHandlerDMG : public IOHandler {
public:
    IOHandlerDMG ();
    void setIOReg(IOREGS regIO, uint8_t value);
    uint8_t getIOReg(IOREGS regIO);
private:
    array<uint8_t, 0x4D> IOPorts;               //< 0xFF00 to 0xFF4B
};
#endif //GBEMU_IOHANDLERDMG_H
