//
// Created by Javier Luque Sanabria on 3/4/16.
//

#ifndef GBEMU_MEMORYBANKCONTROLLER_H
#define GBEMU_MEMORYBANKCONTROLLER_H

#include "Memory.h"

class BasicMemory;
class MemoryBankController {
public:
    MemoryBankController(BasicMemory *memory) : memory(memory) {};
    virtual void writeByte(uint16_t address, uint8_t value) = 0;
    virtual uint8_t readByte(uint16_t address) = 0;
protected:
    BasicMemory *memory;
};

#endif //GBEMU_MEMORYBANKCONTROLLER_H
