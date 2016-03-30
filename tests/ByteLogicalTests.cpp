//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(ByteLogicalTests){
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

TEST(ByteLogicalTests, INC_D) {
    //INC D   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x14);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setD(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getD());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x14);
    cpu->setD(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getD());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getD());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_D) {
    //DEC D   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0x15);
    cpu->setFlag(FLAG_Z);
    cpu->setD(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getD());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x15);
    cpu->setD(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getD());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getD());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, INC_E) {
    //INC E   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x1c);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setE(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getE());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x1c);
    cpu->setE(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getE());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getE());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_E) {
    //DEC E   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0x1d);
    cpu->setFlag(FLAG_Z);
    cpu->setE(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getE());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x1d);
    cpu->setE(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getE());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getE());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, INC_H) {
    //INC H   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x24);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setH(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getH());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x24);
    cpu->setH(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getH());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getH());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_H) {
    //DEC E   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0x25);
    cpu->setFlag(FLAG_Z);
    cpu->setH(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getH());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x25);
    cpu->setH(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getH());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getH());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, INC_L) {
    //INC L   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x2c);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setL(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getL());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x2c);
    cpu->setL(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getL());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_L) {
    //DEC L   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0x2d);
    cpu->setFlag(FLAG_Z);
    cpu->setL(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getL());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x2d);
    cpu->setL(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getL());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getL());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, CPL) {
    //CPL   4 cycles   - 1 1 -
    cpu->writeByteMem(0x0, 0x2f);
    cpu->setA(0x01);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFE, cpu->getA());
    LONGS_EQUAL(0x01, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
}


TEST(ByteLogicalTests, INC_A) {
    //INC A   4 cycles   Z 0 H -
    cpu->writeByteMem(0x0, 0x3c);
    cpu->setFlag(FLAG_Z | FLAG_N);
    cpu->setA(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x13, cpu->getA());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x3c);
    cpu->setA(0xFF);
    cpu->resetFlag(FLAG_Z);
    LONGS_EQUAL(0xFF, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getA());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, DEC_A) {
    //DEC A   4 cycles   Z 1 H -
    cpu->writeByteMem(0x0, 0x3d);
    cpu->setFlag(FLAG_Z);
    cpu->setA(0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x11, cpu->getA());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));

    cpu->writeByteMem(0x1, 0x3d);
    cpu->setA(0x00);
    cpu->setFlag(FLAG_Z);
    LONGS_EQUAL(0x00, cpu->getA());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(4, cpu->getClockCounter());
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFF, cpu->getA());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
}

TEST(ByteLogicalTests, ADD_A_R) {
    //ADD A,R   4 cycles   Z 0 H C
    uint8_t counter = 0x80;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(i, counter);
        cpu->setA(0x2);
        cpu->setB(0x3);
        cpu->setC(0xFE);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0x2 + refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
        // Register C    check FLAGS
        if (i == 1) {
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
        }
        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, ADC_A_R) {
    //ADD A,R   4 cycles   Z 0 H C
    uint8_t counter = 0x88;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(i, counter);
        cpu->setFlag(FLAG_C);
        cpu->setA(0x2);
        cpu->setB(0x3);
        cpu->setC(0xFD);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0x3 + refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
        // Register C    check FLAGS
        if (i == 1) {
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
        }
        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, SUB_A_R) {
    //SUB A,R   4 cycles   Z 1 H C
    uint8_t counter = 0x90;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(i, counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0xFE);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0xCE - refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
        // Register C    check FLAGS
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
        }
        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, SBC_A_R) {
    //SBC A,R   4 cycles   Z 1 H C
    uint8_t counter = 0x98;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->setFlag(FLAG_C);
        cpu->writeByteMem(i, counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0xFE);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0xCD - refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
        // Register C    check FLAGS
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(1, cpu->getFlag(FLAG_C));
        }
        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, AND_A_R) {
    //AND A,R   4 cycles    Z 0 1 0
    uint8_t counter = 0xa0;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->setFlag(FLAG_C);
        cpu->writeByteMem(i, counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x00);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0xCE & refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
        LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
        LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
        // Register C    check FLAGS
        if (i == 1) {
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
        }
        else {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, XOR_A_R) {
    //XOR A,R   4 cycles    Z 0 0 0
    uint8_t counter = 0xa8;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(i, counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x4);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0xCE ^ refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
        LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
        LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
        // Register A    check FLAGS
        if (i == 7) {
            LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
        }
        else {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, OR_A_R) {
    //OR A,R   4 cycles    Z 0 0 0
    uint8_t counter = 0xb0;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(i, counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0x4);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        uint8_t result = (uint8_t) ((i != 7) ? 0xCE | refs[i] : refs[i]);
        LONGS_EQUAL(result, cpu->getA());
        LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
        LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
        LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
        LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));

        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, CP_A_R) {
    //CP A,R   4 cycles    Z 1 H C
    uint8_t counter = 0xb0;
    for(uint16_t i = 0; i < 8; i++) {
        cpu->writeByteMem(i, counter);
        cpu->setA(0xCE);
        cpu->setB(0x3);
        cpu->setC(0xCE);
        cpu->setD(0x5);
        cpu->setE(0x6);
        cpu->setH(0x7);
        cpu->setL(0x8);
        cpu->writeByteMem(cpu->getHL(), 0x6);
        cpu->executeNextOpcode();
        updateRefs();
        // Register C
        if (i == 1) {
            LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
            LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
        }

        counter++;
    }
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(ByteLogicalTests, ADD_A_N) {
    //ADD A,N   8 cycles   Z 0 H C
    cpu->writeByteMem(0x00, 0xC6);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x6, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xC6);
    cpu->writeByteMem(0x03, 0x0F);
    cpu->setA(0xFE);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xD, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, ADC_A_N) {
    //ADC A,N   8 cycles   Z 0 H C
    cpu->writeByteMem(0x00, 0xCE);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x7, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xC6);
    cpu->writeByteMem(0x03, 0x0F);
    cpu->setA(0xFE);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xD, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, SUB_A_N) {
    //SUB d8   8 cycles   Z 1 H C
    cpu->writeByteMem(0x00, 0xD6);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFE, cpu->getA());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xD6);
    cpu->writeByteMem(0x03, 0x0F);
    cpu->setA(0xFE);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xEF, cpu->getA());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, SBC_A_N) {
    //SBC d8   8 cycles   Z 1 H C
    cpu->writeByteMem(0x00, 0xDE);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setFlag(FLAG_C);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xFD, cpu->getA());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xDE);
    cpu->writeByteMem(0x03, 0x0F);
    cpu->setA(0xFE);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xEE, cpu->getA());
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, AND_A_N) {
    //AND d8   8 cycles   Z 0 1 0
    cpu->writeByteMem(0x00, 0xE6);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x0, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xE6);
    cpu->writeByteMem(0x03, 0xF2);
    cpu->setA(0xFE);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0xF2, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, XOR_A_N) {
    //XOR d8   8 cycles   Z 0 0 0
    cpu->writeByteMem(0x00, 0xEE);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x6, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xEE);
    cpu->writeByteMem(0x03, 0xFE);
    cpu->setA(0xFE);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, OR_A_N) {
    //OR d8   8 cycles   Z 0 0 0
    cpu->writeByteMem(0x00, 0xEE);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x6, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xEE);
    cpu->writeByteMem(0x03, 0x0);
    cpu->setA(0x0);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0, cpu->getA());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(ByteLogicalTests, CP_A_N) {
    //CP d8   8 cycles   Z 1 H C
    cpu->writeByteMem(0x00, 0xFE);
    cpu->writeByteMem(0x01, 0x04);
    cpu->setA(0x2);
    cpu->executeNextOpcode();
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(1, cpu->getFlag(FLAG_C));

    cpu->writeByteMem(0x02, 0xFE);
    cpu->writeByteMem(0x03, 0x03);
    cpu->setA(0x04);
    cpu->executeNextOpcode();
    LONGS_EQUAL(1, cpu->getFlag(FLAG_N));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));

    LONGS_EQUAL(16, cpu->getClockCounter());
}