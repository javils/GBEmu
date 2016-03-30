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

TEST(WordLoadOpcodeTests, LD_DE_d16) {
    //LD DE,d16   12 cycles
    cpu->writeByteMem(0x0, 0x11);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getDE());
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x3, cpu->getCP());
}

TEST(WordLoadOpcodeTests, LD_HL_d16) {
    //LD HL,d16   12 cycles
    cpu->writeByteMem(0x0, 0x21);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getHL());
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x3, cpu->getCP());
}

TEST(WordLoadOpcodeTests, LD_SP_d16) {
    //LD SP,d16   12 cycles
    cpu->writeByteMem(0x0, 0x31);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getSP());
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x3, cpu->getCP());
}

TEST(WordLoadOpcodeTests, POP_BC) {
    //POP BC   12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xc1);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(0x9EEE, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getBC());
    LONGS_EQUAL(12, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, PUSH_BC) {
    //PUSH BC   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xc5);
    cpu->setSP(0x9EEE);
    cpu->setBC(0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, POP_DE) {
    //POP DE   12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD1);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(0x9EEE, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getDE());
    LONGS_EQUAL(12, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, PUSH_DE) {
    //PUSH DE   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD5);
    cpu->setSP(0x9EEE);
    cpu->setDE(0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, POP_HL) {
    //POP HL   12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xE1);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(0x9EEE, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getHL());
    LONGS_EQUAL(12, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, PUSH_HL) {
    //PUSH HL   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xE5);
    cpu->setSP(0x9EEE);
    cpu->setHL(0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, POP_AF) {
    //POP AF   12 cycles   Z N H C
    cpu->writeByteMem(0x0, 0xF1);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(0x9EEE, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getAF());
    LONGS_EQUAL(12, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, PUSH_AF) {
    //PUSH AF   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xF5);
    cpu->setSP(0x9EEE);
    cpu->setAF(0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(WordLoadOpcodeTests, LD_HL_SP_PLUS_d16) {
    //LD HL,SP+r8   12 cycles   0 0 H C
    cpu->writeByteMem(0x0, 0xF8);
    cpu->writeByteMem(0x1, 0x12);
    cpu->setSP(0x4567);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x4579, cpu->getHL());
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0x0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0x0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0x0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0x0, cpu->getFlag(FLAG_C));
}

TEST(WordLoadOpcodeTests, LD_SP_HL) {
    //LD SP,HL   8 cycles   - - - -
    cpu->writeByteMem(0x0, 0xF9);
    cpu->setHL(0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getSP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
}