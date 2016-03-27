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
}

void Z80::executeNextOpcode() {
    uint8_t opcode = readByteMem((*refCP())++);
    executeOpcode(opcode);
}

void Z80::executeOpcode(uint8_t opcode) {
    executeInstruction(opcode); // Do that for clean this file.
}