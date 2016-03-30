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

TEST(CPUControlTests, STOP) {
    //STOP 0   4 cycles   - - - -
    cpu->writeByteMem(0x0, 0x10);
    cpu->executeNextOpcode();
    //TODO : NOT IMPLEMENTED YET
    FAIL("NOT IMPLEMENTED YET");
}

TEST(CPUControlTests, SCF) {
    //SCF   4 cycles   - 0 0 1
    cpu->writeByteMem(0x0, 0x37);
    cpu->executeNextOpcode();
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
}

TEST(CPUControlTests, CCF) {
    //CCF   4 cycles   - 0 0 C
    cpu->writeByteMem(0x0, 0x3F);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());

    cpu->writeByteMem(0x1, 0x3F);
    cpu->resetFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(CPUControlTests, DI) {
    //DI   4 cycles   - - - -
    cpu->writeByteMem(0x0, 0xF3);
    cpu->executeNextOpcode();
    //TODO : NOT IMPLEMENTED YET
    FAIL("INTERRUPTS NOT IMPLEMENTED YET");
}

TEST(CPUControlTests, EI) {
    //EI   4 cycles   - - - -
    cpu->writeByteMem(0x0, 0xFB);
    cpu->executeNextOpcode();
    //TODO : NOT IMPLEMENTED YET
    FAIL("INTERRUPTS NOT IMPLEMENTED YET");
}