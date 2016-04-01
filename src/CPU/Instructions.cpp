//
// Created by Javier Luque Sanabria on 27/3/16.
//

#include <Z80.h>


/**
 * This method is created for clean the Z80 file and out this ugly code
 * of Z80.c that is the principal file.
 * @param opcode opcode to be executed.
 */
void Z80::executeInstruction(uint8_t opcode) {
    switch (opcode) {
        case 0x0://NOP   4 cycles   - - - -
            addClockCounter(4);
            break;
        case 0x1://LD BC,d16   12 cycles   - - - -
            op_ld_r16_d16(refBC());
            break;
        case 0x2://LD (BC),A   8 cycles   - - - -
            op_ld_ptr_r16_r8(getBC(), getA());
            break;
        case 0x3://INC BC   8 cycles   - - - -
            op_inc_16(refBC());
            break;
        case 0x4://INC B   4 cycles   Z 0 H -
            op_inc_8(refB());
            break;
        case 0x5://DEC B   4 cycles   Z 1 H -
            op_dec_8(refB());
            break;
        case 0x6://LD B,d8   8 cycles   - - - -
            op_ld_r8_d8(refB());
            break;
        case 0x7://RLCA   4 cycles   0 0 0 C
            setFlagCond(FLAG_C, (getA() & 0x80) != 0);
            resetFlag(FLAG_Z | FLAG_H | FLAG_N);
            setA((getA() << 1) | getFlag(FLAG_C));
            addClockCounter(4);
            break;
        case 0x8://LD (a16),SP   20 cycles   - - - -
            writeWordMem(readWordMem(getCP()), getSP());
            incCP();
            incCP();   // Read word from memory -> 2 inc CP.
            addClockCounter(20);
            break;
        case 0x9://ADD HL,BC   8 cycles   - 0 H C
            op_add_hl_r16(getBC());
            break;
        case 0xa://LD A,(BC)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refA(), getBC());
            break;
        case 0xb://DEC BC   8 cycles   - - - -
            op_dec_16(refBC());
            break;
        case 0xc://INC C   4 cycles   Z 0 H -
            op_inc_8(refC());
            break;
        case 0xd://DEC C   4 cycles   Z 1 H -
            op_dec_8(refC());
            break;
        case 0xe://LD C,d8   8 cycles   - - - -
            op_ld_r8_d8(refC());
            break;
        case 0xf://RRCA   4 cycles   0 0 0 C
            setFlagCond(FLAG_C, (getA() & 0x01) != 0);
            resetFlag(FLAG_Z | FLAG_H | FLAG_N);
            setA((getFlag(FLAG_C) << 7) | (getA() >> 1));
            addClockCounter(4);
            break;
        case 0x10://STOP 0   4 cycles   - - - -
            if (readByteMem(getCP()) != 0x00)  // STOP must be following by 00
            {
                //TODO: PRINTF ERROR OR SOMETHING.
            }
            incCP();
            cpuStatus = STOP;
            addClockCounter(4);
            break;
        case 0x11://LD DE,d16   12 cycles   - - - -
            op_ld_r16_d16(refDE());
            break;
        case 0x12://LD (DE),A   8 cycles   - - - -
            op_ld_ptr_r16_r8(getDE(), getA());
            break;
        case 0x13://INC DE   8 cycles   - - - -
            op_inc_16(refDE());
            break;
        case 0x14://INC D   4 cycles   Z 0 H -
            op_inc_8(refD());
            break;
        case 0x15://DEC D   4 cycles   Z 1 H -
            op_dec_8(refD());
            break;
        case 0x16://LD D,d8   8 cycles   - - - -
            op_ld_r8_d8(refD());
            break;
        case 0x17://RLA   4 cycles   0 0 0 C
        {
            uint8_t c = getFlag(FLAG_C);
            setFlagCond(FLAG_C, (getA() & 0x80) != 0);
            resetFlag(FLAG_Z | FLAG_H | FLAG_N);
            setA((getA() << 1) | c);
            addClockCounter(4);
            break;
        }
        case 0x18://JR r8   12 cycles   - - - -
        {
            uint8_t r8 = readByteMem(getCP());
            incCP();
            setCP(getCP() + (int8_t) r8);    // this value is signed.
            addClockCounter(12);
            break;
        }
        case 0x19://ADD HL,DE   8 cycles   - 0 H C
            op_add_hl_r16(getDE());
            break;
        case 0x1a://LD A,(DE)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refA(), getDE());
            break;
        case 0x1b://DEC DE   8 cycles   - - - -
            op_dec_16(refDE());
            break;
        case 0x1c://INC E   4 cycles   Z 0 H -
            op_inc_8(refE());
            break;
        case 0x1d://DEC E   4 cycles   Z 1 H -
            op_dec_8(refE());
            break;
        case 0x1e://LD E,d8   8 cycles   - - - -
            op_ld_r8_d8(refE());
            break;
        case 0x1f://RRA   4 cycles   0 0 0 C
        {
            uint8_t c = getFlag(FLAG_C);
            setFlagCond(FLAG_C, (getA() & 0x01) != 0);
            resetFlag(FLAG_Z | FLAG_H | FLAG_N);
            setA((c << 7) | (getA() >> 1));
            addClockCounter(4);
            break;
        }
        case 0x20://JR NZ,r8   12/8 cycles   - - - -
            op_jr_cond_nn(getFlag(FLAG_Z) == 0);
            break;
        case 0x21://LD HL,d16   12 cycles   - - - -
            op_ld_r16_d16(refHL());
            break;
        case 0x22://LD (HL+),A   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getA());
            ++(*refHL());
            break;
        case 0x23://INC HL   8 cycles   - - - -
            op_inc_16(refHL());
            break;
        case 0x24://INC H   4 cycles   Z 0 H -
            op_inc_8(refH());
            break;
        case 0x25://DEC H   4 cycles   Z 1 H -
            op_dec_8(refH());
            break;
        case 0x26://LD H,d8   8 cycles   - - - -
            op_ld_r8_d8(refH());
            break;
        case 0x27://DAA   4 cycles   Z - 0 C
        {
            uint8_t correction = 0x0;

            if (getA() > 0x99 || getFlag(FLAG_C)) {
                correction |= 0x60;
                setFlag(FLAG_C);
            }
            else
                resetFlag(FLAG_C);

            if ((getA() & 0x0F) > 9 || getFlag(FLAG_H))
                correction |= 0x06;

            if (getFlag(FLAG_N) == 0)
                setA(getA() + correction);
            else
                setA(getA() - correction);

            setFlagCond(FLAG_Z, getA() == 0);
            resetFlag(FLAG_H);
            addClockCounter(4);
            break;
        }
        case 0x28://JR Z,r8   12/8 cycles   - - - -
            op_jr_cond_nn(getFlag(FLAG_Z));
            break;
        case 0x29://ADD HL,HL   8 cycles   - 0 H C
            op_add_hl_r16(getHL());
            break;
        case 0x2a://LD A,(HL+)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refA(), getHL());
            ++(*refHL());
            break;
        case 0x2b://DEC HL   8 cycles   - - - -
            op_dec_16(refHL());
            break;
        case 0x2c://INC L   4 cycles   Z 0 H -
            op_inc_8(refL());
            break;
        case 0x2d://DEC L   4 cycles   Z 1 H -
            op_dec_8(refL());
            break;
        case 0x2e://LD L,d8   8 cycles   - - - -
            op_ld_r8_d8(refL());
            break;
        case 0x2f://CPL   4 cycles   - 1 1 -
            setA(~getA());
            setFlag(FLAG_H);
            setFlag(FLAG_N);
            addClockCounter(4);
            break;
        case 0x30://JR NC,r8   12/8 cycles   - - - -
            op_jr_cond_nn(getFlag(FLAG_C) == 0);
            break;
        case 0x31://LD SP,d16   12 cycles   - - - -
            op_ld_r16_d16(refSP());
            break;
        case 0x32://LD (HL-),A   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getA());
            --(*refHL());
            break;
        case 0x33://INC SP   8 cycles   - - - -
            op_inc_16(refSP());
            break;
        case 0x34://INC (HL)   12 cycles   Z 0 H -
        {
            uint8_t mem = readByteMem(getHL());
            resetFlag(FLAG_N);
            setFlagCond(FLAG_H, (mem & 0x0F) == 0x0F);
            mem++;
            setFlagCond(FLAG_Z, mem == 0);
            writeByteMem(getHL(), mem);
            addClockCounter(12);
            break;
        }
        case 0x35://DEC (HL)   12 cycles   Z 1 H -
        {
            uint8_t mem = readByteMem(getHL());
            setFlag(FLAG_N);
            setFlagCond(FLAG_H, (mem & 0x0F) == 0x00);
            mem--;
            setFlagCond(FLAG_Z, mem == 0);
            writeByteMem(getHL(), mem);
            addClockCounter(12);
            break;
        }
        case 0x36://LD (HL),d8   12 cycles   - - - -
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            writeByteMem(getHL(), mem);
            addClockCounter(12);
            break;
        }
        case 0x37://SCF   4 cycles   - 0 0 1
            resetFlag(FLAG_N | FLAG_H);
            setFlag(FLAG_C);
            addClockCounter(4);
            break;
        case 0x38://JR C,r8   12/8 cycles   - - - -
            op_jr_cond_nn(getFlag(FLAG_C));
            break;
        case 0x39://ADD HL,SP   8 cycles   - 0 H C
            op_add_hl_r16(getSP());
            break;
        case 0x3a://LD A,(HL-)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refA(), getHL());
            --(*refHL());
            break;
        case 0x3b://DEC SP   8 cycles   - - - -
            op_dec_16(refSP());
            break;
        case 0x3c://INC A   4 cycles   Z 0 H -
            op_inc_8(refA());
            break;
        case 0x3d://DEC A   4 cycles   Z 1 H -
            op_dec_8(refA());
            break;
        case 0x3e://LD A,d8   8 cycles   - - - -
            op_ld_r8_d8(refA());
            break;
        case 0x3f://CCF   4 cycles   - 0 0 C
            resetFlag(FLAG_N | FLAG_H);
            setFlagCond(FLAG_C, getFlag(FLAG_C) == 0);
            addClockCounter(4);
            break;
        case 0x40://LD B,B   4 cycles   - - - -
            op_ld_r8_r8(refB(), getB());
            break;
        case 0x41://LD B,C   4 cycles   - - - -
            op_ld_r8_r8(refB(), getC());
            break;
        case 0x42://LD B,D   4 cycles   - - - -
            op_ld_r8_r8(refB(), getD());
            break;
        case 0x43://LD B,E   4 cycles   - - - -
            op_ld_r8_r8(refB(), getE());
            break;
        case 0x44://LD B,H   4 cycles   - - - -
            op_ld_r8_r8(refB(), getH());
            break;
        case 0x45://LD B,L   4 cycles   - - - -
            op_ld_r8_r8(refB(), getL());
            break;
        case 0x46://LD B,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refB(), getHL());
            break;
        case 0x47://LD B,A   4 cycles   - - - -
            op_ld_r8_r8(refB(), getA());
            break;
        case 0x48://LD C,B   4 cycles   - - - -
            op_ld_r8_r8(refC(), getB());
            break;
        case 0x49://LD C,C   4 cycles   - - - -
            op_ld_r8_r8(refC(), getC());
            break;
        case 0x4a://LD C,D   4 cycles   - - - -
            op_ld_r8_r8(refC(), getD());
            break;
        case 0x4b://LD C,E   4 cycles   - - - -
            op_ld_r8_r8(refC(), getE());
            break;
        case 0x4c://LD C,H   4 cycles   - - - -
            op_ld_r8_r8(refC(), getH());
            break;
        case 0x4d://LD C,L   4 cycles   - - - -
            op_ld_r8_r8(refC(), getL());
            break;
        case 0x4e://LD C,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refC(), getHL());
            break;
        case 0x4f://LD C,A   4 cycles   - - - -
            op_ld_r8_r8(refC(), getA());
            break;
        case 0x50://LD D,B   4 cycles   - - - -
            op_ld_r8_r8(refD(), getB());
            break;
        case 0x51://LD D,C   4 cycles   - - - -
            op_ld_r8_r8(refD(), getC());
            break;
        case 0x52://LD D,D   4 cycles   - - - -
            op_ld_r8_r8(refD(), getD());
            break;
        case 0x53://LD D,E   4 cycles   - - - -
            op_ld_r8_r8(refD(), getE());
            break;
        case 0x54://LD D,H   4 cycles   - - - -
            op_ld_r8_r8(refD(), getH());
            break;
        case 0x55://LD D,L   4 cycles   - - - -
            op_ld_r8_r8(refD(), getL());
            break;
        case 0x56://LD D,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refD(), getHL());
            break;
        case 0x57://LD D,A   4 cycles   - - - -
            op_ld_r8_r8(refD(), getA());
            break;
        case 0x58://LD E,B   4 cycles   - - - -
            op_ld_r8_r8(refE(), getB());
            break;
        case 0x59://LD E,C   4 cycles   - - - -
            op_ld_r8_r8(refE(), getC());
            break;
        case 0x5a://LD E,D   4 cycles   - - - -
            op_ld_r8_r8(refE(), getD());
            break;
        case 0x5b://LD E,E   4 cycles   - - - -
            op_ld_r8_r8(refE(), getE());
            break;
        case 0x5c://LD E,H   4 cycles   - - - -
            op_ld_r8_r8(refE(), getH());
            break;
        case 0x5d://LD E,L   4 cycles   - - - -
            op_ld_r8_r8(refE(), getL());
            break;
        case 0x5e://LD E,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refE(), getHL());
            break;
        case 0x5f://LD E,A   4 cycles   - - - -
            op_ld_r8_r8(refE(), getA());
            break;
        case 0x60://LD H,B   4 cycles   - - - -
            op_ld_r8_r8(refH(), getB());
            break;
        case 0x61://LD H,C   4 cycles   - - - -
            op_ld_r8_r8(refH(), getC());
            break;
        case 0x62://LD H,D   4 cycles   - - - -
            op_ld_r8_r8(refH(), getD());
            break;
        case 0x63://LD H,E   4 cycles   - - - -
            op_ld_r8_r8(refH(), getE());
            break;
        case 0x64://LD H,H   4 cycles   - - - -
            op_ld_r8_r8(refH(), getH());
            break;
        case 0x65://LD H,L   4 cycles   - - - -
            op_ld_r8_r8(refH(), getL());
            break;
        case 0x66://LD H,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refH(), getHL());
            break;
        case 0x67://LD H,A   4 cycles   - - - -
            op_ld_r8_r8(refH(), getA());
            break;
        case 0x68://LD L,B   4 cycles   - - - -
            op_ld_r8_r8(refL(), getB());
            break;
        case 0x69://LD L,C   4 cycles   - - - -
            op_ld_r8_r8(refL(), getC());
            break;
        case 0x6a://LD L,D   4 cycles   - - - -
            op_ld_r8_r8(refL(), getD());
            break;
        case 0x6b://LD L,E   4 cycles   - - - -
            op_ld_r8_r8(refL(), getE());
            break;
        case 0x6c://LD L,H   4 cycles   - - - -
            op_ld_r8_r8(refL(), getH());
            break;
        case 0x6d://LD L,L   4 cycles   - - - -
            op_ld_r8_r8(refL(), getL());
            break;
        case 0x6e://LD L,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refL(), getHL());
            break;
        case 0x6f://LD L,A   4 cycles   - - - -
            op_ld_r8_r8(refL(), getA());
            break;
        case 0x70://LD (HL),B   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getB());
            break;
        case 0x71://LD (HL),C   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getC());
            break;
        case 0x72://LD (HL),D   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getD());
            break;
        case 0x73://LD (HL),E   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getE());
            break;
        case 0x74://LD (HL),H   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getH());
            break;
        case 0x75://LD (HL),L   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getL());
            break;
        case 0x76://HALT   4 cycles   - - - -
            cpuStatus = HALT;
            addClockCounter(4);
            break;
        case 0x77://LD (HL),A   8 cycles   - - - -
            op_ld_ptr_r16_r8(getHL(), getA());
            break;
        case 0x78://LD A,B   4 cycles   - - - -
            op_ld_r8_r8(refA(), getB());
            break;
        case 0x79://LD A,C   4 cycles   - - - -
            op_ld_r8_r8(refA(), getC());
            break;
        case 0x7a://LD A,D   4 cycles   - - - -
            op_ld_r8_r8(refA(), getD());
            break;
        case 0x7b://LD A,E   4 cycles   - - - -
            op_ld_r8_r8(refA(), getE());
            break;
        case 0x7c://LD A,H   4 cycles   - - - -
            op_ld_r8_r8(refA(), getH());
            break;
        case 0x7d://LD A,L   4 cycles   - - - -
            op_ld_r8_r8(refA(), getL());
            break;
        case 0x7e://LD A,(HL)   8 cycles   - - - -
            op_ld_r8_ptr_r16(refA(), getHL());
            break;
        case 0x7f://LD A,A   4 cycles   - - - -
            op_ld_r8_r8(refA(), getA());
            break;
        case 0x80://ADD A,B   4 cycles   Z 0 H C
            op_add_a_r8(getB());
            break;
        case 0x81://ADD A,C   4 cycles   Z 0 H C
            op_add_a_r8(getC());
            break;
        case 0x82://ADD A,D   4 cycles   Z 0 H C
            op_add_a_r8(getD());
            break;
        case 0x83://ADD A,E   4 cycles   Z 0 H C
            op_add_a_r8(getE());
            break;
        case 0x84://ADD A,H   4 cycles   Z 0 H C
            op_add_a_r8(getH());
            break;
        case 0x85://ADD A,L   4 cycles   Z 0 H C
            op_add_a_r8(getL());
            break;
        case 0x86://ADD A,(HL)   8 cycles   Z 0 H C
        {
            uint8_t mem = readByteMem(getHL());
            calc_flags(getA(), mem, false);
            setA(getA() + mem);
            addClockCounter(8);
            break;
        }
        case 0x87://ADD A,A   4 cycles   Z 0 H C
            op_add_a_r8(getA());
            break;
        case 0x88://ADC A,B   4 cycles   Z 0 H C
            op_adc_a_r8(getB());
            break;
        case 0x89://ADC A,C   4 cycles   Z 0 H C
            op_adc_a_r8(getC());
            break;
        case 0x8a://ADC A,D   4 cycles   Z 0 H C
            op_adc_a_r8(getD());
            break;
        case 0x8b://ADC A,E   4 cycles   Z 0 H C
            op_adc_a_r8(getE());
            break;
        case 0x8c://ADC A,H   4 cycles   Z 0 H C
            op_adc_a_r8(getH());
            break;
        case 0x8d://ADC A,L   4 cycles   Z 0 H C
            op_adc_a_r8(getL());
            break;
        case 0x8e://ADC A,(HL)   8 cycles   Z 0 H C
        {
            uint8_t mem = readByteMem(getHL());
            uint8_t carry = getFlag(FLAG_C);
            calc_flags(getA(), mem + carry, false);
            setA(getA() + mem + carry);
            addClockCounter(8);
            break;
        }
        case 0x8f://ADC A,A   4 cycles   Z 0 H C
            op_adc_a_r8(getA());
            break;
        case 0x90://SUB B   4 cycles   Z 1 H C
            op_sub_a_r8(getB());
            break;
        case 0x91://SUB C   4 cycles   Z 1 H C
            op_sub_a_r8(getC());
            break;
        case 0x92://SUB D   4 cycles   Z 1 H C
            op_sub_a_r8(getD());
            break;
        case 0x93://SUB E   4 cycles   Z 1 H C
            op_sub_a_r8(getE());
            break;
        case 0x94://SUB H   4 cycles   Z 1 H C
            op_sub_a_r8(getH());
            break;
        case 0x95://SUB L   4 cycles   Z 1 H C
            op_sub_a_r8(getL());
            break;
        case 0x96://SUB (HL)   8 cycles   Z 1 H C
        {
            uint8_t mem = readByteMem(getHL());
            calc_flags(getA(), mem, true);
            setA(getA() - mem);
            addClockCounter(8);
            break;
        }
        case 0x97://SUB A   4 cycles   Z 1 H C
            op_sub_a_r8(getA());
            break;
        case 0x98://SBC A,B   4 cycles   Z 1 H C
            op_sbc_a_r8(getB());
            break;
        case 0x99://SBC A,C   4 cycles   Z 1 H C
            op_sbc_a_r8(getC());
            break;
        case 0x9a://SBC A,D   4 cycles   Z 1 H C
            op_sbc_a_r8(getD());
            break;
        case 0x9b://SBC A,E   4 cycles   Z 1 H C
            op_sbc_a_r8(getE());
            break;
        case 0x9c://SBC A,H   4 cycles   Z 1 H C
            op_sbc_a_r8(getH());
            break;
        case 0x9d://SBC A,L   4 cycles   Z 1 H C
            op_sbc_a_r8(getL());
            break;
        case 0x9e://SBC A,(HL)   8 cycles   Z 1 H C
        {
            uint8_t mem = readByteMem(getHL());
            uint8_t carry = getFlag(FLAG_C);
            calc_flags(getA(), mem - carry, true);
            setA(getA() - mem - carry);
            addClockCounter(8);
            break;
        }
        case 0x9f://SBC A,A   4 cycles   Z 1 H C
            op_sbc_a_r8(getA());
            break;
        case 0xa0://AND B   4 cycles   Z 0 1 0
            op_and_a_r8(getB());
            break;
        case 0xa1://AND C   4 cycles   Z 0 1 0
            op_and_a_r8(getC());
            break;
        case 0xa2://AND D   4 cycles   Z 0 1 0
            op_and_a_r8(getD());
            break;
        case 0xa3://AND E   4 cycles   Z 0 1 0
            op_and_a_r8(getE());
            break;
        case 0xa4://AND H   4 cycles   Z 0 1 0
            op_and_a_r8(getH());
            break;
        case 0xa5://AND L   4 cycles   Z 0 1 0
            op_and_a_r8(getL());
            break;
        case 0xa6://AND (HL)   8 cycles   Z 0 1 0
        {
            uint8_t mem = readByteMem(getHL());
            resetFlag(FLAG_N | FLAG_C);
            setFlag(FLAG_H);
            setA(getA() & mem);
            setFlagCond(FLAG_Z, getA() == 0);
            addClockCounter(8);
            break;
        }
        case 0xa7://AND A   4 cycles   Z 0 1 0
            op_and_a_r8(getA());
            break;
        case 0xa8://XOR B   4 cycles   Z 0 0 0
            op_xor_a_r8(getB());
            break;
        case 0xa9://XOR C   4 cycles   Z 0 0 0
            op_xor_a_r8(getC());
            break;
        case 0xaa://XOR D   4 cycles   Z 0 0 0
            op_xor_a_r8(getD());
            break;
        case 0xab://XOR E   4 cycles   Z 0 0 0
            op_xor_a_r8(getE());
            break;
        case 0xac://XOR H   4 cycles   Z 0 0 0
            op_xor_a_r8(getH());
            break;
        case 0xad://XOR L   4 cycles   Z 0 0 0
            op_xor_a_r8(getL());
            break;
        case 0xae://XOR (HL)   8 cycles   Z 0 0 0
        {
            uint8_t mem = readByteMem(getHL());
            resetFlag(FLAG_N | FLAG_C | FLAG_H);
            setA(getA() ^ mem);
            setFlagCond(FLAG_Z, getA() == 0);
            addClockCounter(8);
            break;
        }
        case 0xaf://XOR A   4 cycles   Z 0 0 0
            op_xor_a_r8(getA());
            break;
        case 0xb0://OR B   4 cycles   Z 0 0 0
            op_or_a_r8(getB());
            break;
        case 0xb1://OR C   4 cycles   Z 0 0 0
            op_or_a_r8(getC());
            break;
        case 0xb2://OR D   4 cycles   Z 0 0 0
            op_or_a_r8(getD());
            break;
        case 0xb3://OR E   4 cycles   Z 0 0 0
            op_or_a_r8(getE());
            break;
        case 0xb4://OR H   4 cycles   Z 0 0 0
            op_or_a_r8(getH());
            break;
        case 0xb5://OR L   4 cycles   Z 0 0 0
            op_or_a_r8(getL());
            break;
        case 0xb6://OR (HL)   8 cycles   Z 0 0 0
        {
            uint8_t mem = readByteMem(getHL());
            resetFlag(FLAG_N | FLAG_C | FLAG_H);
            setA(getA() | mem);
            setFlagCond(FLAG_Z, getA() == 0);
            addClockCounter(8);
            break;
        }
        case 0xb7://OR A   4 cycles   Z 0 0 0
            op_or_a_r8(getA());
            break;
        case 0xb8://CP B   4 cycles   Z 1 H C
            op_cp_a_r8(getB());
            break;
        case 0xb9://CP C   4 cycles   Z 1 H C
            op_cp_a_r8(getC());
            break;
        case 0xba://CP D   4 cycles   Z 1 H C
            op_cp_a_r8(getD());
            break;
        case 0xbb://CP E   4 cycles   Z 1 H C
            op_cp_a_r8(getE());
            break;
        case 0xbc://CP H   4 cycles   Z 1 H C
            op_cp_a_r8(getH());
            break;
        case 0xbd://CP L   4 cycles   Z 1 H C
            op_cp_a_r8(getL());
            break;
        case 0xbe://CP (HL)   8 cycles   Z 1 H C
        {
            uint8_t mem = readByteMem(getHL());
            calc_flags(getA(), mem, true);
            addClockCounter(8);
            break;
        }
        case 0xbf://CP A   4 cycles   Z 1 H C
            op_cp_a_r8(getA());
            break;
        case 0xc0://RET NZ   20/8 cycles   - - - -
            op_ret_cond(getFlag(FLAG_Z) == 0);
            break;
        case 0xc1://POP BC   12 cycles   - - - -
            op_pop_r16(refBC());
            break;
        case 0xc2://JP NZ,a16   16/12 cycles   - - - -
            op_jp_cond_nn(getFlag(FLAG_Z) == 0);
            break;
        case 0xc3://JP a16   16 cycles   - - - -
        {
            uint16_t word = readWordMem(getCP());
            setCP(word);
            addClockCounter(16);
            break;
        }
        case 0xc4://CALL NZ,a16   24/12 cycles   - - - -
            op_call_cond_nn(getFlag(FLAG_Z) == 0);
            break;
        case 0xc5://PUSH BC   16 cycles   - - - -
            op_push_r16(getBC());
            break;
        case 0xc6://ADD A,d8   8 cycles   Z 0 H C
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            calc_flags(getA(), mem, false);
            setA(getA() + mem);
            addClockCounter(8);
            break;
        }
        case 0xc7://RST 00H   16 cycles   - - - -
            op_rst_n(0x00);
            break;
        case 0xc8://RET Z   20/8 cycles   - - - -
            op_ret_cond(getFlag(FLAG_Z));
            break;
        case 0xc9://RET   16 cycles   - - - -
        {
            uint16_t word = readWordMem(getSP());
            (*refSP())++;
            (*refSP())++;
            setCP(word);
            addClockCounter(16);
            break;
        }
        case 0xca://JP Z,a16   16/12 cycles   - - - -
            op_jp_cond_nn(getFlag(FLAG_Z));
            break;
        case 0xcb://PREFIX CB   4 cycles   - - - -
        {
            addClockCounter(4);
            uint8_t cbOpcode = readByteMem(getCP());
            incCP();
            executeCBInstruction(cbOpcode);
            break;
        }
        case 0xcc://CALL Z,a16   24/12 cycles   - - - -
            op_call_cond_nn(getFlag(FLAG_Z));
            break;
        case 0xcd://CALL a16   24 cycles   - - - -
        {
            (*refSP())--;
            (*refSP())--;
            uint16_t word = readWordMem(getCP());
            incCP();
            incCP();    // Next instruction is 2 bytes along.
            writeWordMem(getSP(), getCP()); // Save CP in stack
            setCP(word);
            addClockCounter(24);
            break;
        }
        case 0xce://ADC A,d8   8 cycles   Z 0 H C
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            uint8_t carry = getFlag(FLAG_C);
            calc_flags(getA(), mem + carry, false);
            setA(getA() + mem + carry);
            addClockCounter(8);
            break;
        }
        case 0xcf://RST 08H   16 cycles   - - - -
            op_rst_n(0x08);
            break;
        case 0xd0://RET NC   20/8 cycles   - - - -
            op_ret_cond(getFlag(FLAG_C) == 0);
            break;
        case 0xd1://POP DE   12 cycles   - - - -
            op_pop_r16(refDE());
            break;
        case 0xd2://JP NC,a16   16/12 cycles   - - - -
            op_jp_cond_nn(getFlag(FLAG_C) == 0);
            break;
        case 0xd3: // unused
            op_unused();
            break;
        case 0xd4://CALL NC,a16   24/12 cycles   - - - -
            op_call_cond_nn(getFlag(FLAG_C) == 0);
            break;
        case 0xd5://PUSH DE   16 cycles   - - - -
            op_push_r16(getDE());
            break;
        case 0xd6://SUB d8   8 cycles   Z 1 H C
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            calc_flags(getA(), mem, true);
            setA(getA() - mem);
            addClockCounter(8);
            break;
        }
        case 0xd7://RST 10H   16 cycles   - - - -
            op_rst_n(0x10);
            break;
        case 0xd8://RET C   20/8 cycles   - - - -
            op_ret_cond(getFlag(FLAG_C));
            break;
        case 0xd9://RETI   16 cycles   - - - -
            setCP(readWordMem(getSP()));
            (*refSP())++;
            (*refSP())++;   // Read word from SP => 2 inc.
            InterruptMasterEnable = true;
            addClockCounter(16);
            break;
        case 0xda://JP C,a16   16/12 cycles   - - - -
            op_jp_cond_nn(getFlag(FLAG_C));
            break;
        case 0xdb: // unused
            op_unused();
            break;
        case 0xdc://CALL C,a16   24/12 cycles   - - - -
            op_call_cond_nn(getFlag(FLAG_C));
            break;
        case 0xdd: // unused
            op_unused();
            break;
        case 0xde://SBC A,d8   8 cycles   Z 1 H C
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            uint8_t carry = getFlag(FLAG_C);
            calc_flags(getA(), mem - carry, true);
            setA(getA() - mem - carry);
            addClockCounter(8);
            break;
        }
        case 0xdf://RST 18H   16 cycles   - - - -
            op_rst_n(0x18);
            break;
        case 0xe0://LDH (0xFF00 + nn),A   12 cycles   - - - -
        {
            uint16_t mem = (uint16_t) 0xFF00 + readByteMem(getCP());
            incCP();
            writeByteMem(mem, getA());
            addClockCounter(12);
            break;
        }
        case 0xe1://POP HL   12 cycles   - - - -
            op_pop_r16(refHL());
            break;
        case 0xe2://LD (0xFF00 + C),A   8 cycles   - - - -
        {
            writeByteMem((uint16_t) 0xFF00 + getC(), getA());
            addClockCounter(8);
            break;
        }
        case 0xe3: // unused
            op_unused();
            break;
        case 0xe4: // unused
            op_unused();
            break;
        case 0xe5://PUSH HL   16 cycles   - - - -
            op_push_r16(getHL());
            break;
        case 0xe6://AND d8   8 cycles   Z 0 1 0
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            resetFlag(FLAG_N | FLAG_C);
            setFlag(FLAG_H);
            setA(getA() & mem);
            setFlagCond(FLAG_Z, getA() == 0);
            addClockCounter(8);
            break;
        }
        case 0xe7://RST 20H   16 cycles   - - - -
            op_rst_n(0x20);
            break;
        case 0xe8://ADD SP,r8   16 cycles   0 0 H C
        {
            int8_t mem = readByteMem(getCP());  // Signed
            incCP();
            int16_t result = getSP() + mem;

            setFlagCond(FLAG_C, (result & 0x100) == 1);
            setFlagCond(FLAG_H, (result & 0x08) == 1);
            resetFlag(FLAG_N);
            resetFlag(FLAG_Z);

            setSP((uint16_t) result);
            addClockCounter(16);
            break;
        }
        case 0xe9://JP HL   4 cycles   - - - -
        {
            setCP(getHL());
            addClockCounter(4);
            break;
        }
        case 0xea://LD (a16),A   16 cycles   - - - -
        {
            uint16_t val = readWordMem(getCP());
            incCP();
            incCP();
            writeByteMem(val, getA());
            addClockCounter(16);
            break;
        }
        case 0xeb: // unused
            op_unused();
            break;
        case 0xec: // unused
            op_unused();
            break;
        case 0xed: // unused
            op_unused();
            break;
        case 0xee://XOR d8   8 cycles   Z 0 0 0
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            resetFlag(FLAG_N | FLAG_C | FLAG_H);
            setA(getA() ^ mem);
            setFlagCond(FLAG_Z, getA() == 0);
            addClockCounter(8);
            break;
        }
        case 0xef://RST 28H   16 cycles   - - - -
            op_rst_n(0x28);
            break;
        case 0xf0://LDH A,(0xFF00 + n)   12 cycles   - - - -
        {
            uint8_t n = readByteMem(getCP());
            incCP();
            setA(readByteMem((uint16_t) (0xFF00 + n)));
            addClockCounter(12);
            break;
        }
        case 0xf1://POP AF   12 cycles   Z N H C
            op_pop_r16(refAF());
            break;
        case 0xf2://LD A,(0xFF00 + C)   8 cycles   - - - -
            setA(readByteMem((uint16_t) (0xFF00 + getC())));
            addClockCounter(8);
            break;
        case 0xf3://DI   4 cycles   - - - -
            EIExecuted = false;
            InterruptMasterEnable = false;
            addClockCounter(4);
            break;
        case 0xf4: // unused
            op_unused();
            break;
        case 0xf5://PUSH AF   16 cycles   - - - -
            op_push_r16(getAF());
            break;
        case 0xf6://OR d8   8 cycles   Z 0 0 0
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            resetFlag(FLAG_N | FLAG_C | FLAG_H);
            setA(getA() | mem);
            setFlagCond(FLAG_Z, getA() == 0);
            addClockCounter(4);
            break;
        }
        case 0xf7://RST 30H   16 cycles   - - - -
            op_rst_n(0x30);
            break;
        case 0xf8://LD HL,SP+r8   12 cycles   0 0 H C
        {
            int8_t r8 = readByteMem(getCP());
            incCP();
            int16_t result = getSP() + r8;
            setHL((uint16_t) result);
            resetFlag(FLAG_Z | FLAG_N);

            setFlagCond(FLAG_C, (result & 0x100) == 1);
            setFlagCond(FLAG_H, (result & 0x08) == 1);
            addClockCounter(12);
            break;
        }
        case 0xf9://LD SP,HL   8 cycles   - - - -
            setSP(getHL());
            addClockCounter(8);
            break;
        case 0xfa://LD A,(a16)   16 cycles   - - - -
        {
            uint16_t mem = readWordMem(getCP());
            incCP();
            incCP();
            setA(readByteMem(mem));
            addClockCounter(16);
            break;
        }
        case 0xfb://EI   4 cycles   - - - -
            EIExecuted = true;
            addClockCounter(4);
            break;
        case 0xfc: // unused
            op_unused();
            break;
        case 0xfd: // unused
            op_unused();
            break;
        case 0xfe://CP d8   8 cycles   Z 1 H C
        {
            uint8_t mem = readByteMem(getCP());
            incCP();
            calc_flags(getA(),mem, true);
            addClockCounter(8);
            break;
        }
        case 0xff://RST 38H   16 cycles   - - - -
            op_rst_n(0x38);
            break;
        default:
            printf("NOT POSSIBLE STAY HERE");
            break;
    }
}

/**
 * Execute CB opcodes.
 * @param bcopcode CB opcode to be executed.
 */
void Z80::executeCBInstruction(uint8_t cbopcode) {
    switch (cbopcode) {
        case 0x0://RLC B   8 cycles   Z 0 0 C
            op_rlc_r(refB());
            break;
        case 0x1://RLC C   8 cycles   Z 0 0 C
            op_rlc_r(refC());
            break;
        case 0x2://RLC D   8 cycles   Z 0 0 C
            op_rlc_r(refD());
            break;
        case 0x3://RLC E   8 cycles   Z 0 0 C
            op_rlc_r(refE());
            break;
        case 0x4://RLC H   8 cycles   Z 0 0 C
            op_rlc_r(refH());
            break;
        case 0x5://RLC L   8 cycles   Z 0 0 C
            op_rlc_r(refL());
            break;
        case 0x6://RLC (HL)   16 cycles   Z 0 0 C
        {
            uint8_t mem = readByteMem(getHL());
            setFlagCond(FLAG_C, (mem & 0x80) != 0);
            uint8_t result = mem << 1 | getFlag(FLAG_C);
            resetFlag(FLAG_H | FLAG_N);
            writeByteMem(getHL(), result);
            setFlagCond(FLAG_Z, result == 0);
            addClockCounter(16);
            break;
        }
        case 0x7://RLC A   8 cycles   Z 0 0 C
            op_rlc_r(refA());
            break;
        case 0x8://RRC B   8 cycles   Z 0 0 C
            op_rrc_r(refB());
            break;
        case 0x9://RRC C   8 cycles   Z 0 0 C
            op_rrc_r(refC());
            break;
        case 0xa://RRC D   8 cycles   Z 0 0 C
            op_rrc_r(refD());
            break;
        case 0xb://RRC E   8 cycles   Z 0 0 C
            op_rrc_r(refE());
            break;
        case 0xc://RRC H   8 cycles   Z 0 0 C
            op_rrc_r(refH());
            break;
        case 0xd://RRC L   8 cycles   Z 0 0 C
            op_rrc_r(refL());
            break;
        case 0xe://RRC (HL)   16 cycles   Z 0 0 C
        {
            uint8_t mem = readByteMem(getHL());
            setFlagCond(FLAG_C, (mem & 0x01) != 0);
            uint8_t result = getFlag(FLAG_C) << 7| mem >> 1;
            resetFlag(FLAG_H | FLAG_N);
            writeByteMem(getHL(), result);
            setFlagCond(FLAG_Z, result == 0);
            addClockCounter(16);
            break;
        }
        case 0xf://RRC A   8 cycles   Z 0 0 C
            op_rrc_r(refA());
            break;
        case 0x10://RL B   8 cycles   Z 0 0 C
            op_rl_r(refB());
            break;
        case 0x11://RL C   8 cycles   Z 0 0 C
            op_rl_r(refC());
            break;
        case 0x12://RL D   8 cycles   Z 0 0 C
            op_rl_r(refD());
            break;
        case 0x13://RL E   8 cycles   Z 0 0 C
            op_rl_r(refE());
            break;
        case 0x14://RL H   8 cycles   Z 0 0 C
            op_rl_r(refH());
            break;
        case 0x15://RL L   8 cycles   Z 0 0 C
            op_rl_r(refL());
            break;
        case 0x16://RL (HL)   16 cycles   Z 0 0 C
        {
            uint8_t mem = readByteMem(getHL());
            uint8_t carry = getFlag(FLAG_C);
            uint8_t result = (mem << 1) | carry;
            setFlagCond(FLAG_C, (mem & 0x80) != 0);
            writeByteMem(getHL(), result);
            resetFlag(FLAG_H | FLAG_N);
            setFlagCond(FLAG_Z, result == 0);
            addClockCounter(16);
            break;
        }
        case 0x17://RL A   8 cycles   Z 0 0 C
            op_rl_r(refA());
            break;
        case 0x18://RR B   8 cycles   Z 0 0 C
            op_rr_r(refB());
            break;
        case 0x19://RR C   8 cycles   Z 0 0 C
            op_rr_r(refC());
            break;
        case 0x1a://RR D   8 cycles   Z 0 0 C
            op_rr_r(refD());
            break;
        case 0x1b://RR E   8 cycles   Z 0 0 C
            op_rr_r(refE());
            break;
        case 0x1c://RR H   8 cycles   Z 0 0 C
            op_rr_r(refH());
            break;
        case 0x1d://RR L   8 cycles   Z 0 0 C
            op_rr_r(refL());
            break;
        case 0x1e://RR (HL)   16 cycles   Z 0 0 C
        {
            uint8_t mem = readByteMem(getHL());
            uint8_t carry = getFlag(FLAG_C);
            uint8_t result = (carry << 7) | (mem >> 1);
            setFlagCond(FLAG_C, (mem & 0x01) != 0);
            writeByteMem(getHL(), result);
            resetFlag(FLAG_H | FLAG_N);
            setFlagCond(FLAG_Z, result == 0);
            addClockCounter(16);
            break;
        }
        case 0x1f://RR A   8 cycles   Z 0 0 C
            op_rr_r(refA());
            break;
        case 0x20://SLA B   8 cycles   Z 0 0 C
            op_sla_r(refB());
            break;
        case 0x21://SLA C   8 cycles   Z 0 0 C
            op_sla_r(refC());
            break;
        case 0x22://SLA D   8 cycles   Z 0 0 C
            op_sla_r(refD());
            break;
        case 0x23://SLA E   8 cycles   Z 0 0 C
            op_sla_r(refE());
            break;
        case 0x24://SLA H   8 cycles   Z 0 0 C
            op_sla_r(refH());
            break;
        case 0x25://SLA L   8 cycles   Z 0 0 C
            op_sla_r(refL());
            break;
        case 0x26://SLA (HL)   16 cycles   Z 0 0 C
        {
            uint8_t mem = readByteMem(getHL());
            setFlagCond(FLAG_C, (mem & 0x80) != 0);
            resetFlag(FLAG_N | FLAG_H);
            mem <<= 1;
            writeByteMem(getHL(), mem);
            setFlagCond(FLAG_Z, mem == 0);
            addClockCounter(16);
            break;
        }
        case 0x27://SLA A   8 cycles   Z 0 0 C
            op_sla_r(refA());
            break;
        case 0x28://SRA B   8 cycles   Z 0 0 0
            op_sra_r(refB());
            break;
        case 0x29://SRA C   8 cycles   Z 0 0 0
            op_sra_r(refC());
            break;
        case 0x2a://SRA D   8 cycles   Z 0 0 0
            op_sra_r(refD());
            break;
        case 0x2b://SRA E   8 cycles   Z 0 0 0
            op_sra_r(refE());
            break;
        case 0x2c://SRA H   8 cycles   Z 0 0 0
            op_sra_r(refH());
            break;
        case 0x2d://SRA L   8 cycles   Z 0 0 0
            op_sra_r(refL());
            break;
        case 0x2e://SRA (HL)   16 cycles   Z 0 0 0
        {
            uint8_t mem = readByteMem(getHL());
            uint8_t b7 = (uint8_t) (mem & 0x80);
            setFlagCond(FLAG_C, (mem & 0x01) != 0);
            resetFlag(FLAG_N | FLAG_H);
            mem = (b7 << 7) | mem >> 1;
            writeByteMem(getHL(), mem);
            setFlagCond(FLAG_Z, mem == 0);
            addClockCounter(16);
            break;
        }
        case 0x2f://SRA A   8 cycles   Z 0 0 0
            op_sra_r(refA());
            break;
        case 0x30://SWAP B   8 cycles   Z 0 0 0
            op_swap_r(refB());
            break;
        case 0x31://SWAP C   8 cycles   Z 0 0 0
            op_swap_r(refC());
            break;
        case 0x32://SWAP D   8 cycles   Z 0 0 0
            op_swap_r(refD());
            break;
        case 0x33://SWAP E   8 cycles   Z 0 0 0
            op_swap_r(refE());
            break;
        case 0x34://SWAP H   8 cycles   Z 0 0 0
            op_swap_r(refH());
            break;
        case 0x35://SWAP L   8 cycles   Z 0 0 0
            op_swap_r(refL());
            break;
        case 0x36://SWAP (HL)   16 cycles   Z 0 0 0
        {
            uint8_t mem = readByteMem(getHL());
            resetFlag(FLAG_C | FLAG_N | FLAG_H);
            writeByteMem(getHL(), (mem << 4) | (mem >> 4));
            setFlagCond(FLAG_Z, mem == 0);
            addClockCounter(16);
            break;
        }
        case 0x37://SWAP A   8 cycles   Z 0 0 0
            op_swap_r(refA());
            break;
        case 0x38://SRL B   8 cycles   Z 0 0 C
            op_srl_r(refB());
            break;
        case 0x39://SRL C   8 cycles   Z 0 0 C
            op_srl_r(refC());
            break;
        case 0x3a://SRL D   8 cycles   Z 0 0 C
            op_srl_r(refD());
            break;
        case 0x3b://SRL E   8 cycles   Z 0 0 C
            op_srl_r(refE());
            break;
        case 0x3c://SRL H   8 cycles   Z 0 0 C
            op_srl_r(refH());
            break;
        case 0x3d://SRL L   8 cycles   Z 0 0 C
            op_srl_r(refL());
            break;
        case 0x3e://SRL (HL)   16 cycles   Z 0 0 C
        {
            uint8_t mem = readByteMem(getHL());
            setFlagCond(FLAG_C, (mem & 0x01) != 0);
            resetFlag(FLAG_N | FLAG_H);
            mem >>= 1;
            writeByteMem(getHL(), mem);
            setFlagCond(FLAG_Z, mem == 0);
            addClockCounter(16);
            break;
        }
        case 0x3f://SRL A   8 cycles   Z 0 0 C
            op_srl_r(refA());
            break;
        case 0x40://BIT 0,B   8 cycles   Z 0 1 -
            op_bit_n_r(0, getB());
            break;
        case 0x41://BIT 0,C   8 cycles   Z 0 1 -
            op_bit_n_r(0, getC());
            break;
        case 0x42://BIT 0,D   8 cycles   Z 0 1 -
            op_bit_n_r(0, getD());
            break;
        case 0x43://BIT 0,E   8 cycles   Z 0 1 -
            op_bit_n_r(0, getE());
            break;
        case 0x44://BIT 0,H   8 cycles   Z 0 1 -
            op_bit_n_r(0, getH());
            break;
        case 0x45://BIT 0,L   8 cycles   Z 0 1 -
            op_bit_n_r(0, getL());
            break;
        case 0x46://BIT 0,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(0);
            break;
        case 0x47://BIT 0,A   8 cycles   Z 0 1 -
            op_bit_n_r(0, getA());
            break;
        case 0x48://BIT 1,B   8 cycles   Z 0 1 -
            op_bit_n_r(1, getB());
            break;
        case 0x49://BIT 1,C   8 cycles   Z 0 1 -
            op_bit_n_r(1, getC());
            break;
        case 0x4a://BIT 1,D   8 cycles   Z 0 1 -
            op_bit_n_r(1, getD());
            break;
        case 0x4b://BIT 1,E   8 cycles   Z 0 1 -
            op_bit_n_r(1, getE());
            break;
        case 0x4c://BIT 1,H   8 cycles   Z 0 1 -
            op_bit_n_r(1, getH());
            break;
        case 0x4d://BIT 1,L   8 cycles   Z 0 1 -
            op_bit_n_r(1, getL());
            break;
        case 0x4e://BIT 1,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(1);
            break;
        case 0x4f://BIT 1,A   8 cycles   Z 0 1 -
            op_bit_n_r(1, getA());
            break;
        case 0x50://BIT 2,B   8 cycles   Z 0 1 -
            op_bit_n_r(2, getB());
            break;
        case 0x51://BIT 2,C   8 cycles   Z 0 1 -
            op_bit_n_r(2, getC());
            break;
        case 0x52://BIT 2,D   8 cycles   Z 0 1 -
            op_bit_n_r(2, getD());
            break;
        case 0x53://BIT 2,E   8 cycles   Z 0 1 -
            op_bit_n_r(2, getE());
            break;
        case 0x54://BIT 2,H   8 cycles   Z 0 1 -
            op_bit_n_r(2, getH());
            break;
        case 0x55://BIT 2,L   8 cycles   Z 0 1 -
            op_bit_n_r(2, getL());
            break;
        case 0x56://BIT 2,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(2);
            break;
        case 0x57://BIT 2,A   8 cycles   Z 0 1 -
            op_bit_n_r(2, getA());
            break;
        case 0x58://BIT 3,B   8 cycles   Z 0 1 -
            op_bit_n_r(3, getB());
            break;
        case 0x59://BIT 3,C   8 cycles   Z 0 1 -
            op_bit_n_r(3, getC());
            break;
        case 0x5a://BIT 3,D   8 cycles   Z 0 1 -
            op_bit_n_r(3, getD());
            break;
        case 0x5b://BIT 3,E   8 cycles   Z 0 1 -
            op_bit_n_r(3, getE());
            break;
        case 0x5c://BIT 3,H   8 cycles   Z 0 1 -
            op_bit_n_r(3, getH());
            break;
        case 0x5d://BIT 3,L   8 cycles   Z 0 1 -
            op_bit_n_r(3, getL());
            break;
        case 0x5e://BIT 3,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(3);
            break;
        case 0x5f://BIT 3,A   8 cycles   Z 0 1 -
            op_bit_n_r(3, getA());
            break;
        case 0x60://BIT 4,B   8 cycles   Z 0 1 -
            op_bit_n_r(4, getB());
            break;
        case 0x61://BIT 4,C   8 cycles   Z 0 1 -
            op_bit_n_r(4, getC());
            break;
        case 0x62://BIT 4,D   8 cycles   Z 0 1 -
            op_bit_n_r(4, getD());
            break;
        case 0x63://BIT 4,E   8 cycles   Z 0 1 -
            op_bit_n_r(4, getE());
            break;
        case 0x64://BIT 4,H   8 cycles   Z 0 1 -
            op_bit_n_r(4, getH());
            break;
        case 0x65://BIT 4,L   8 cycles   Z 0 1 -
            op_bit_n_r(4, getL());
            break;
        case 0x66://BIT 4,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(4);
            break;
        case 0x67://BIT 4,A   8 cycles   Z 0 1 -
            op_bit_n_r(4, getA());
            break;
        case 0x68://BIT 5,B   8 cycles   Z 0 1 -
            op_bit_n_r(5, getB());
            break;
        case 0x69://BIT 5,C   8 cycles   Z 0 1 -
            op_bit_n_r(5, getC());
            break;
        case 0x6a://BIT 5,D   8 cycles   Z 0 1 -
            op_bit_n_r(5, getD());
            break;
        case 0x6b://BIT 5,E   8 cycles   Z 0 1 -
            op_bit_n_r(5, getE());
            break;
        case 0x6c://BIT 5,H   8 cycles   Z 0 1 -
            op_bit_n_r(5, getH());
            break;
        case 0x6d://BIT 5,L   8 cycles   Z 0 1 -
            op_bit_n_r(5, getL());
            break;
        case 0x6e://BIT 5,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(5);
            break;
        case 0x6f://BIT 5,A   8 cycles   Z 0 1 -
            op_bit_n_r(5, getA());
            break;
        case 0x70://BIT 6,B   8 cycles   Z 0 1 -
            op_bit_n_r(6, getB());
            break;
        case 0x71://BIT 6,C   8 cycles   Z 0 1 -
            op_bit_n_r(6, getC());
            break;
        case 0x72://BIT 6,D   8 cycles   Z 0 1 -
            op_bit_n_r(6, getD());
            break;
        case 0x73://BIT 6,E   8 cycles   Z 0 1 -
            op_bit_n_r(6, getE());
            break;
        case 0x74://BIT 6,H   8 cycles   Z 0 1 -
            op_bit_n_r(6, getH());
            break;
        case 0x75://BIT 6,L   8 cycles   Z 0 1 -
            op_bit_n_r(6, getL());
            break;
        case 0x76://BIT 6,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(6);
            break;
        case 0x77://BIT 6,A   8 cycles   Z 0 1 -
            op_bit_n_r(6, getA());
            break;
        case 0x78://BIT 7,B   8 cycles   Z 0 1 -
            op_bit_n_r(7, getB());
            break;
        case 0x79://BIT 7,C   8 cycles   Z 0 1 -
            op_bit_n_r(7, getC());
            break;
        case 0x7a://BIT 7,D   8 cycles   Z 0 1 -
            op_bit_n_r(7, getD());
            break;
        case 0x7b://BIT 7,E   8 cycles   Z 0 1 -
            op_bit_n_r(7, getE());
            break;
        case 0x7c://BIT 7,H   8 cycles   Z 0 1 -
            op_bit_n_r(7, getH());
            break;
        case 0x7d://BIT 7,L   8 cycles   Z 0 1 -
            op_bit_n_r(7, getL());
            break;
        case 0x7e://BIT 7,(HL)   16 cycles   Z 0 1 -
            op_bit_n_ptr_HL(7);
            break;
        case 0x7f://BIT 7,A   8 cycles   Z 0 1 -
            op_bit_n_r(7, getA());
            break;
        case 0x80://RES 0,B   8 cycles   - - - -
            op_res_n_r(0, refB());
            break;
        case 0x81://RES 0,C   8 cycles   - - - -
            op_res_n_r(0, refC());
            break;
        case 0x82://RES 0,D   8 cycles   - - - -
            op_res_n_r(0, refD());
            break;
        case 0x83://RES 0,E   8 cycles   - - - -
            op_res_n_r(0, refE());
            break;
        case 0x84://RES 0,H   8 cycles   - - - -
            op_res_n_r(0, refH());
            break;
        case 0x85://RES 0,L   8 cycles   - - - -
            op_res_n_r(0, refL());
            break;
        case 0x86://RES 0,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(0);
            break;
        case 0x87://RES 0,A   8 cycles   - - - -
            op_res_n_r(0, refA());
            break;
        case 0x88://RES 1,B   8 cycles   - - - -
            op_res_n_r(1, refB());
            break;
        case 0x89://RES 1,C   8 cycles   - - - -
            op_res_n_r(1, refC());
            break;
        case 0x8a://RES 1,D   8 cycles   - - - -
            op_res_n_r(1, refD());
            break;
        case 0x8b://RES 1,E   8 cycles   - - - -
            op_res_n_r(1, refE());
            break;
        case 0x8c://RES 1,H   8 cycles   - - - -
            op_res_n_r(1, refH());
            break;
        case 0x8d://RES 1,L   8 cycles   - - - -
            op_res_n_r(1, refL());
            break;
        case 0x8e://RES 1,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(1);
            break;
        case 0x8f://RES 1,A   8 cycles   - - - -
            op_res_n_r(1, refA());
            break;
        case 0x90://RES 2,B   8 cycles   - - - -
            op_res_n_r(2, refB());
            break;
        case 0x91://RES 2,C   8 cycles   - - - -
            op_res_n_r(2, refC());
            break;
        case 0x92://RES 2,D   8 cycles   - - - -
            op_res_n_r(2, refD());
            break;
        case 0x93://RES 2,E   8 cycles   - - - -
            op_res_n_r(2, refE());
            break;
        case 0x94://RES 2,H   8 cycles   - - - -
            op_res_n_r(2, refH());
            break;
        case 0x95://RES 2,L   8 cycles   - - - -
            op_res_n_r(2, refL());
            break;
        case 0x96://RES 2,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(2);
            break;
        case 0x97://RES 2,A   8 cycles   - - - -
            op_res_n_r(2, refA());
            break;
        case 0x98://RES 3,B   8 cycles   - - - -
            op_res_n_r(3, refB());
            break;
        case 0x99://RES 3,C   8 cycles   - - - -
            op_res_n_r(3, refC());
            break;
        case 0x9a://RES 3,D   8 cycles   - - - -
            op_res_n_r(3, refD());
            break;
        case 0x9b://RES 3,E   8 cycles   - - - -
            op_res_n_r(3, refE());
            break;
        case 0x9c://RES 3,H   8 cycles   - - - -
            op_res_n_r(3, refH());
            break;
        case 0x9d://RES 3,L   8 cycles   - - - -
            op_res_n_r(3, refL());
            break;
        case 0x9e://RES 3,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(3);
            break;
        case 0x9f://RES 3,A   8 cycles   - - - -
            op_res_n_r(3, refA());
            break;
        case 0xa0://RES 4,B   8 cycles   - - - -
            op_res_n_r(4, refB());
            break;
        case 0xa1://RES 4,C   8 cycles   - - - -
            op_res_n_r(4, refC());
            break;
        case 0xa2://RES 4,D   8 cycles   - - - -
            op_res_n_r(4, refD());
            break;
        case 0xa3://RES 4,E   8 cycles   - - - -
            op_res_n_r(4, refE());
            break;
        case 0xa4://RES 4,H   8 cycles   - - - -
            op_res_n_r(4, refH());
            break;
        case 0xa5://RES 4,L   8 cycles   - - - -
            op_res_n_r(4, refL());
            break;
        case 0xa6://RES 4,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(4);
            break;
        case 0xa7://RES 4,A   8 cycles   - - - -
            op_res_n_r(4, refA());
            break;
        case 0xa8://RES 5,B   8 cycles   - - - -
            op_res_n_r(5, refB());
            break;
        case 0xa9://RES 5,C   8 cycles   - - - -
            op_res_n_r(5, refC());
            break;
        case 0xaa://RES 5,D   8 cycles   - - - -
            op_res_n_r(5, refD());
            break;
        case 0xab://RES 5,E   8 cycles   - - - -
            op_res_n_r(5, refE());
            break;
        case 0xac://RES 5,H   8 cycles   - - - -
            op_res_n_r(5, refH());
            break;
        case 0xad://RES 5,L   8 cycles   - - - -
            op_res_n_r(5, refL());
            break;
        case 0xae://RES 5,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(5);
            break;
        case 0xaf://RES 5,A   8 cycles   - - - -
            op_res_n_r(5, refA());
            break;
        case 0xb0://RES 6,B   8 cycles   - - - -
            op_res_n_r(6, refB());
            break;
        case 0xb1://RES 6,C   8 cycles   - - - -
            op_res_n_r(6, refC());
            break;
        case 0xb2://RES 6,D   8 cycles   - - - -
            op_res_n_r(6, refD());
            break;
        case 0xb3://RES 6,E   8 cycles   - - - -
            op_res_n_r(6, refE());
            break;
        case 0xb4://RES 6,H   8 cycles   - - - -
            op_res_n_r(6, refH());
            break;
        case 0xb5://RES 6,L   8 cycles   - - - -
            op_res_n_r(6, refL());
            break;
        case 0xb6://RES 6,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(6);
            break;
        case 0xb7://RES 6,A   8 cycles   - - - -
            op_res_n_r(6, refA());
            break;
        case 0xb8://RES 7,B   8 cycles   - - - -
            op_res_n_r(7, refB());
            break;
        case 0xb9://RES 7,C   8 cycles   - - - -
            op_res_n_r(7, refC());
            break;
        case 0xba://RES 7,D   8 cycles   - - - -
            op_res_n_r(7, refD());
            break;
        case 0xbb://RES 7,E   8 cycles   - - - -
            op_res_n_r(7, refE());
            break;
        case 0xbc://RES 7,H   8 cycles   - - - -
            op_res_n_r(7, refH());
            break;
        case 0xbd://RES 7,L   8 cycles   - - - -
            op_res_n_r(7, refL());
            break;
        case 0xbe://RES 7,(HL)   16 cycles   - - - -
            op_res_n_ptr_HL(7);
            break;
        case 0xbf://RES 7,A   8 cycles   - - - -
            op_res_n_r(7, refA());
            break;
        case 0xc0://SET 0,B   8 cycles   - - - -
            op_set_n_r(0, refB());
            break;
        case 0xc1://SET 0,C   8 cycles   - - - -
            op_set_n_r(0, refC());
            break;
        case 0xc2://SET 0,D   8 cycles   - - - -
            op_set_n_r(0, refD());
            break;
        case 0xc3://SET 0,E   8 cycles   - - - -
            op_set_n_r(0, refE());
            break;
        case 0xc4://SET 0,H   8 cycles   - - - -
            op_set_n_r(0, refH());
            break;
        case 0xc5://SET 0,L   8 cycles   - - - -
            op_set_n_r(0, refL());
            break;
        case 0xc6://SET 0,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(0);
            break;
        case 0xc7://SET 0,A   8 cycles   - - - -
            op_set_n_r(0, refA());
            break;
        case 0xc8://SET 1,B   8 cycles   - - - -
            op_set_n_r(1, refB());
            break;
        case 0xc9://SET 1,C   8 cycles   - - - -
            op_set_n_r(1, refC());
            break;
        case 0xca://SET 1,D   8 cycles   - - - -
            op_set_n_r(1, refD());
            break;
        case 0xcb://SET 1,E   8 cycles   - - - -
            op_set_n_r(1, refE());
            break;
        case 0xcc://SET 1,H   8 cycles   - - - -
            op_set_n_r(1, refH());
            break;
        case 0xcd://SET 1,L   8 cycles   - - - -
            op_set_n_r(1, refL());
            break;
        case 0xce://SET 1,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(1);
            break;
        case 0xcf://SET 1,A   8 cycles   - - - -
            op_set_n_r(1, refA());
            break;
        case 0xd0://SET 2,B   8 cycles   - - - -
            op_set_n_r(2, refB());
            break;
        case 0xd1://SET 2,C   8 cycles   - - - -
            op_set_n_r(2, refC());
            break;
        case 0xd2://SET 2,D   8 cycles   - - - -
            op_set_n_r(2, refD());
            break;
        case 0xd3://SET 2,E   8 cycles   - - - -
            op_set_n_r(2, refE());
            break;
        case 0xd4://SET 2,H   8 cycles   - - - -
            op_set_n_r(2, refH());
            break;
        case 0xd5://SET 2,L   8 cycles   - - - -
            op_set_n_r(2, refL());
            break;
        case 0xd6://SET 2,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(2);
            break;
        case 0xd7://SET 2,A   8 cycles   - - - -
            op_set_n_r(2, refA());
            break;
        case 0xd8://SET 3,B   8 cycles   - - - -
            op_set_n_r(3, refB());
            break;
        case 0xd9://SET 3,C   8 cycles   - - - -
            op_set_n_r(3, refC());
            break;
        case 0xda://SET 3,D   8 cycles   - - - -
            op_set_n_r(3, refD());
            break;
        case 0xdb://SET 3,E   8 cycles   - - - -
            op_set_n_r(3, refE());
            break;
        case 0xdc://SET 3,H   8 cycles   - - - -
            op_set_n_r(3, refH());
            break;
        case 0xdd://SET 3,L   8 cycles   - - - -
            op_set_n_r(3, refL());
            break;
        case 0xde://SET 3,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(3);
            break;
        case 0xdf://SET 3,A   8 cycles   - - - -
            op_set_n_r(3, refA());
            break;
        case 0xe0://SET 4,B   8 cycles   - - - -
            op_set_n_r(4, refB());
            break;
        case 0xe1://SET 4,C   8 cycles   - - - -
            op_set_n_r(4, refC());
            break;
        case 0xe2://SET 4,D   8 cycles   - - - -
            op_set_n_r(4, refD());
            break;
        case 0xe3://SET 4,E   8 cycles   - - - -
            op_set_n_r(4, refE());
            break;
        case 0xe4://SET 4,H   8 cycles   - - - -
            op_set_n_r(4, refH());
            break;
        case 0xe5://SET 4,L   8 cycles   - - - -
            op_set_n_r(4, refL());
            break;
        case 0xe6://SET 4,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(4);
            break;
        case 0xe7://SET 4,A   8 cycles   - - - -
            op_set_n_r(4, refA());
            break;
        case 0xe8://SET 5,B   8 cycles   - - - -
            op_set_n_r(5, refB());
            break;
        case 0xe9://SET 5,C   8 cycles   - - - -
            op_set_n_r(5, refC());
            break;
        case 0xea://SET 5,D   8 cycles   - - - -
            op_set_n_r(5, refD());
            break;
        case 0xeb://SET 5,E   8 cycles   - - - -
            op_set_n_r(5, refE());
            break;
        case 0xec://SET 5,H   8 cycles   - - - -
            op_set_n_r(5, refH());
            break;
        case 0xed://SET 5,L   8 cycles   - - - -
            op_set_n_r(5, refL());
            break;
        case 0xee://SET 5,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(5);
            break;
        case 0xef://SET 5,A   8 cycles   - - - -
            op_set_n_r(5, refA());
            break;
        case 0xf0://SET 6,B   8 cycles   - - - -
            op_set_n_r(6, refB());
            break;
        case 0xf1://SET 6,C   8 cycles   - - - -
            op_set_n_r(6, refC());
            break;
        case 0xf2://SET 6,D   8 cycles   - - - -
            op_set_n_r(6, refD());
            break;
        case 0xf3://SET 6,E   8 cycles   - - - -
            op_set_n_r(6, refE());
            break;
        case 0xf4://SET 6,H   8 cycles   - - - -
            op_set_n_r(6, refH());
            break;
        case 0xf5://SET 6,L   8 cycles   - - - -
            op_set_n_r(6, refL());
            break;
        case 0xf6://SET 6,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(6);
            break;
        case 0xf7://SET 6,A   8 cycles   - - - -
            op_set_n_r(6, refA());
            break;
        case 0xf8://SET 7,B   8 cycles   - - - -
            op_set_n_r(7, refB());
            break;
        case 0xf9://SET 7,C   8 cycles   - - - -
            op_set_n_r(7, refC());
            break;
        case 0xfa://SET 7,D   8 cycles   - - - -
            op_set_n_r(7, refD());
            break;
        case 0xfb://SET 7,E   8 cycles   - - - -
            op_set_n_r(7, refE());
            break;
        case 0xfc://SET 7,H   8 cycles   - - - -
            op_set_n_r(7, refH());
            break;
        case 0xfd://SET 7,L   8 cycles   - - - -
            op_set_n_r(7, refL());
            break;
        case 0xfe://SET 7,(HL)   16 cycles   - - - -
            op_set_n_ptr_HL(7);
            break;
        case 0xff://SET 7,A   8 cycles   - - - -
            op_set_n_r(7, refA());
            break;
        default:
            printf("NOT POSSIBLE STAY HERE");
            break;
    }
}

/**************************************************/
/*                                                */
/*                 Helper Methods                 */
/*                                                */
/**************************************************/

void Z80::op_inc_8(uint8_t *reg) {
    setFlagCond(FLAG_H, (*reg & 0xF) == 0xF);
    ++(*reg);
    setFlagCond(FLAG_Z, *reg == 0);
    resetFlag(FLAG_N);
    addClockCounter(4);
}

void Z80::op_inc_16(uint16_t *reg) {
    ++(*reg);
    addClockCounter(8);
}

void Z80::op_dec_8(uint8_t *reg) {
    setFlagCond(FLAG_H, (*reg & 0xF) == 0x0);
    --(*reg);
    setFlagCond(FLAG_Z, *reg == 0);
    setFlag(FLAG_N);
    addClockCounter(4);
}

void Z80::op_dec_16(uint16_t *reg) {
    --(*reg);
    addClockCounter(8);
}

void Z80::op_ld_r8_r8(uint8_t *dst, uint8_t src) {
    (*dst) = src;
    addClockCounter(4);
}

void Z80::op_ld_r8_d8(uint8_t *reg) {
    (*reg) = readByteMem(getCP());
    incCP();    // Read byte from memory -> 1 inc CP
    addClockCounter(8);
}

void Z80::op_ld_r8_ptr_r16(uint8_t *reg8, uint16_t reg16) {
    (*reg8) = readByteMem(reg16);
    addClockCounter(8);
}

void Z80::op_ld_r16_d16(uint16_t *reg) {
    (*reg) = readWordMem(getCP());
    incCP(); incCP();   // Read word from memory -> 2 inc CP.
    addClockCounter(12);
}

void Z80::op_ld_ptr_r16_r8(uint16_t reg16, uint8_t reg8) {
    writeByteMem(reg16, reg8);
    addClockCounter(8);
}

void Z80::calc_flags(uint8_t a, uint8_t b, bool sub) {
    uint16_t result = !sub ? a + b : a - b;
    uint8_t lb = (uint8_t) (!sub ? ((a & 0x0F) + (b & 0x0F)) : ((a & 0x0F) - (b & 0x0F)));
    setFlagCond(FLAG_C, (result & 0x100) != 0); // Number greater than unsigned 8 bits
    setFlagCond(FLAG_H, (lb & 0x10) != 0);
    setFlagCond(FLAG_N, sub);
    setFlagCond(FLAG_Z, ((uint8_t)result) == 0);
}

void Z80::op_add_hl_r16(uint16_t reg16) {
    resetFlag(FLAG_N);
    setFlagCond(FLAG_C, (getHL() + reg16) > 0xFFFF);
    setFlagCond(FLAG_H, ((getHL() & 0xFFF) + (reg16 & 0xFFF)) > 0xFFF);
    setHL(getHL() + reg16);
    addClockCounter(8);
}

void Z80::op_add_a_r8(uint8_t reg) {
    calc_flags(getA(), reg, false);
    setA(getA() + reg);
    addClockCounter(4);
}

void Z80::op_adc_a_r8(uint8_t reg) {
    uint8_t carry = getFlag(FLAG_C);
    calc_flags(getA(), reg + carry, false);
    setA(getA() + reg + carry);
    addClockCounter(4);
}

void Z80::op_sub_a_r8(uint8_t reg) {
    calc_flags(getA(), reg, true);
    setA(getA() - reg);
    addClockCounter(4);
}

void Z80::op_sbc_a_r8(uint8_t reg) {
    uint8_t carry = getFlag(FLAG_C);
    calc_flags(getA(), reg - carry, true);
    setA(getA() - reg - carry);
    addClockCounter(4);
}

void Z80::op_and_a_r8(uint8_t reg) {
    resetFlag(FLAG_N | FLAG_C);
    setFlag(FLAG_H);
    setA(getA() & reg);
    setFlagCond(FLAG_Z, getA() == 0);
    addClockCounter(4);
}

void Z80::op_xor_a_r8(uint8_t reg) {
    resetFlag(FLAG_N | FLAG_C | FLAG_H);
    setA(getA() ^ reg);
    setFlagCond(FLAG_Z, getA() == 0);
    addClockCounter(4);
}

void Z80::op_or_a_r8(uint8_t reg) {
    resetFlag(FLAG_N | FLAG_C | FLAG_H);
    setA(getA() | reg);
    setFlagCond(FLAG_Z, getA() == 0);
    addClockCounter(4);
}

void Z80::op_cp_a_r8(uint8_t reg) {
    calc_flags(getA(), reg, true);
    addClockCounter(4);
}

void Z80::op_ret_cond(bool cond) {
    if (cond) {
        setCP(readWordMem(getSP()));
        (*refSP())++;
        (*refSP())++;   // Read word from SP => 2 inc.
        addClockCounter(20);
    }else
        addClockCounter(8);
}

void Z80::op_pop_r16(uint16_t *reg) {
    (*reg) = readWordMem(getSP());
    (*refSP())++;
    (*refSP())++;   // Read word from SP => 2 inc.
    addClockCounter(12);
}

void Z80::op_push_r16(uint16_t reg) {
    (*refSP())--;
    (*refSP())--;
    writeWordMem(getSP(), reg);
    addClockCounter(16);
}

void Z80::op_jr_cond_nn(bool cond) {
    if (cond) {
        uint8_t r8 = readByteMem(getCP());
        incCP();
        setCP(getCP() + (int8_t)r8);    // This value is signed.
        addClockCounter(12);
    }
    else {
        incCP();
        addClockCounter(8);
    }
}

void Z80::op_jp_cond_nn(bool cond) {
    if (cond) {
        uint16_t word = readWordMem(getCP());
        setCP(word);
        addClockCounter(16);
    }
    else {
        incCP();
        incCP();
        addClockCounter(12);
    }
}

void Z80::op_call_cond_nn(bool cond) {
    if (cond) {
        (*refSP())--;
        (*refSP())--;
        uint16_t word = readWordMem(getCP());
        incCP();
        incCP();    // Next instruction is 2 bytes along.
        writeWordMem(getSP(), getCP()); // Save CP in stack
        setCP(word);
        addClockCounter(24);
    }else {
        incCP();
        incCP();
        addClockCounter(12);
    }
}

void Z80::op_rst_n(uint8_t address) {
    (*refSP())--;
    (*refSP())--;
    writeWordMem(getSP(), getCP());
    setCP(address);
    addClockCounter(16);
}


void Z80::op_rlc_r(uint8_t *reg) {
    setFlagCond(FLAG_C, (*reg & 0x80) != 0);
    resetFlag(FLAG_H | FLAG_N);
    (*reg) = (*reg << 1) | getFlag(FLAG_C);
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_rrc_r(uint8_t *reg) {
    setFlagCond(FLAG_C, (*reg & 0x01) != 0);
    resetFlag(FLAG_H | FLAG_N);
    (*reg) = getFlag(FLAG_C) << 7 | (*reg >> 1);
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_rl_r(uint8_t *reg) {
    uint8_t carry = getFlag(FLAG_C);
    setFlagCond(FLAG_C, (*reg & 0x80) != 0);
    *(reg) = (*reg << 1) | carry;
    resetFlag(FLAG_H | FLAG_N);
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_rr_r(uint8_t *reg) {
    uint8_t carry = getFlag(FLAG_C);
    setFlagCond(FLAG_C, (*reg & 0x01) != 0);
    *(reg) = (carry << 7) | (*reg >> 1);
    resetFlag(FLAG_H | FLAG_N);
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_sla_r(uint8_t *reg) {
    setFlagCond(FLAG_C, (*reg & 0x80) != 0);
    resetFlag(FLAG_N | FLAG_H);
    (*reg) <<= 1;
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_sra_r(uint8_t *reg) {
    uint8_t b7 = (uint8_t) ((*reg) & 0x80);
    setFlagCond(FLAG_C, (*reg & 0x01) != 0);
    resetFlag(FLAG_N | FLAG_H);
    (*reg) = b7 << 7| (*reg >> 1);
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_swap_r(uint8_t *reg) {
    resetFlag(FLAG_C | FLAG_N | FLAG_H);
    (*reg) = (*reg << 4) | (*reg >> 4);
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_srl_r(uint8_t *reg) {
    setFlagCond(FLAG_C, (*reg & 0x01) != 0);
    resetFlag(FLAG_N | FLAG_H);
    (*reg) >>= 1;
    setFlagCond(FLAG_Z, *reg == 0);
    addClockCounter(8);
}

void Z80::op_bit_n_r(uint8_t n, uint8_t reg) {
    resetFlag(FLAG_N);
    setFlag(FLAG_H);
    setFlagCond(FLAG_Z, (reg & (1 << n)) != 0);
    addClockCounter(8);
}

void Z80::op_bit_n_ptr_HL(uint8_t n) {
    resetFlag(FLAG_N);
    setFlag(FLAG_H);
    setFlagCond(FLAG_Z, (readByteMem(getHL()) & (1 << n)) != 0);
    addClockCounter(16);
}

void Z80::op_res_n_r(uint8_t n, uint8_t *reg) {
    (*reg) &= ~(1 << n);
    addClockCounter(8);
}

void Z80::op_res_n_ptr_HL(uint8_t n) {
    uint8_t mem = readByteMem(getHL());
    writeByteMem(getHL(), (uint8_t) (mem & (~(1 << n))));
    addClockCounter(16);
}

void Z80::op_set_n_r(uint8_t n, uint8_t *reg) {
    (*reg) |= (1 << n);
    addClockCounter(8);
}

void Z80::op_set_n_ptr_HL(uint8_t n) {
    uint8_t mem = readByteMem(getHL());
    writeByteMem(getHL(), (uint8_t) (mem | ((1 << n))));
    addClockCounter(16);
}

