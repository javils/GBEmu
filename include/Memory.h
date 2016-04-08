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
    /**
    * IO REgisters.
    */
    enum IOREGS{
        P1 =    0xFF00,         //< Joystick
        SB =    0xFF01,         //< Serial IO data buffer
        SC =    0xFF02,         //< Serial IO control
        DIV =   0xFF04,         //< Divider register
        TIMA =  0xFF05,         //< Timer counter
        TMA =   0xFF06,         //< Timer modulo
        TAC =   0xFF07,         //< Timer control
        IF =    0xFF0F,         //< Interrupt Flag
        NR_10 = 0xFF10,         //< Sound Register - Sweep register
        NR_11 = 0xFF11,         //< Sound Register - Sound length
        NR_12 = 0xFF12,         //< Sound Register - Envelope
        NR_13 = 0xFF13,         //< Sound Register - Frequency lo
        NR_14 = 0xFF14,         //< Sound Register - Frequency hi
        NR_21 = 0xFF16,         //< Sound Register - Sound length
        NR_22 = 0xFF17,         //< Sound Register - Envelope
        NR_23 = 0xFF18,         //< Sound Register - Frequency lo
        NR_24 = 0xFF19,         //< Sound Register - Frequency hi
        NR_30 = 0xFF1A,         //< Sound Register - ON/OFF
        NR_31 = 0xFF1B,         //< Sound Register - Sound length
        NR_32 = 0xFF1C,         //< Sound Register - Output level
        NR_33 = 0xFF1D,         //< Sound Register - Frequency lo
        NR_34 = 0xFF1E,         //< Sound Register - Frequency hi
        NR_41 = 0xFF20,         //< Sound Register - Sound length
        NR_42 = 0xFF21,         //< Sound Register - Envelope
        NR_43 = 0xFF22,         //< Sound Register - Polynomial counter
        NR_44 = 0xFF23,         //< Sound Register - Counter/consecutive; inital
        NR_50 = 0xFF24,         //< Sound Register - Channel control/ON-OFF/Volume
        NR_51 = 0xFF25,         //< Sound Register - Sound output terminal
        NR_52 = 0xFF26,         //< Sound Register - Sound on/off
        WPR_0 = 0xFF30,         //< Sound Register - Waveform storage
        WPR_1 = 0xFF31,         //< Sound Register - Waveform storage
        WPR_2 = 0xFF32,         //< Sound Register - Waveform storage
        WPR_3 = 0xFF33,         //< Sound Register - Waveform storage
        WPR_4 = 0xFF34,         //< Sound Register - Waveform storage
        WPR_5 = 0xFF35,         //< Sound Register - Waveform storage
        WPR_6 = 0xFF36,         //< Sound Register - Waveform storage
        WPR_7 = 0xFF37,         //< Sound Register - Waveform storage
        WPR_8 = 0xFF38,         //< Sound Register - Waveform storage
        WPR_9 = 0xFF39,         //< Sound Register - Waveform storage
        WPR_A = 0xFF3A,         //< Sound Register - Waveform storage
        WPR_B = 0xFF3B,         //< Sound Register - Waveform storage
        WPR_C = 0xFF3C,         //< Sound Register - Waveform storage
        WPR_D = 0xFF3D,         //< Sound Register - Waveform storage
        WPR_E = 0xFF3E,         //< Sound Register - Waveform storage
        WPR_F = 0xFF3E,         //< Sound Register - Waveform storage
        LCDC =  0xFF40,         //< LCD Control
        STAT =  0xFF41,         //< LCDC Status
        SCY =   0xFF42,         //< Scroll Y
        SCX =   0xFF43,         //< Scroll Y
        LY =    0xFF44,         //< LCDC Y-Coordinate
        LYC =   0xFF45,         //< LY Compare
        DMA =   0xFF46,         //< DMA Transfer and start address
        BGP =   0xFF47,         //< BG & Window Palette Data
        OBP0 =  0xFF48,         //< Object Palette 0 Data
        OBP1 =  0xFF49,         //< Object Palette 1 Data
        WY =    0xFF4A,         //< Window Y Position
        WX =    0xFF4B,         //< Window X Position
        IE =    0xFFFF,         //< Interrupt Enable
    };

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
