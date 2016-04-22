//
// Created by Javier Luque Sanabria on 23/3/16.
//

#include <registers.h>
#include "Z80.h"


Z80::Z80(unique_ptr<BasicMemory> memory) {
    AF.reset(new reg_t());
    BC.reset(new reg_t());
    DE.reset(new reg_t());
    HL.reset(new reg_t());
    CP.reset(new reg_t());
    SP.reset(new reg_t());
    bus.reset(new Bus());   // Init pointer.
    bus->connectToMemory(move(memory)); // Connect bus with memory.
    cpu_clock_counter = 0;
    InterruptMasterEnable = false;
    EIExecuted = false;
    cpuStatus = NORMAL;
}

void Z80::executeNextOpcode() {
    uint8_t opcode = readByteMem((*refCP())++);
    executeOpcode(opcode);
}

void Z80::executeOpcode(uint8_t opcode) {
    executeInstruction(opcode); // Do that for clean this file.
}

void Z80::step(){

    bool isEIafter = EIExecuted;

    Interrupts interrupt = pendingInterruption();
    executeInterrupt(interrupt);
    if (cpuStatus == HALT) {
        addClockCounter(4); //< Maintain memory refresh logic.

        if (interrupt != NONE_INT)
            cpuStatus = NORMAL;
    } else {
        executeNextOpcode();
    }

    // If this condition is true imply that EI and the next instruction was executed
    if (isEIafter && EIExecuted) {
        EIExecuted = false;
        InterruptMasterEnable = true;
    }
}

Interrupts Z80::pendingInterruption() {
    uint8_t ie_reg = readByteMem(0xFFFF);
    uint8_t if_reg = readByteMem(0xFF0F);
    uint8_t result = ie_reg & if_reg;   //< The result have the interrupt that must be processed.

    //< Must be this order, VBLANK have more priority than JOYPAD.
    if (result & VBLANK_INT)
        return VBLANK_INT;
    else if (result & LCD_STAT_INT)
        return LCD_STAT_INT;
    else if (result & TIMER_INT)
        return TIMER_INT;
    else if (result & SERIAL_INT)
        return SERIAL_INT;
    else if (result & JOYPAD_INT)
        return JOYPAD_INT;

    return NONE_INT;
}

void Z80::executeInterrupt(Interrupts interrupt) {
    if (InterruptMasterEnable) {
        //< Interrupt Flag
        uint8_t if_reg = readByteMem(0xFF0F);
        writeByteMem(0xFF0F, if_reg & ~interrupt);   //< Reset the interrupt that will be executed.

        //< If there are interrupt, set IME to false and push CP into stack.
        if (interrupt != NONE_INT) {
            InterruptMasterEnable = false;
            (*refSP())--;
            (*refSP())--;
            writeWordMem(getSP(), getCP());
        }

        //< Change the CP to the interrupt.
        switch (interrupt) {
            case VBLANK_INT:
                setCP(0x40);
                break;
            case LCD_STAT_INT:
                setCP(0x48);
                break;
            case TIMER_INT:
                setCP(0x50);
                break;
            case SERIAL_INT:
                setCP(0x48);
                break;
            case JOYPAD_INT:
                setCP(0x60);
                break;
            default:
                break;
        }
    }
}

void Z80::requestInterrupt(Interrupts interrupt) {
    uint8_t if_reg = readByteMem(0xFF0F);
    writeByteMem(0xFF0F, (if_reg | interrupt));
}