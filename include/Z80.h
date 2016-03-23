//
// Created by Javier Luque Sanabria on 23/3/16.
//

#ifndef GBEMU_Z80_H
#define GBEMU_Z80_H

#include "registers.h"

/**
 * Flags
 */
typedef enum {
    FLAG_Z = 0x80,  // 10000000 Zero
    FLAG_N = 0x40,  // 01000000 Add/Sub (BCD)
    FLAG_H = 0x20,  // 00100000 Half-Carry (BCD)
    FLAG_C = 0x10,  // 00010000 Carry
    FLAG_3 = 0x08,  // Unused
    FLAG_2 = 0x04,  // Unused
    FLAG_1 = 0x02,  // Unused
    FLAG_0 = 0x01,  // Unused
} FLAGS_T;

class Z80 {
private:
    reg_t AF;   // AF register 16bits
    reg_t BC;   // BC register 16bits
    reg_t DE;   // DE register 16bits
    reg_t HL;   // HL register 16bits

    reg_t SP;   // Stack Pointer 16bits
    reg_t CP;   // Counter Program 16bits

    uint16_t cpu_clock_counter; // Clock counter
public:
    // Getters
    inline uint16_t getCP() { return CP.val; }
    inline uint16_t getSP() { return SP.val; }

    inline uint16_t getAF() { return AF.val; }
    inline uint16_t getBC() { return BC.val; }
    inline uint16_t getDE() { return DE.val; }
    inline uint16_t getHL() { return HL.val; }

    inline uint8_t getA() { return AF.bytes.h; }
    inline uint8_t getF() { return AF.bytes.l; }
    inline uint8_t getB() { return BC.bytes.h; }
    inline uint8_t getC() { return BC.bytes.l; }
    inline uint8_t getD() { return DE.bytes.h; }
    inline uint8_t getE() { return DE.bytes.l; }
    inline uint8_t getH() { return HL.bytes.h; }
    inline uint8_t getL() { return HL.bytes.l; }

    inline uint8_t getFlag(FLAGS_T flag) { return (uint8_t) ((getF() & flag) != 0); }
    inline uint16_t getClockCounter() { return cpu_clock_counter; }

    // Setters
    inline void setCP(uint16_t value) { CP.val = value; }
    inline void setSP(uint16_t value) { SP.val = value; }

    inline void setAF(uint16_t value) { AF.val = value; }
    inline void setBC(uint16_t value) { BC.val = value; }
    inline void setDE(uint16_t value) { DE.val = value; }
    inline void setHL(uint16_t value) { HL.val = value; }

    inline void setA(uint8_t value) { AF.bytes.h = value; }
    inline void setF(uint8_t value) { AF.bytes.l = value; }
    inline void setB(uint8_t value) { BC.bytes.h = value; }
    inline void setC(uint8_t value) { BC.bytes.l = value; }
    inline void setD(uint8_t value) { DE.bytes.h = value; }
    inline void setE(uint8_t value) { DE.bytes.l = value; }
    inline void setH(uint8_t value) { HL.bytes.h = value; }
    inline void setL(uint8_t value) { HL.bytes.l = value; }

    inline void setFlag(FLAGS_T flag) { setF(getF() | flag); }
    inline void resetFlag(FLAGS_T flag) { setF(getF() & ~flag); }

    inline void addClockCounter(uint8_t value) { cpu_clock_counter += value; }
    inline uint16_t incCP() { return ++CP.val; }

};

#endif //GBEMU_Z80_H
