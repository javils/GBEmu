//
// Created by Javier Luque Sanabria on 23/3/16.
//

#ifndef GBEMU_Z80_H
#define GBEMU_Z80_H

#include "registers.h"
#include "Bus.h"


using namespace std;

/**
 * Flags
 */
typedef enum {
    FLAG_Z = 0x80,  //< 10000000 Zero
    FLAG_N = 0x40,  //< 01000000 Add/Sub (BCD)
    FLAG_H = 0x20,  //< 00100000 Half-Carry (BCD)
    FLAG_C = 0x10,  //< 00010000 Carry
    FLAG_3 = 0x08,  //< Unused
    FLAG_2 = 0x04,  //< Unused
    FLAG_1 = 0x02,  //< Unused
    FLAG_0 = 0x01,  //< Unused
} FLAGS_T;

class Z80 {
private:
    unique_ptr<reg_t> AF;   //< AF register 16bits
    unique_ptr<reg_t> BC;   //< BC register 16bits
    unique_ptr<reg_t> DE;   //< DE register 16bits
    unique_ptr<reg_t> HL;   //< HL register 16bits

    unique_ptr<reg_t> SP;   //< Stack Pointer 16bits
    unique_ptr<reg_t> CP;   //< Counter Program 16bits

    uint16_t cpu_clock_counter; //< Clock counter

    unique_ptr<Bus> bus;  //< Bus for the memory.

    // Opcode functions
    void op_inc_8(uint8_t *reg);
    void op_dec_8(uint8_t *reg);
    void op_ld_r8_d8(uint8_t *reg);

    // Helper functions to get the pointer. Only used internally.
    inline uint16_t *refCP() { return &(CP->val); }
    inline uint16_t *refSP() { return &(SP->val); }

    inline uint16_t *refAF() { return &(AF->val); }
    inline uint16_t *refBC() { return &(BC->val); }
    inline uint16_t *refDE() { return &(DE->val); }
    inline uint16_t *refHL() { return &(HL->val); }

    inline uint8_t *refA() { return &(AF->bytes.h); }
    inline uint8_t *refF() { return &(AF->bytes.l); }
    inline uint8_t *refB() { return &(BC->bytes.h); }
    inline uint8_t *refC() { return &(BC->bytes.l); }
    inline uint8_t *refD() { return &(DE->bytes.h); }
    inline uint8_t *refE() { return &(DE->bytes.l); }
    inline uint8_t *refH() { return &(HL->bytes.h); }
    inline uint8_t *refL() { return &(HL->bytes.l); }
public:
    /**
     * Constructor
     * @param memory pointer to the memory that is used by the micro.
     */
    Z80(unique_ptr<BasicMemory> memory);

    // Getters
    inline uint16_t getCP() { return CP->val; }
    inline uint16_t getSP() { return SP->val; }

    inline uint16_t getAF() { return AF->val; }
    inline uint16_t getBC() { return BC->val; }
    inline uint16_t getDE() { return DE->val; }
    inline uint16_t getHL() { return HL->val; }

    inline uint8_t getA() { return AF->bytes.h; }
    inline uint8_t getF() { return AF->bytes.l; }
    inline uint8_t getB() { return BC->bytes.h; }
    inline uint8_t getC() { return BC->bytes.l; }
    inline uint8_t getD() { return DE->bytes.h; }
    inline uint8_t getE() { return DE->bytes.l; }
    inline uint8_t getH() { return HL->bytes.h; }
    inline uint8_t getL() { return HL->bytes.l; }

    inline uint8_t getFlag(FLAGS_T flag) { return (uint8_t) ((getF() & flag) != 0); }
    inline uint16_t getClockCounter() { return cpu_clock_counter; }

    // Setters
    inline void setCP(uint16_t value) { CP->val = value; }
    inline void setSP(uint16_t value) { SP->val = value; }

    inline void setAF(uint16_t value) { AF->val = value; }
    inline void setBC(uint16_t value) { BC->val = value; }
    inline void setDE(uint16_t value) { DE->val = value; }
    inline void setHL(uint16_t value) { HL->val = value; }

    inline void setA(uint8_t value) { AF->bytes.h = value; }
    inline void setF(uint8_t value) { AF->bytes.l = value; }
    inline void setB(uint8_t value) { BC->bytes.h = value; }
    inline void setC(uint8_t value) { BC->bytes.l = value; }
    inline void setD(uint8_t value) { DE->bytes.h = value; }
    inline void setE(uint8_t value) { DE->bytes.l = value; }
    inline void setH(uint8_t value) { HL->bytes.h = value; }
    inline void setL(uint8_t value) { HL->bytes.l = value; }

    inline void setFlag(uint8_t flag) { setF(getF() | flag); }
    inline void resetFlag(uint8_t flag) { setF(getF() & ~flag); }
    inline void setFlagCond(uint8_t flag, bool cond) { if (cond) setFlag(flag); else resetFlag(flag); }

    inline void addClockCounter(uint8_t value) { cpu_clock_counter += value; }

    inline uint16_t incCP() { return ++CP->val; }

    /**
     * Write a byte in the bus.
     * @param address address where want to write.
     * @param value value to write.
     * @see Bus
     */
    inline void writeByteMem(uint16_t address, uint8_t value) { bus->sendByte(address, value); }

    /**
     * Write a word in the bus.
     * @param address where want to write.
     * @param value value to write.
     * @see Bus
     */
    inline void writeWordMem(uint16_t address, uint16_t value) { bus->sendWord(address, value); }

    /**
     * Read byte from the bus.
     * @param address address where want to read.
     * @return byte read from address.
     * @see Bus
     */
    inline uint8_t readByteMem(uint16_t address) { return bus->receiveByte(address); }

    /**
     * Read word from the bus.
     * @param address address where want to read.
     * @return word read from the address.
     * @see Bus
     */
    inline uint16_t readWordMem(uint16_t address) { return bus->receiveWord(address); }

    void executeNextOpcode();

    void executeOpcode(uint8_t opcode); //< Used for debugging and tests.

};

#endif //GBEMU_Z80_H
