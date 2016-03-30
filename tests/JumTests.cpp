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

TEST(JumpTests, RET_NZ) {
    //RET NZ   20/8 cycles   - - - -
    cpu->writeByteMem(0x0, 0xC0);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(20, cpu->getClockCounter());
}

TEST(JumpTests, JP_NZ_NN) {
    //JP NZ,a16   16/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xC2);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->resetFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());

    cpu->writeByteMem((uint16_t) (cpu->getCP()), 0xC2);
    cpu->writeWordMem((uint16_t) (cpu->getCP() + 0x1), 0x5678);
    cpu->setFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(28, cpu->getClockCounter());
}

TEST(JumpTests, JP_NN) {
    //JP a16   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xC3);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, CALL_NZ_NN) {
    //CALL NZ,a16   24/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xC4);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->resetFlag(FLAG_Z);
    cpu->setSP(0x9E04);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(24, cpu->getClockCounter());

    cpu->writeByteMem(0x1234, 0xC4);
    cpu->writeWordMem(0x1235, 0x4567);
    cpu->setFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(JumpTests, RST_00) {
    //RST 00H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xC7);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x0, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, RET_Z) {
    //RET Z   20/8 cycles   - - - -
    cpu->writeByteMem(0x0, 0xC8);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->setFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(20, cpu->getClockCounter());
}

TEST(JumpTests, RET) {
    //RET   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xC9);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, JP_Z_NN) {
    //JP Z,a16   16/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xCA);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->setFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());

    cpu->writeByteMem((uint16_t) (cpu->getCP()), 0xCA);
    cpu->writeWordMem((uint16_t) (cpu->getCP() + 0x1), 0x5678);
    cpu->resetFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(28, cpu->getClockCounter());
}

TEST(JumpTests, CALL_Z_NN) {
    //CALL Z,a16   24/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xCC);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->setFlag(FLAG_Z);
    cpu->setSP(0x9E04);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(24, cpu->getClockCounter());

    cpu->writeByteMem(0x1234, 0xCC);
    cpu->writeWordMem(0x1235, 0x4567);
    cpu->resetFlag(FLAG_Z);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(JumpTests, CALL_NN) {
    //CALL a16   24 cycles   - - - -
    cpu->writeByteMem(0x0, 0xCD);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->setSP(0x9E04);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(24, cpu->getClockCounter());
}

TEST(JumpTests, RST_08) {
    //RST 08H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xCF);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x8, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, RET_NC) {
    //RET NC   20/8 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD0);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->resetFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(20, cpu->getClockCounter());
}

TEST(JumpTests, JP_NC_NN) {
    //JP NC,a16   16/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD2);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->resetFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());

    cpu->writeByteMem((uint16_t) (cpu->getCP()), 0xD2);
    cpu->writeWordMem((uint16_t) (cpu->getCP() + 0x1), 0x5678);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(28, cpu->getClockCounter());
}

TEST(JumpTests, CALL_NC_NN) {
    //CALL NC,a16   24/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD4);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->resetFlag(FLAG_C);
    cpu->setSP(0x9E04);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(24, cpu->getClockCounter());

    cpu->writeByteMem(0x1234, 0xD4);
    cpu->writeWordMem(0x1235, 0x4567);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(JumpTests, RST_10) {
    //RST 10H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xD7);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x10, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, RET_C) {
    //RET C   20/8 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD8);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(20, cpu->getClockCounter());
}

TEST(JumpTests, RETI) {
    //RETI   16 cycles   - - - -
    cpu->writeByteMem(0x0, 0xD9);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EF0, cpu->getSP());
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
    FAIL("INTERRUPTS NOT IMPLEMENTED YET");
}

TEST(JumpTests, JP_C_NN) {
    //JP C,a16   16/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xDA);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->setFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());

    cpu->writeByteMem((uint16_t) (cpu->getCP()), 0xDA);
    cpu->writeWordMem((uint16_t) (cpu->getCP() + 0x1), 0x5678);
    cpu->resetFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(28, cpu->getClockCounter());
}

TEST(JumpTests, CALL_C_NN) {
    //CALL C,a16   24/12 cycles   - - - -
    cpu->writeByteMem(0x0, 0xDC);
    cpu->writeWordMem(0x1, 0x1234);
    cpu->setFlag(FLAG_C);
    cpu->setSP(0x9E04);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(24, cpu->getClockCounter());

    cpu->writeByteMem(0x1234, 0xDC);
    cpu->writeWordMem(0x1235, 0x4567);
    cpu->resetFlag(FLAG_C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1237, cpu->getCP());
    LONGS_EQUAL(0x9E02, cpu->getSP());
    LONGS_EQUAL(0x03, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(36, cpu->getClockCounter());
}

TEST(JumpTests, RST_18) {
    //RST 18H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xDF);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x18, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, RST_20) {
    //RST 20H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xE7);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x20, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, JP_PTR_HL) {
    //JP HL   4 cycles   - - - -
    cpu->writeByteMem(0x0, 0xE9);
    cpu->setHL(0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x1234, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
}

TEST(JumpTests, RST_28) {
    //RST 28H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xEF);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x28, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, RST_30) {
    //RST 30H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xF7);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x30, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}

TEST(JumpTests, RST_38) {
    //RST 30H   16 cycles   - - - -
    cpu->writeByteMem(0x2, 0xFF);
    cpu->setCP(0x2);
    cpu->setSP(0x9EEE);
    cpu->writeWordMem(cpu->getSP(), 0x1234);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x9EEC, cpu->getSP());
    LONGS_EQUAL(0x3, cpu->readWordMem(cpu->getSP()));
    LONGS_EQUAL(0x38, cpu->getCP());
    LONGS_EQUAL(16, cpu->getClockCounter());
}