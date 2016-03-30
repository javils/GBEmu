//
// Created by Javier Luque Sanabria on 30/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(SingleBitOpTests){
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

TEST(SingleBitOpTests, BIT_N_R) {
    //BIT N,R   8 cycles   Z 0 1 -
    uint8_t counter = 0x40;
    cpu->writeByteMem(0x0, 0xCB);
    for (uint8_t i = 0; i < 8; i++) {
        for (uint16_t j = 0; j < 8; j++) {
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
            uint8_t expected = (uint8_t) (refs[j] & (1 << i));
            cpu->executeNextOpcode();
            updateRefs();
            LONGS_EQUAL(expected, refs[j]& (1 << i));
            // Bit 0 Register C
            if (j == 0 && i == 1) {
                LONGS_EQUAL(0, cpu->getFlag(FLAG_N));
                LONGS_EQUAL(1, cpu->getFlag(FLAG_H));
                LONGS_EQUAL(1, cpu->getFlag(FLAG_Z));
            }

            counter++;
        }
    }
    LONGS_EQUAL(832, cpu->getClockCounter());
}


TEST(SingleBitOpTests, RES_N_R) {
    //RES N,R   8 cycles   - - - -
    uint8_t counter = 0x80;
    cpu->writeByteMem(0x0, 0xCB);
    for (uint8_t i = 0; i < 8; i++) {   // N
        for (uint16_t j = 0; j < 8; j++) {  // R
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
            cpu->executeNextOpcode();
            updateRefs();
            LONGS_EQUAL(0, (refs[j] & (1 << i)) >> i);

            counter++;
        }
    }
    LONGS_EQUAL(832, cpu->getClockCounter());
}

TEST(SingleBitOpTests, SET_N_R) {
    //SET N,R   8 cycles   - - - -
    uint8_t counter = 0xC0;
    cpu->writeByteMem(0x0, 0xCB);
    for (uint8_t i = 0; i < 8; i++) {   // N

        //printf("\n%d\n", i);
        for (uint16_t j = 0; j < 8; j++) {  // R
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
            cpu->executeNextOpcode();
            updateRefs();
            LONGS_EQUAL(1, (refs[j] & (1 << i)) >> i);

            counter++;
        }

    }
    LONGS_EQUAL(832, cpu->getClockCounter());
}