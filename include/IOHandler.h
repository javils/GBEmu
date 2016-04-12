//
// Created by Javier Luque Sanabria on 8/4/16.
//

#ifndef GBEMU_IOHANDLER_H
#define GBEMU_IOHANDLER_H

#include <stdint.h>
#include "Z80.h"

class IOHandler {
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

    //< Helper methods to get values without rules.
    virtual void setIOReg(IOREGS regIO, uint8_t value) = 0;
    virtual uint8_t getIOReg(IOREGS regIO) = 0;

    //< Get values of the registers with rules. Like must be and GB do it.
    virtual void writeIOReg(IOREGS regIO, uint8_t value) = 0;
    virtual uint8_t readIOReg(IOREGS regIO) = 0;

    inline void setCPU(Z80 * cpu) { this->cpu = cpu; }
    inline Z80* getCPU() { return cpu; }
protected:
    Z80 * cpu;


};
#endif //GBEMU_IOHANDLER_H
