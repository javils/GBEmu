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
    reg_t AF;
    reg_t BC;
    reg_t DE;
    reg_t HL;

    reg_t SP;
    reg_t CP;
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

    // Useful methods
    inline uint16_t incCP() { return ++CP.val; }

};

#endif //GBEMU_Z80_H
