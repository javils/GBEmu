//
// Created by Javier Luque Sanabria on 27/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(JumpTests){
        unique_ptr<Z80> cpu;
        unique_ptr<BasicMemory> mem;
        void setup() {
            mem.reset(new Memory_DMG());
            cpu.reset(new Z80(move(mem)));
        }

        void teardown() {
        }
};

TEST(JumpTests, JR_dd){
    cpu->setCP(0x5000);
    cpu->writeByteMem(0x5000, 0x18);
    cpu->writeByteMem(0x5001, 0x40);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x5042, cpu->getCP());    // Initial CP + 1 readByte + 1 next opcode.
    LONGS_EQUAL(12, cpu->getClockCounter());

    cpu->writeByteMem(cpu->getCP(), 0x18);
    cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), (uint8_t) -4);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x5040, cpu->getCP());
    LONGS_EQUAL(24, cpu->getClockCounter());
}

TEST(JumpTests, JR_NZ_dd){
    //JR NZ,r8   12/8 cycles   - - - -
    cpu->setCP(0x5000);
    cpu->writeByteMem(0x5000, 0x20);
    cpu->writeByteMem(0x5001, 0x40);
    cpu->setFlag(FLAG_Z);
    cpu->executeNextOpcode();
    if (cpu->getFlag(FLAG_Z) == 0) {
        LONGS_EQUAL(0x5042, cpu->getCP());    // Initial CP + 1 readByte + 1 next opcode.
        LONGS_EQUAL(12, cpu->getClockCounter());
    }
    else {
        LONGS_EQUAL(0x5002, cpu->getCP());    // 1 readByte + 1 next opcode.
        LONGS_EQUAL(8, cpu->getClockCounter());
    }
}

TEST(JumpTests, JR_Z_dd){
    //JR Z,r8   12/8 cycles   - - - -
    cpu->setCP(0x5000);
    cpu->writeByteMem(0x5000, 0x28);
    cpu->writeByteMem(0x5001, 0x40);
    cpu->setFlag(FLAG_Z);
    cpu->executeNextOpcode();
    if (cpu->getFlag(FLAG_Z)) {
        LONGS_EQUAL(0x5042, cpu->getCP());    // Initial CP + 1 readByte + 1 next opcode.
        LONGS_EQUAL(12, cpu->getClockCounter());
    }
    else {
        LONGS_EQUAL(0x5002, cpu->getCP());    // 1 readByte + 1 next opcode.
        LONGS_EQUAL(8, cpu->getClockCounter());
    }
}

TEST(JumpTests, JR_NC_dd){
    //JR NC,r8   12/8 cycles   - - - -
    cpu->setCP(0x5000);
    cpu->writeByteMem(0x5000, 0x30);
    cpu->writeByteMem(0x5001, 0x40);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    if (cpu->getFlag(FLAG_C) == 0) {
        LONGS_EQUAL(0x5042, cpu->getCP());    // Initial CP + 1 readByte + 1 next opcode.
        LONGS_EQUAL(12, cpu->getClockCounter());
    }
    else {
        LONGS_EQUAL(0x5002, cpu->getCP());    // 1 readByte + 1 next opcode.
        LONGS_EQUAL(8, cpu->getClockCounter());
    }
}

TEST(JumpTests, JR_C_dd){
    //JR C,r8   12/8 cycles   - - - -
    cpu->setCP(0x5000);
    cpu->writeByteMem(0x5000, 0x38);
    cpu->writeByteMem(0x5001, 0x40);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    if (cpu->getFlag(FLAG_C)) {
        LONGS_EQUAL(0x5042, cpu->getCP());    // Initial CP + 1 readByte + 1 next opcode.
        LONGS_EQUAL(12, cpu->getClockCounter());
    }
    else {
        LONGS_EQUAL(0x5002, cpu->getCP());    // 1 readByte + 1 next opcode.
        LONGS_EQUAL(8, cpu->getClockCounter());
    }
}