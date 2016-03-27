//
// Created by Javier Luque Sanabria on 25/3/16.
//
#include <iostream>
#include "CppUTest/TestHarness.h"
#include "Z80.h"

TEST_GROUP(ByteLoadOpcodeTests){
    unique_ptr<Z80> cpu;
    unique_ptr<BasicMemory> mem;

    uint8_t refs [8];

    void setup() {
        mem.reset(new Memory_DMG());
        cpu.reset(new Z80(move(mem)));

    }

    void teardown() {
    }

    void updateRefs(bool ptrHL) {
        refs[0] = cpu->getB();
        refs[1] = cpu->getC();
        refs[2] = cpu->getD();
        refs[3] = cpu->getE();
        refs[4] = cpu->getH();
        refs[5] = cpu->getL();
        if (ptrHL)
            refs[6] = cpu->readByteMem(cpu->getHL());
        refs[7] = cpu->getA();
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

TEST(ByteLoadOpcodeTests, LD_PTR_DE_A) {
    //LD (DE),A   8 cycles
    cpu->writeByteMem(0x0, 0x12);
    cpu->setDE(0x20);
    cpu->setA(0x12);
    LONGS_EQUAL(0, cpu->readByteMem(cpu->getDE()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x20, cpu->getDE());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(cpu->getA(), cpu->readByteMem(cpu->getDE()));
}

TEST(ByteLoadOpcodeTests, LD_D_d8) {
    //LD D,d8   8 cycles
    cpu->writeByteMem(0x0, 0x16);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getD());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, LD_A_PTR_DE) {
    //LD A,(DE)   8 cycles
    cpu->writeByteMem(0x0, 0x1a);
    cpu->setDE(0x5854);
    cpu->writeByteMem(cpu->getDE(), 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, LD_E_d8) {
    //LD E,d8   8 cycles
    cpu->writeByteMem(0x0, 0x1e);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getE());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, LD_PTR_HL_INC_A) {
    //LD (HL+),A   8 cycles   - - - -
    cpu->writeByteMem(0x0, 0x22);
    cpu->setHL(0x20);
    cpu->setA(0x12);
    LONGS_EQUAL(0, cpu->readByteMem(cpu->getHL()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x21, cpu->getHL());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(cpu->getA(), cpu->readByteMem(cpu->getHL() - 1));
}

TEST(ByteLoadOpcodeTests, LD_H_d8) {
    //LD H,d8   8 cycles
    cpu->writeByteMem(0x0, 0x26);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getH());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, DAA) {
    //DAA   4 cycles   Z - 0 C
    cpu->writeByteMem(0x0, 0x27);
    cpu->setA(0x3C);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x42, cpu->getA());
    LONGS_EQUAL(0x1, cpu->getCP());
    LONGS_EQUAL(4, cpu->getClockCounter());
    LONGS_EQUAL(0, cpu->getFlag(FLAG_Z));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_H));
    LONGS_EQUAL(0, cpu->getFlag(FLAG_C));
}

TEST(ByteLoadOpcodeTests, LD_A_PTR_HL_INC) {
    //LD A,(HL+)   8 cycles   - - - -
    cpu->writeByteMem(0x0, 0x2A);
    cpu->setHL(0x20);
    cpu->setA(0x12);
    cpu->writeByteMem(cpu->getHL(), 0x02);
    LONGS_EQUAL(2, cpu->readByteMem(cpu->getHL()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x02, cpu->getA());
    LONGS_EQUAL(0x21, cpu->getHL());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(cpu->getA(), cpu->readByteMem(cpu->getHL() - 1));
}

TEST(ByteLoadOpcodeTests, LD_L_d8) {
    //LD L,d8   8 cycles
    cpu->writeByteMem(0x0, 0x2e);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getL());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, LD_PTR_HL_DEC_A) {
    //LD (HL-),A   8 cycles   - - - -
    cpu->writeByteMem(0x0, 0x32);
    cpu->setHL(0x20);
    cpu->setA(0x12);
    LONGS_EQUAL(0, cpu->readByteMem(cpu->getHL()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x1F, cpu->getHL());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(cpu->getA(), cpu->readByteMem(cpu->getHL() + 1));
}

TEST(ByteLoadOpcodeTests, LD_PTR_HL_d8) {
    //LD (HL),d8   12 cycles   - - - -
    cpu->writeByteMem(0x0, 0x36);
    cpu->writeByteMem(0x1, 0x12);
    cpu->setHL(0x20);
    LONGS_EQUAL(0, cpu->readByteMem(cpu->getHL()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x20, cpu->getHL());
    LONGS_EQUAL(2, cpu->getCP());
    LONGS_EQUAL(12, cpu->getClockCounter());
    LONGS_EQUAL(0x12, cpu->readByteMem(cpu->getHL()));
}

TEST(ByteLoadOpcodeTests, LD_A_PTR_HL_DEC) {
    //LD A,(HL-)   8 cycles   - - - -
    cpu->writeByteMem(0x0, 0x3A);
    cpu->setHL(0x20);
    cpu->setA(0x12);
    cpu->writeByteMem(cpu->getHL(), 0x02);
    LONGS_EQUAL(2, cpu->readByteMem(cpu->getHL()));
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x02, cpu->getA());
    LONGS_EQUAL(0x1F, cpu->getHL());
    LONGS_EQUAL(1, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
    LONGS_EQUAL(cpu->getA(), cpu->readByteMem(cpu->getHL() + 1));
}

TEST(ByteLoadOpcodeTests, LD_A_d8) {
    //LD A,d8   8 cycles
    cpu->writeByteMem(0x0, 0x3e);
    cpu->writeByteMem(0x1, 0x12);
    cpu->executeNextOpcode();
    LONGS_EQUAL(0x12, cpu->getA());
    LONGS_EQUAL(0x2, cpu->getCP());
    LONGS_EQUAL(8, cpu->getClockCounter());
}

TEST(ByteLoadOpcodeTests, LD_R1_R2) {
    //LD B,B   4 cycles   - - - -
    uint8_t counter = 0x40;
    uint16_t addr = 0;
    for(int i = 0; i < 8;i++) { // R1
        for(int j = 0; j < 8;j++) {  //R2
            // HALT
            if (counter == 0x76) {
                counter++;
                addr++;
                cpu->incCP();
                continue;
            }
            cpu->writeByteMem(addr, counter);
            cpu->setA(0x02);
            cpu->setBC(0x1234);
            cpu->setDE(0x5678);
            cpu->setHL(0x9012);
            cpu->writeByteMem(cpu->getHL(), 0x35);
            cpu->executeNextOpcode();
            // LD H,(HL)            LD L,(HL)
            if (counter == 0x66 || counter == 0x6E) {
                updateRefs(false);
                refs[6] = cpu->readByteMem(0x9012);
            }
            else
                updateRefs(true);
            LONGS_EQUAL(refs[i], refs[j]);
            counter++;
            addr++;
        }
    }
    LONGS_EQUAL(308, cpu->getClockCounter());

}