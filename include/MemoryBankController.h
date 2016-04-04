//
// Created by Javier Luque Sanabria on 3/4/16.
//

#ifndef GBEMU_MEMORYBANKCONTROLLER_H
#define GBEMU_MEMORYBANKCONTROLLER_H

class MemoryBankController {
public:
    MemoryBankController(BasicMemory *memory) : memory(memory) {};
    virtual void writeByte(uint16_t address, uint8_t value) = 0;
    virtual void readByte(uint16_t address) = 0;
private:
    BasicMemory *memory;
};

#endif //GBEMU_MEMORYBANKCONTROLLER_H
