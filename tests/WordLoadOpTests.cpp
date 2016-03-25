//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(WordLoadOpcodeTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

TEST(WordLoadOpcodeTests, LD_BC_d16) {
    //LD BC,d16   12 cycles
    cpu->writeByteMem(0x0, 0x1);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getBC());
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x3, cpu->getCP());
}

TEST(WordLoadOpcodeTests, LD_NN_SP) {
    //LD (a16),SP   20 cycles
    cpu->writeByteMem(0x0, 0x8);
    cpu->setSP(0x12);
    cpu->writeWordMem(0x1, 0x1234);
    LONGS_EQUAL(0, cpu->readWordMem(0x1234));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getSP());
    LONGS_EQUAL(20, cpu->getClockCounter());
    LONGS_EQUAL(0x12, cpu->readWordMem(0x1234));
    LONGS_EQUAL(0x3, cpu->getCP());
}
