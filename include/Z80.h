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

typedef enum {
    HALT = 0,
    STOP,
    NORMAL
} CPUStatus;

typedef enum {
    NONE_INT = 0x0,         //< No interrupts
    VBLANK_INT = 0x1,       //< VBLANK interrupt (40h)
    LCD_STAT_INT = 0x2,     //< LCD STAT interrupt (48h)
    TIMER_INT = 0x4,        //< TIMER interrupt (50h)
    SERIAL_INT = 0x8,       //< SERIAL interrupt (58h)
    JOYPAD_INT = 0x10       //< JOYPAD interrupt (60h)
} Interrupts;

class Z80 {
private:
    unique_ptr<reg_t> AF;   //< AF register 16bits
    unique_ptr<reg_t> BC;   //< BC register 16bits
    unique_ptr<reg_t> DE;   //< DE register 16bits
    unique_ptr<reg_t> HL;   //< HL register 16bits

    unique_ptr<reg_t> SP;   //< Stack Pointer 16bits
    unique_ptr<reg_t> CP;   //< Counter Program 16bits

    uint32_t cpu_clock_counter; //< Clock counter

    unique_ptr<Bus> bus;  //< Bus for the memory.

    bool InterruptMasterEnable; //< IME
    bool EIExecuted;    //< the next opcode following EI not have interruptions.
    CPUStatus cpuStatus;    //< Status of the cpu. @see CPUStatus

    // Opcode helper functions
    void op_inc_8(uint8_t *reg);
    void op_inc_16(uint16_t *reg);
    void op_dec_8(uint8_t *reg);
    void op_dec_16(uint16_t *reg);

    void op_ld_r8_r8(uint8_t* dst, uint8_t src);
    void op_ld_r8_d8(uint8_t *reg);
    void op_ld_r8_ptr_r16(uint8_t *reg8, uint16_t reg16);

    void op_ld_r16_d16(uint16_t *reg);
    void op_ld_ptr_r16_r8(uint16_t reg16, uint8_t reg8);

    void calc_flags(uint8_t a, uint8_t b, uint8_t carry, bool sub);
    void op_add_hl_r16(uint16_t reg16);
    void op_add_a_r8(uint8_t reg);
    void op_adc_a_r8(uint8_t reg);
    void op_sub_a_r8(uint8_t reg);
    void op_sbc_a_r8(uint8_t reg);

    void op_and_a_r8(uint8_t reg);
    void op_xor_a_r8(uint8_t reg);
    void op_or_a_r8(uint8_t reg);
    void op_cp_a_r8(uint8_t reg);

    void op_ret_cond(bool cond);
    void op_pop_r16(uint16_t* reg);
    void op_push_r16(uint16_t reg);

    void op_jr_cond_nn(bool cond);
    void op_jp_cond_nn(bool cond);

    void op_call_cond_nn(bool cond);
    void op_rst_n(uint8_t address);

    // CB opcode helper functions.
    void op_rlc_r(uint8_t* reg);
    void op_rrc_r(uint8_t* reg);
    void op_rl_r(uint8_t* reg);
    void op_rr_r(uint8_t* reg);
    void op_sla_r(uint8_t* reg);
    void op_sra_r(uint8_t* reg);
    void op_swap_r(uint8_t* reg);
    void op_srl_r(uint8_t* reg);
    void op_bit_n_r(uint8_t n, uint8_t reg);
    void op_bit_n_ptr_HL(uint8_t n);
    void op_res_n_r(uint8_t n, uint8_t *reg);
    void op_res_n_ptr_HL(uint8_t n);
    void op_set_n_r(uint8_t n, uint8_t *reg);
    void op_set_n_ptr_HL(uint8_t n);

    inline void op_unused() { } // TODO: The unused (-) opcodes will lock-up the gameboy CPU when used.

    /**
     * Execute normal opcode.
     * @param opcode opcode to be executed.
     */
    void executeInstruction(uint8_t opcode);    //< Used for clean the Z80 file.

    /**
     * Execute extended opcode (CB Opcodes)
     * @param cbopcode opcode to be executed.
     */
    void executeCBInstruction(uint8_t cbopcode);

    /**
     * Get the next interruption to be executed.
     * @return the interrupt to be executed.
     */
    Interrupts pendingInterruption();

    /**
     * Execute the interrupt procedure.
     * @param interrupt interrupt to be executed.
     */
    void executeInterrupt(Interrupts interrupt);


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
    inline uint32_t getClockCounter() { return cpu_clock_counter; }
    inline CPUStatus getStatus() { return cpuStatus; }
    inline bool areInterruptsEnabled() { return InterruptMasterEnable; }

    // Setters
    inline void setCP(uint16_t value) { CP->val = value; }
    inline void setSP(uint16_t value) { SP->val = value; }

    inline void setAF(uint16_t value) { AF->val = (uint16_t) (value & 0xFFF0); }
    inline void setBC(uint16_t value) { BC->val = value; }
    inline void setDE(uint16_t value) { DE->val = value; }
    inline void setHL(uint16_t value) { HL->val = value; }

    inline void setA(uint8_t value) { AF->bytes.h = value; }

    inline void setF(uint8_t value) { AF->bytes.l = (uint8_t) (value & 0xF0); }
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
    inline void setClockCounter(uint8_t value) { cpu_clock_counter = value; }

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

    /**
     * Execute next opcode in the memory.
     */
    void executeNextOpcode();

    /**
     * Execute a opcode.
     * @param opcode opcode to be executed.
     */
    void executeOpcode(uint8_t opcode); //< Used for debugging and tests.

    /**
     * Resquest an interrupt to be executed next step.
     * @interrupt interrupt to be requested
     */
    void requestInterrupt(Interrupts interrupt);

    /**
     * Set of the CPU.
     */
    void step();

    BasicMemory *getMem() { return bus->getMem(); }

};

#endif //GBEMU_Z80_H
