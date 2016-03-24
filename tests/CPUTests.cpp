//
// Created by Javier Luque Sanabria on 23/3/16.
//

#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(CPUTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;
    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }
};

// Disable warnings for overflow here.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
TEST(CPUTests, Overflow){
    cpu->setAF(0x10012);
    LONGS_EQUAL(0x12, cpu->getAF());
    cpu->setCP(-1);
    LONGS_EQUAL(0xFFFF, cpu->getCP());
}
#pragma clang diagnostic pop

TEST(CPUTests, WordRegisters) {
    // AF
    cpu->setAF(0x1234);
    LONGS_EQUAL(0x1234, cpu->getAF());
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x34, cpu->getF());

    // BC
    cpu->setBC(0x1234);
    LONGS_EQUAL(0x1234, cpu->getBC());
    LONGS_EQUAL(0x12, cpu->getB());
    LONGS_EQUAL(0x34, cpu->getC());

    // DE
    cpu->setDE(0x1234);
    LONGS_EQUAL(0x1234, cpu->getDE());
    LONGS_EQUAL(0x12, cpu->getD());
    LONGS_EQUAL(0x34, cpu->getE());

    // HL
    cpu->setHL(0x1234);
    LONGS_EQUAL(0x1234, cpu->getHL());
    LONGS_EQUAL(0x12, cpu->getH());
    LONGS_EQUAL(0x34, cpu->getL());
};

TEST(CPUTests, ByteRegisters) {
    // A and F
    cpu->setA(0x12);
    cpu->setF(0x34);
    LONGS_EQUAL(0x1234, cpu->getAF());
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x34, cpu->getF());

    // B and C
    cpu->setB(0x12);
    cpu->setC(0x34);
    LONGS_EQUAL(0x1234, cpu->getBC());
    LONGS_EQUAL(0x12, cpu->getB());
    LONGS_EQUAL(0x34, cpu->getC());

    // D and E
    cpu->setD(0x12);
    cpu->setE(0x34);
    LONGS_EQUAL(0x1234, cpu->getDE());
    LONGS_EQUAL(0x12, cpu->getD());
    LONGS_EQUAL(0x34, cpu->getE());

    // H and L
    cpu->setH(0x12);
    cpu->setL(0x34);
    LONGS_EQUAL(0x1234, cpu->getHL());
    LONGS_EQUAL(0x12, cpu->getH());
    LONGS_EQUAL(0x34, cpu->getL());
};

TEST(CPUTests, IncCP){
    cpu->setCP(0x100);
    cpu->incCP();
    LONGS_EQUAL(0x101, cpu->getCP());
};

TEST(CPUTests, Flags){
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
}

TEST(CPUTests, MemoryTests) {
    cpu->writeByteMem(0x1, 0x1);
    cpu->writeByteMem(0x2, 0x2);
    cpu->writeByteMem(0x3, 0x3);
    cpu->writeWordMem(0x4, 0x9876);

    LONGS_EQUAL(0x0, cpu->readByteMem(0x0));
    LONGS_EQUAL(0x1, cpu->readByteMem(0x1));
    LONGS_EQUAL(0x2, cpu->readByteMem(0x2));
    LONGS_EQUAL(0x3, cpu->readByteMem(0x3));
    LONGS_EQUAL(0x76, cpu->readByteMem(0x04));
    LONGS_EQUAL(0x98, cpu->readByteMem(0x05));

    LONGS_EQUAL(0x9876, cpu->readWordMem(0x4));
    LONGS_EQUAL(0x0098, cpu->readWordMem(0x5));
    LONGS_EQUAL(0x0, cpu->readWordMem(0x06));
}