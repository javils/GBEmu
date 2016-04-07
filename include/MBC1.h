//
// Created by Javier Luque Sanabria on 7/4/16.
//

#include "MemoryBankController.h"

#ifndef GBEMU_MBC1_H
#define GBEMU_MBC1_H

class MBC1 : public MemoryBankController {

public:
    MBC1(BasicMemory *memory) : MemoryBankController(memory) { }

    void writeByte(uint16_t address, uint8_t value);
    uint8_t readByte(uint16_t address);
};
#endif //GBEMU_MBC1_H
