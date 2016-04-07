//
// Created by Javier Luque Sanabria on 23/3/16.
//

#ifndef GBEMU_MEMORY_H
#define GBEMU_MEMORY_H

#include <stdint.h>
#include <functional>
#include <array>
#include <vector>

#include <random>

/**
 * Abstract class used to implement memories.
 * This class have the basic methods that all memories need to have to work.
 */
using namespace std;

class BasicMemory {
public:
    virtual void setByte(uint16_t address, uint8_t byte) = 0;
    virtual uint8_t getByte(uint16_t address) = 0;
    virtual void setWord(uint16_t address, uint16_t word) = 0;
    virtual uint16_t getWord(uint16_t address) = 0;

    inline void enableRAM(bool enable) { RAMEnabled = enable; }
    inline bool isRAMEnabled() { return RAMEnabled; }

    inline uint8_t getSelectedRAMBank() { return selectedRAMBank; }
    inline void setSelectedRAMBank(uint8_t ramBank) { selectedRAMBank = ramBank; }
    inline uint8_t getSelectedROMBank() { return selectedROMBank; }
    inline void setSelectedROMBank(uint8_t romBank) { selectedROMBank = romBank; }

    inline uint8_t getNumROMBanks() { return numROMBanks; }
    inline uint8_t getNumRAMBanks() { return numRAMBanks; }

    inline void setNumROMBanks(uint8_t banks) { numROMBanks = banks; }
    inline void setNumRAMBanks(uint8_t banks) { numRAMBanks = banks; }

    inline uint8_t getMBCMode() { return mbcMode; }
    inline void setMBCMode(uint8_t mode) { mbcMode = mode; }


    //< Helper function to fill memory with random values.
    auto fillRandom(){
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<uint8_t > dist(
                numeric_limits<uint8_t>::min(),
                numeric_limits<uint8_t>::max());

        return bind(dist,mt);
    }

    // All the gameboys (DMG, SGB, CGB) have this memory map in common.
    array<uint8_t, 0x4000> ROMBase;             //< 0x0000 to 0x3FFF 16KB
    vector<array<uint8_t, 0x4000>> ROMBanks;    //< 0x4000 to 0x7FFF 16KB
    array<uint8_t, 0x2000> VideoRAM;            //< 0x8000 to 0x9FFF 8KB
    vector<array<uint8_t, 0x2000>> RAMBanks;    //< 0xA000 to 0xBFFF 8KB
    array<uint8_t, 0x2000> WorkRAM;             //< 0xC000 to 0xDFFF 8KB
    array<uint8_t, 0x1E00> EchoRam;             //< 0xE000 to 0xFDFF    (ECHO RAM of 0xC000 - 0xDDFF)
    array<uint8_t, 0xA0> OAMRam;                //< 0xFE00 to 0xFE9F

private:
    bool RAMEnabled;

    uint8_t numROMBanks;
    uint8_t numRAMBanks;

    uint8_t selectedROMBank;    //< Current selected ROM bank.
    uint8_t selectedRAMBank;    //< Current selected RAM bank.

    uint8_t mbcMode;            //< MBC Mode 0 => ROM, 1 => RAM

};

/**
 * Class in progress. At the moment only is used for tests.
 */
class Memory_DMG : public BasicMemory {
private:
    uint8_t mem[0x10000];
public:
    void setByte(uint16_t address, uint8_t value) {
        mem[address] = value;
    }

    void setWord(uint16_t address, uint16_t value) {
        setByte(address++, value & 0xFF);
        setByte(address, value >> 8);
    }

    uint8_t getByte(uint16_t address) {
        return mem[address];
    }

    uint16_t getWord(uint16_t address) {
        uint8_t h, l;
        h = getByte(address + 1);
        l = getByte(address);
        return h << 8 | l;
    }
};
#endif //GBEMU_MEMORY_H
