//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(ByteLoadOpcodeTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

TEST(ByteLoadOpcodeTests, LD_B_d8) {
    //LD B,d8   8 cycles
    cpu->writeByteMem(0x0, 0x6);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getB());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, LD_PTR_BC_A) {
    //LD (BC),A   8 cycles
    cpu->writeByteMem(0x0, 0x2);
    cpu->setBC(0x20);
    cpu->setA(0x12);
    LONGS_EQUAL(0, cpu->readByteMem(cpu->getBC()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x20, cpu->getBC());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(cpu->getA(), cpu->readByteMem(cpu->getBC()));
}

TEST(ByteLoadOpcodeTests, LD_A_PTR_BC) {
    //LD A,(BC)   8 cycles
    cpu->writeByteMem(0x0, 0xa);
    cpu->setBC(0x5854);
    cpu->writeByteMem(cpu->getBC(), 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}


TEST(ByteLoadOpcodeTests, LD_C_d8) {
    //LD C,d8   8 cycles
    cpu->writeByteMem(0x0, 0xe);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getC());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}