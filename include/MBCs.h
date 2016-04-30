//
// Created by Javier Luque Sanabria on 7/4/16.
//

#include "MemoryBankController.h"

#ifndef GBEMU_MBC1_H
#define GBEMU_MBC1_H

/**
 * No Memory Bank Controller
 */
class NOMBC : public MemoryBankController {

public:
    NOMBC(BasicMemory *memory) : MemoryBankController(memory) { }

    inline void writeByte(uint16_t address, uint8_t value) {};
    inline uint8_t readByte(uint16_t address){ return 0xFF; };
};

/**
 * Memory Bank Controller 1
 */
class MBC1 : public MemoryBankController {
private:
    uint8_t mbcMode;
public:
    MBC1(BasicMemory *memory) : MemoryBankController(memory) { }

    void writeByte(uint16_t address, uint8_t value);
    uint8_t readByte(uint16_t address);
};

/**
 * Memory Bank Controller 2
 */
class MBC2 : public MemoryBankController {
private:
    uint8_t mbcMode;
public:
    MBC2(BasicMemory *memory) : MemoryBankController(memory) { }

    void writeByte(uint16_t address, uint8_t value);

    uint8_t readByte(uint16_t address);
};

/**
 * Memory Bank Controller 5
 */
class MBC5 : public MemoryBankController {
private:
    uint8_t mbcMode;
public:
    MBC5(BasicMemory *memory) : MemoryBankController(memory) { }

    void writeByte(uint16_t address, uint8_t value);

    uint8_t readByte(uint16_t address);
};
#endif //GBEMU_MBC1_H
