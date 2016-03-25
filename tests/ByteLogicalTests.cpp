//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(ByteLogicalTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

TEST(ByteLogicalTests, INC_B) {
    //INC B   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x4);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setB(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getB());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x4);
    cpu->setB(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getB());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getB());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_B) {
    //DEC B   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0x5);
    cpu->setFlag(FLAG_Z);
    cpu->setB(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getB());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x5);
    cpu->setB(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getB());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getB());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, INC_C) {
    //INC C   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0xc);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setC(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getC());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0xc);
    cpu->setC(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getC());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getC());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_C) {
    //DEC C   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0xd);
    cpu->setFlag(FLAG_Z);
    cpu->setC(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getC());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0xd);
    cpu->setC(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getC());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getC());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}