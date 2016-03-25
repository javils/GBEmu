//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(WordLogicalTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

TEST(WordLogicalTests, INC_BC) {
    //INC BC   8 cycles
    cpu->writeByteMem(0x0, 0x3);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1, cpu->getBC());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, ADD_HL_BC) {
    //ADD HL,BC   8 cycles   - 0 H C
    cpu->writeByteMem(0x0, 0x9);
    cpu->setHL(0xFFFE);
    cpu->setBC(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getHL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
}

TEST(WordLogicalTests, DEC_BC) {
    //DEC BC   8 cycles
    cpu->writeByteMem(0x0, 0xb);
    cpu->setBC(0xFFFF);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFFFE, cpu->getBC());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}