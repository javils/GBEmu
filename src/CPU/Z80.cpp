//
// Created by Javier Luque Sanabria on 23/3/16.
//

#include <registers.h>
#include "Z80.h"


Z80::Z80(unique_ptr<BasicMemory> memory) {
    bus.reset(new Bus());   // Init pointer.
    bus->connectToMemory(move(memory)); // Connect bus with memory.
}

void Z80::executeNextOpcode() {
    uint8_t opcode = bus->receiveByte(getCP());
    executeOpcode(opcode);
}

void Z80::executeOpcode(uint8_t opcode) {
    switch (opcode) {
        case 0x0://NOP   4 cycles   - - - -
            addClockCounter(4);
            break;
        case 0x1://LD BC,d16   12 cycles   - - - -
            setBC(readWordMem(getCP()));
            addClockCounter(12);
            break;
        case 0x2://LD (BC),A   8 cycles   - - - -
            writeByteMem(getBC(), getA());
            addClockCounter(8);
            break;
        case 0x3://INC BC   8 cycles   - - - -
        {
            uint16_t bc = getBC();
            setBC(++bc);
            addClockCounter(8);
            break;
        }
        case 0x4://INC B   4 cycles   Z 0 H -
        {
            setFlagCond(FLAG_H, (getB() & 0xF) == 0xF);
            uint8_t b = getB();
            setB(++b);
            setFlagCond(FLAG_Z, getB() == 0);
            resetFlag(FLAG_N);
            addClockCounter(4);
            break;
        }
        case 0x5://DEC B   4 cycles   Z 1 H -
        {
            setFlagCond(FLAG_H, (getB() & 0xF) == 0xF);
            uint8_t b = getB();
            setB(--b);
            addClockCounter(4);
            break;
        }
        case 0x6://LD B,d8   8 cycles   - - - -
            setB(readByteMem(getCP()));
            addClockCounter(8);
            break;
        case 0x7://RLCA   4 cycles   0 0 0 C
            setFlagCond(FLAG_C, (getA() & 0x80) != 0);
            resetFlag(FLAG_Z | FLAG_H | FLAG_N);
            setA(getA() << 7 | getFlag(FLAG_C));
            addClockCounter(4);
            break;
        case 0x8://LD (a16),SP   20 cycles   - - - -
            writeWordMem(readWordMem(getCP()), getSP());
            addClockCounter(20);
            break;
        case 0x9://ADD HL,BC   8 cycles   - 0 H C
            resetFlag(FLAG_N);
            setFlagCond(FLAG_C, (getHL() + getBC()) > 0xFFFF);
            setFlagCond(FLAG_H, ((getHL() & 0xFFF) + (getBC() & 0xFFF)) > 0xFFF);
            setHL(getHL() + getBC());
            addClockCounter(8);
            break;
        case 0xa://LD A,(BC)   8 cycles   - - - -
            setA(readByteMem(getBC()));
            addClockCounter(8);
            break;
        case 0xb://DEC BC   8 cycles   - - - -
        {
            uint16_t bc = getBC();
            setBC(++bc);
            break;
        }
        case 0xc://INC C   4 cycles   Z 0 H -
            break;
        case 0xd://DEC C   4 cycles   Z 1 H -
            break;
        case 0xe://LD C,d8   8 cycles   - - - -
            break;
        case 0xf://RRCA   4 cycles   0 0 0 C
            break;
        case 0x10://STOP 0   4 cycles   - - - -
            break;
        case 0x11://LD DE,d16   12 cycles   - - - -
            break;
        case 0x12://LD (DE),A   8 cycles   - - - -
            break;
        case 0x13://INC DE   8 cycles   - - - -
            break;
        case 0x14://INC D   4 cycles   Z 0 H -
            break;
        case 0x15://DEC D   4 cycles   Z 1 H -
            break;
        case 0x16://LD D,d8   8 cycles   - - - -
            break;
        case 0x17://RLA   4 cycles   0 0 0 C
            break;
        case 0x18://JR r8   12 cycles   - - - -
            break;
        case 0x19://ADD HL,DE   8 cycles   - 0 H C
            break;
        case 0x1a://LD A,(DE)   8 cycles   - - - -
            break;
        case 0x1b://DEC DE   8 cycles   - - - -
            break;
        case 0x1c://INC E   4 cycles   Z 0 H -
            break;
        case 0x1d://DEC E   4 cycles   Z 1 H -
            break;
        case 0x1e://LD E,d8   8 cycles   - - - -
            break;
        case 0x1f://RRA   4 cycles   0 0 0 C
            break;
        case 0x20://JR NZ,r8   12/8 cycles   - - - -
            break;
        case 0x21://LD HL,d16   12 cycles   - - - -
            break;
        case 0x22://LD (HL+),A   8 cycles   - - - -
            break;
        case 0x23://INC HL   8 cycles   - - - -
            break;
        case 0x24://INC H   4 cycles   Z 0 H -
            break;
        case 0x25://DEC H   4 cycles   Z 1 H -
            break;
        case 0x26://LD H,d8   8 cycles   - - - -
            break;
        case 0x27://DAA   4 cycles   Z - 0 C
            break;
        case 0x28://JR Z,r8   12/8 cycles   - - - -
            break;
        case 0x29://ADD HL,HL   8 cycles   - 0 H C
            break;
        case 0x2a://LD A,(HL+)   8 cycles   - - - -
            break;
        case 0x2b://DEC HL   8 cycles   - - - -
            break;
        case 0x2c://INC L   4 cycles   Z 0 H -
            break;
        case 0x2d://DEC L   4 cycles   Z 1 H -
            break;
        case 0x2e://LD L,d8   8 cycles   - - - -
            break;
        case 0x2f://CPL   4 cycles   - 1 1 -
            break;
        case 0x30://JR NC,r8   12/8 cycles   - - - -
            break;
        case 0x31://LD SP,d16   12 cycles   - - - -
            break;
        case 0x32://LD (HL-),A   8 cycles   - - - -
            break;
        case 0x33://INC SP   8 cycles   - - - -
            break;
        case 0x34://INC (HL)   12 cycles   Z 0 H -
            break;
        case 0x35://DEC (HL)   12 cycles   Z 1 H -
            break;
        case 0x36://LD (HL),d8   12 cycles   - - - -
            break;
        case 0x37://SCF   4 cycles   - 0 0 1
            break;
        case 0x38://JR C,r8   12/8 cycles   - - - -
            break;
        case 0x39://ADD HL,SP   8 cycles   - 0 H C
            break;
        case 0x3a://LD A,(HL-)   8 cycles   - - - -
            break;
        case 0x3b://DEC SP   8 cycles   - - - -
            break;
        case 0x3c://INC A   4 cycles   Z 0 H -
            break;
        case 0x3d://DEC A   4 cycles   Z 1 H -
            break;
        case 0x3e://LD A,d8   8 cycles   - - - -
            break;
        case 0x3f://CCF   4 cycles   - 0 0 C
            break;
        case 0x40://LD B,B   4 cycles   - - - -
            break;
        case 0x41://LD B,C   4 cycles   - - - -
            break;
        case 0x42://LD B,D   4 cycles   - - - -
            break;
        case 0x43://LD B,E   4 cycles   - - - -
            break;
        case 0x44://LD B,H   4 cycles   - - - -
            break;
        case 0x45://LD B,L   4 cycles   - - - -
            break;
        case 0x46://LD B,(HL)   8 cycles   - - - -
            break;
        case 0x47://LD B,A   4 cycles   - - - -
            break;
        case 0x48://LD C,B   4 cycles   - - - -
            break;
        case 0x49://LD C,C   4 cycles   - - - -
            break;
        case 0x4a://LD C,D   4 cycles   - - - -
            break;
        case 0x4b://LD C,E   4 cycles   - - - -
            break;
        case 0x4c://LD C,H   4 cycles   - - - -
            break;
        case 0x4d://LD C,L   4 cycles   - - - -
            break;
        case 0x4e://LD C,(HL)   8 cycles   - - - -
            break;
        case 0x4f://LD C,A   4 cycles   - - - -
            break;
        case 0x50://LD D,B   4 cycles   - - - -
            break;
        case 0x51://LD D,C   4 cycles   - - - -
            break;
        case 0x52://LD D,D   4 cycles   - - - -
            break;
        case 0x53://LD D,E   4 cycles   - - - -
            break;
        case 0x54://LD D,H   4 cycles   - - - -
            break;
        case 0x55://LD D,L   4 cycles   - - - -
            break;
        case 0x56://LD D,(HL)   8 cycles   - - - -
            break;
        case 0x57://LD D,A   4 cycles   - - - -
            break;
        case 0x58://LD E,B   4 cycles   - - - -
            break;
        case 0x59://LD E,C   4 cycles   - - - -
            break;
        case 0x5a://LD E,D   4 cycles   - - - -
            break;
        case 0x5b://LD E,E   4 cycles   - - - -
            break;
        case 0x5c://LD E,H   4 cycles   - - - -
            break;
        case 0x5d://LD E,L   4 cycles   - - - -
            break;
        case 0x5e://LD E,(HL)   8 cycles   - - - -
            break;
        case 0x5f://LD E,A   4 cycles   - - - -
            break;
        case 0x60://LD H,B   4 cycles   - - - -
            break;
        case 0x61://LD H,C   4 cycles   - - - -
            break;
        case 0x62://LD H,D   4 cycles   - - - -
            break;
        case 0x63://LD H,E   4 cycles   - - - -
            break;
        case 0x64://LD H,H   4 cycles   - - - -
            break;
        case 0x65://LD H,L   4 cycles   - - - -
            break;
        case 0x66://LD H,(HL)   8 cycles   - - - -
            break;
        case 0x67://LD H,A   4 cycles   - - - -
            break;
        case 0x68://LD L,B   4 cycles   - - - -
            break;
        case 0x69://LD L,C   4 cycles   - - - -
            break;
        case 0x6a://LD L,D   4 cycles   - - - -
            break;
        case 0x6b://LD L,E   4 cycles   - - - -
            break;
        case 0x6c://LD L,H   4 cycles   - - - -
            break;
        case 0x6d://LD L,L   4 cycles   - - - -
            break;
        case 0x6e://LD L,(HL)   8 cycles   - - - -
            break;
        case 0x6f://LD L,A   4 cycles   - - - -
            break;
        case 0x70://LD (HL),B   8 cycles   - - - -
            break;
        case 0x71://LD (HL),C   8 cycles   - - - -
            break;
        case 0x72://LD (HL),D   8 cycles   - - - -
            break;
        case 0x73://LD (HL),E   8 cycles   - - - -
            break;
        case 0x74://LD (HL),H   8 cycles   - - - -
            break;
        case 0x75://LD (HL),L   8 cycles   - - - -
            break;
        case 0x76://HALT   4 cycles   - - - -
            break;
        case 0x77://LD (HL),A   8 cycles   - - - -
            break;
        case 0x78://LD A,B   4 cycles   - - - -
            break;
        case 0x79://LD A,C   4 cycles   - - - -
            break;
        case 0x7a://LD A,D   4 cycles   - - - -
            break;
        case 0x7b://LD A,E   4 cycles   - - - -
            break;
        case 0x7c://LD A,H   4 cycles   - - - -
            break;
        case 0x7d://LD A,L   4 cycles   - - - -
            break;
        case 0x7e://LD A,(HL)   8 cycles   - - - -
            break;
        case 0x7f://LD A,A   4 cycles   - - - -
            break;
        case 0x80://ADD A,B   4 cycles   Z 0 H C
            break;
        case 0x81://ADD A,C   4 cycles   Z 0 H C
            break;
        case 0x82://ADD A,D   4 cycles   Z 0 H C
            break;
        case 0x83://ADD A,E   4 cycles   Z 0 H C
            break;
        case 0x84://ADD A,H   4 cycles   Z 0 H C
            break;
        case 0x85://ADD A,L   4 cycles   Z 0 H C
            break;
        case 0x86://ADD A,(HL)   8 cycles   Z 0 H C
            break;
        case 0x87://ADD A,A   4 cycles   Z 0 H C
            break;
        case 0x88://ADC A,B   4 cycles   Z 0 H C
            break;
        case 0x89://ADC A,C   4 cycles   Z 0 H C
            break;
        case 0x8a://ADC A,D   4 cycles   Z 0 H C
            break;
        case 0x8b://ADC A,E   4 cycles   Z 0 H C
            break;
        case 0x8c://ADC A,H   4 cycles   Z 0 H C
            break;
        case 0x8d://ADC A,L   4 cycles   Z 0 H C
            break;
        case 0x8e://ADC A,(HL)   8 cycles   Z 0 H C
            break;
        case 0x8f://ADC A,A   4 cycles   Z 0 H C
            break;
        case 0x90://SUB B   4 cycles   Z 1 H C
            break;
        case 0x91://SUB C   4 cycles   Z 1 H C
            break;
        case 0x92://SUB D   4 cycles   Z 1 H C
            break;
        case 0x93://SUB E   4 cycles   Z 1 H C
            break;
        case 0x94://SUB H   4 cycles   Z 1 H C
            break;
        case 0x95://SUB L   4 cycles   Z 1 H C
            break;
        case 0x96://SUB (HL)   8 cycles   Z 1 H C
            break;
        case 0x97://SUB A   4 cycles   Z 1 H C
            break;
        case 0x98://SBC A,B   4 cycles   Z 1 H C
            break;
        case 0x99://SBC A,C   4 cycles   Z 1 H C
            break;
        case 0x9a://SBC A,D   4 cycles   Z 1 H C
            break;
        case 0x9b://SBC A,E   4 cycles   Z 1 H C
            break;
        case 0x9c://SBC A,H   4 cycles   Z 1 H C
            break;
        case 0x9d://SBC A,L   4 cycles   Z 1 H C
            break;
        case 0x9e://SBC A,(HL)   8 cycles   Z 1 H C
            break;
        case 0x9f://SBC A,A   4 cycles   Z 1 H C
            break;
        case 0xa0://AND B   4 cycles   Z 0 1 0
            break;
        case 0xa1://AND C   4 cycles   Z 0 1 0
            break;
        case 0xa2://AND D   4 cycles   Z 0 1 0
            break;
        case 0xa3://AND E   4 cycles   Z 0 1 0
            break;
        case 0xa4://AND H   4 cycles   Z 0 1 0
            break;
        case 0xa5://AND L   4 cycles   Z 0 1 0
            break;
        case 0xa6://AND (HL)   8 cycles   Z 0 1 0
            break;
        case 0xa7://AND A   4 cycles   Z 0 1 0
            break;
        case 0xa8://XOR B   4 cycles   Z 0 0 0
            break;
        case 0xa9://XOR C   4 cycles   Z 0 0 0
            break;
        case 0xaa://XOR D   4 cycles   Z 0 0 0
            break;
        case 0xab://XOR E   4 cycles   Z 0 0 0
            break;
        case 0xac://XOR H   4 cycles   Z 0 0 0
            break;
        case 0xad://XOR L   4 cycles   Z 0 0 0
            break;
        case 0xae://XOR (HL)   8 cycles   Z 0 0 0
            break;
        case 0xaf://XOR A   4 cycles   Z 0 0 0
            break;
        case 0xb0://OR B   4 cycles   Z 0 0 0
            break;
        case 0xb1://OR C   4 cycles   Z 0 0 0
            break;
        case 0xb2://OR D   4 cycles   Z 0 0 0
            break;
        case 0xb3://OR E   4 cycles   Z 0 0 0
            break;
        case 0xb4://OR H   4 cycles   Z 0 0 0
            break;
        case 0xb5://OR L   4 cycles   Z 0 0 0
            break;
        case 0xb6://OR (HL)   8 cycles   Z 0 0 0
            break;
        case 0xb7://OR A   4 cycles   Z 0 0 0
            break;
        case 0xb8://CP B   4 cycles   Z 1 H C
            break;
        case 0xb9://CP C   4 cycles   Z 1 H C
            break;
        case 0xba://CP D   4 cycles   Z 1 H C
            break;
        case 0xbb://CP E   4 cycles   Z 1 H C
            break;
        case 0xbc://CP H   4 cycles   Z 1 H C
            break;
        case 0xbd://CP L   4 cycles   Z 1 H C
            break;
        case 0xbe://CP (HL)   8 cycles   Z 1 H C
            break;
        case 0xbf://CP A   4 cycles   Z 1 H C
            break;
        case 0xc0://RET NZ   20/8 cycles   - - - -
            break;
        case 0xc1://POP BC   12 cycles   - - - -
            break;
        case 0xc2://JP NZ,a16   16/12 cycles   - - - -
            break;
        case 0xc3://JP a16   16 cycles   - - - -
            break;
        case 0xc4://CALL NZ,a16   24/12 cycles   - - - -
            break;
        case 0xc5://PUSH BC   16 cycles   - - - -
            break;
        case 0xc6://ADD A,d8   8 cycles   Z 0 H C
            break;
        case 0xc7://RST 00H   16 cycles   - - - -
            break;
        case 0xc8://RET Z   20/8 cycles   - - - -
            break;
        case 0xc9://RET   16 cycles   - - - -
            break;
        case 0xca://JP Z,a16   16/12 cycles   - - - -
            break;
        case 0xcb://PREFIX CB   4 cycles   - - - -
            break;
        case 0xcc://CALL Z,a16   24/12 cycles   - - - -
            break;
        case 0xcd://CALL a16   24 cycles   - - - -
            break;
        case 0xce://ADC A,d8   8 cycles   Z 0 H C
            break;
        case 0xcf://RST 08H   16 cycles   - - - -
            break;
        case 0xd0://RET NC   20/8 cycles   - - - -
            break;
        case 0xd1://POP DE   12 cycles   - - - -
            break;
        case 0xd2://JP NC,a16   16/12 cycles   - - - -
            break;
        case 0xd3: // unused
            break;
        case 0xd4://CALL NC,a16   24/12 cycles   - - - -
            break;
        case 0xd5://PUSH DE   16 cycles   - - - -
            break;
        case 0xd6://SUB d8   8 cycles   Z 1 H C
            break;
        case 0xd7://RST 10H   16 cycles   - - - -
            break;
        case 0xd8://RET C   20/8 cycles   - - - -
            break;
        case 0xd9://RETI   16 cycles   - - - -
            break;
        case 0xda://JP C,a16   16/12 cycles   - - - -
            break;
        case 0xdb: // unused
            break;
        case 0xdc://CALL C,a16   24/12 cycles   - - - -
            break;
        case 0xdd: // unused
            break;
        case 0xde://SBC A,d8   8 cycles   Z 1 H C
            break;
        case 0xdf://RST 18H   16 cycles   - - - -
            break;
        case 0xe0://LDH (a8),A   12 cycles   - - - -
            break;
        case 0xe1://POP HL   12 cycles   - - - -
            break;
        case 0xe2://LD (C),A   8 cycles   - - - -
            break;
        case 0xe3: // unused
            break;
        case 0xe4: // unused
            break;
        case 0xe5://PUSH HL   16 cycles   - - - -
            break;
        case 0xe6://AND d8   8 cycles   Z 0 1 0
            break;
        case 0xe7://RST 20H   16 cycles   - - - -
            break;
        case 0xe8://ADD SP,r8   16 cycles   0 0 H C
            break;
        case 0xe9://JP (HL)   4 cycles   - - - -
            break;
        case 0xea://LD (a16),A   16 cycles   - - - -
            break;
        case 0xeb: // unused
            break;
        case 0xec: // unused
            break;
        case 0xed: // unused
            break;
        case 0xee://XOR d8   8 cycles   Z 0 0 0
            break;
        case 0xef://RST 28H   16 cycles   - - - -
            break;
        case 0xf0://LDH A,(a8)   12 cycles   - - - -
            break;
        case 0xf1://POP AF   12 cycles   Z N H C
            break;
        case 0xf2://LD A,(C)   8 cycles   - - - -
            break;
        case 0xf3://DI   4 cycles   - - - -
            break;
        case 0xf4: // unused
            break;
        case 0xf5://PUSH AF   16 cycles   - - - -
            break;
        case 0xf6://OR d8   8 cycles   Z 0 0 0
            break;
        case 0xf7://RST 30H   16 cycles   - - - -
            break;
        case 0xf8://LD HL,SP+r8   12 cycles   0 0 H C
            break;
        case 0xf9://LD SP,HL   8 cycles   - - - -
            break;
        case 0xfa://LD A,(a16)   16 cycles   - - - -
            break;
        case 0xfb://EI   4 cycles   - - - -
            break;
        case 0xfc: // unused
            break;
        case 0xfd: // unused
            break;
        case 0xfe://CP d8   8 cycles   Z 1 H C
            break;
        case 0xff://RST 38H   16 cycles   - - - -
            break;
        default:
            printf("NOT POSSIBLE STAY HERE");
            break;
    }
}