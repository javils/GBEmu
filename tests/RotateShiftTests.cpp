//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(RotateShiftTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;

    uint8_t refs [8];

    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));
    }

    void teardown() {
    }

    void updateRefs() {
        refs[0] = cpu->getB();
        refs[1] = cpu->getC();
        refs[2] = cpu->getD();
        refs[3] = cpu->getE();
        refs[4] = cpu->getH();
        refs[5] = cpu->getL();
        refs[6] = cpu->readByteMem(cpu->getHL());
        refs[7] = cpu->getA();
    }
};

TEST(RotateShiftTests, RLCA) {
    //RLCA   4 cycles   0 0 0 C
    cpu->writeByteMem(0x0, 0x7);
    cpu->setA(0x12);
    cpu->setFlag(FLAG_Z | FLAG_N | FLAG_C | FLAG_H);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x24, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
}

TEST(RotateShiftTests, RRCA) {
    //RRCA   4 cycles   0 0 0 C
    cpu->writeByteMem(0x0, 0xf);
    cpu->setA(0x13);
    cpu->setFlag(FLAG_Z | FLAG_N | FLAG_C | FLAG_H);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x89, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
}

TEST(RotateShiftTests, RLA) {
    //RLA   4 cycles   0 0 0 C
    cpu->writeByteMem(0x0, 0x17);
    cpu->setA(0x12);
    cpu->setFlag(FLAG_Z | FLAG_N | FLAG_C | FLAG_H);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x25, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x1, 0x17);
    cpu->setA(0x81);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x03, cpu->getA());
    LONGS_EQUAL(2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
}

TEST(RotateShiftTests, RRA) {
    //RRA   4 cycles   0 0 0 C
    cpu->writeByteMem(0x0, 0x1f);
    cpu->setA(0x12);
    cpu->setFlag(FLAG_Z | FLAG_N | FLAG_C | FLAG_H);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x89, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x1, 0x1f);
    cpu->setA(0x81);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xC0, cpu->getA());
    LONGS_EQUAL(2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
}

// CB OPCODES
TEST(RotateShiftTests, RLC_R) {
    //RLC B   8 cycles   Z 0 0 C
    uint8_t counter = 0x00;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x80);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (refs[i] << 1);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(uint8_t(expected | cpu->getFlag(FLAG_C)), refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, RRC_R) {
    //RRC B   8 cycles   Z 0 0 C
    uint8_t counter = 0x08;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x81);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (refs[i] >> 1);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL((cpu->getFlag(FLAG_C) << 7) | expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, RL_R) {
    //RL B   8 cycles   Z 0 0 C
    uint8_t counter = 0x10;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x80);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (refs[i] << 1) | cpu->getFlag(FLAG_C);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, RR_R) {
    //RR B   8 cycles   Z 0 0 C
    uint8_t counter = 0x18;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x81);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (cpu->getFlag(FLAG_C) << 7) | (refs[i] >> 1);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, SLA_R) {
    //SLA R   8 cycles   Z 0 0 C
    uint8_t counter = 0x20;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x80);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (refs[i] << 1);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, SRA_R) {
    //SRA R   8 cycles   Z 0 0 C
    uint8_t counter = 0x28;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x80);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (uint8_t) ((refs[i] & 0x80) << 7 | (refs[i] >> 1));
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, SWAP_R) {
    //SWAP R   8 cycles   Z 0 0 0
    uint8_t counter = 0x30;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x01);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (refs[i] << 4) | (refs[i] >> 4);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}

TEST(RotateShiftTests, SRL_R) {
    //SRL R   8 cycles   Z 0 0 C
    uint8_t counter = 0x38;
    cpu->writeByteMem(0x0, 0xCB);
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(cpu->getCP(), 0xCB);
        cpu->writeByteMem((uint16_t) (cpu->getCP() + 1), counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x01);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        updateRefs();
        uint8_t expected = (refs[i] >> 1);
        cpu->executeNextOpcode();
        updateRefs();
        LONGS_EQUAL(expected, refs[i]);
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(104, cpu->getClockCounter());
}