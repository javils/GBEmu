//
// Created by Javier Luque Sanabria on 23/3/16.
//

#include "Z80.h"


Z80::Z80(unique_ptr<BasicMemory> memory) {
    bus.reset(new Bus());   // Init pointer.
    bus->connectToMemory(move(memory)); // Connect bus with memory.
}