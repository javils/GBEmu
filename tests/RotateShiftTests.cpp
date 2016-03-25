//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(RotateShiftTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

TEST(RotateShiftTests, RLCA) {
    //RLCA   4 cycles   0 0 0 C
    cpu->writeByteMem(0x0, 0x7);
    cpu->setA(0x12);
    cpu->setFlag(FLAG_Z | FLAG_N | FLAG_C | FLAG_H);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x24, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
}
