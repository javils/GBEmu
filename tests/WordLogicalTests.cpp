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

TEST(WordLogicalTests, INC_DE) {
    //INC DE   8 cycles
    cpu->writeByteMem(0x0, 0x13);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1, cpu->getDE());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, ADD_HL_DE) {
    //ADD HL,DE   8 cycles   - 0 H C
    cpu->writeByteMem(0x0, 0x19);
    cpu->setHL(0xFFFE);
    cpu->setDE(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getHL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
}

TEST(WordLogicalTests, DEC_DE) {
    //DEC DE   8 cycles
    cpu->writeByteMem(0x0, 0x1b);
    cpu->setDE(0xFFFF);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFFFE, cpu->getDE());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, INC_HL) {
    //INC HL   8 cycles
    cpu->writeByteMem(0x0, 0x23);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1, cpu->getHL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, ADD_HL_HL) {
    //ADD HL,DE   8 cycles   - 0 H C
    cpu->writeByteMem(0x0, 0x29);
    cpu->setHL(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x4, cpu->getHL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
}

TEST(WordLogicalTests, DEC_HL) {
    //DEC HL   8 cycles
    cpu->writeByteMem(0x0, 0x2b);
    cpu->setHL(0xFFFF);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFFFE, cpu->getHL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, INC_SP) {
    //INC SP   8 cycles
    cpu->writeByteMem(0x0, 0x33);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1, cpu->getSP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, INC_PTR_HL) {
    //INC (HL)   12 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x34);
    cpu->setHL(0x1234);
    cpu->writeByteMem(cpu->getHL(), 0x02);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x3, cpu->readByteMem(cpu->getHL()));
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, DEC_PTR_HL) {
    //DEC (HL)   12 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x35);
    cpu->setHL(0x1234);
    cpu->writeByteMem(cpu->getHL(), 0x02);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1, cpu->readByteMem(cpu->getHL()));
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, ADD_HL_SP) {
    //ADD HL,SP   8 cycles   - 0 H C
    cpu->writeByteMem(0x0, 0x39);
    cpu->setHL(0x2);
    cpu->setSP(0x3);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x5, cpu->getHL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
}

TEST(WordLogicalTests, DEC_SP) {
    //DEC SP   8 cycles
    cpu->writeByteMem(0x0, 0x3B);
    cpu->setSP(0xFFFF);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFFFE, cpu->getSP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}

TEST(WordLogicalTests, ADD_SP_R8) {
    //ADD SP,r8   16 cycles   0 0 H C
    cpu->writeByteMem(0x0, 0xE8);
    cpu->writeByteMem(0x1, 0x04);
    cpu->setSP(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x6, cpu->getSP());
    LONGS_EQUAL(16, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
}