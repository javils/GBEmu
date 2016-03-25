//
// Created by Javier Luque Sanabria on 25/3/16.
//

//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(CPUControlTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

TEST(CPUControlTests, NOP) {
    //NOP   4 cycles
    cpu->writeByteMem(0x0, 0x0);
    cpu->executeNextOpcode();
    LONGS_EQUAL(4, cpu->getClockCounter());
}
