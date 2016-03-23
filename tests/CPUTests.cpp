//
// Created by Javier Luque Sanabria on 23/3/16.
//

#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(CPUTests){
    Z80 *cpu;
    void setup() {
        cpu = new Z80();
    }

    void teardown() {
        delete cpu;
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