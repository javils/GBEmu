//
// Created by Javier Luque Sanabria on 31/3/16.
//

#ifndef GBEMU_MEMORYDMG_H
#define GBEMU_MEMORYDMG_H

#include <vector>
#include <array>
#include <stdint.h>

#include "Memory.h"
#include "MemoryBankController.h"
#include "Cartridge.h"

using namespace std;
class MemoryDMG : public BasicMemory {
private:
    // This memory is special for DMG
    array<uint8_t, 0x4D> IOPorts;               //< 0xFF00 to 0xFF4B
                                                //< 0xFF4C to 0xFF7F    NOT USABLE MEM
    array<uint8_t, 0x80> HRAM;                  //< 0xFF80 to 0xFFFE
    uint8_t IERegister;                         //< 0xFFFF  Interrupt Enable Register

    unique_ptr<MemoryBankController> mbc;
    vector<uint8_t> ROM;

    Cartridge::CartrigdeType cartrigdeType;

    // Setter and Getter IOREGS
    inline void setIOReg(IOREGS regIO, uint8_t value) { IOPorts[regIO - 0xFF00] = value; }
    inline uint8_t getIOReg(IOREGS regIO) { return IOPorts[regIO - 0xFF00]; }

    void selectMBC(Cartridge::CartrigdeType cartridgeType);
public:
    MemoryDMG(vector<uint8_t> ROM, uint8_t numROMBanks, uint8_t numRAMBanks, Cartridge::CartrigdeType cartrigdeType);

    void init(Cartridge::CartrigdeType cartrigdeType);

    // Getter and setters
    void setByte(uint16_t address, uint8_t value);
    void setWord(uint16_t address, uint16_t value);
    uint8_t getByte(uint16_t address);
    uint16_t getWord(uint16_t address);
};

#endif //GBEMU_MEMORYDMG_H
