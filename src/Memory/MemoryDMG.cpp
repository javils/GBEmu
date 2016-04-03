//
// Created by Javier Luque Sanabria on 31/3/16.
//

#include "MemoryDMG.h"

MemoryDMG::MemoryDMG(vector<uint8_t> ROM, uint8_t numROMBanks, uint8_t numRAMBanks){
    this->ROM = ROM;
    this->numROMBanks = numROMBanks;
    this->numRAMBanks = numRAMBanks;
}

auto MemoryDMG::fillRandom(){
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<uint8_t > dist(
            numeric_limits<uint8_t>::min(),
            numeric_limits<uint8_t>::max());

    return std::bind(dist,mt);
}

void MemoryDMG::init(Cartridge::CartrigdeType cartrigdeType) {

    //< Init bank 0 ROM.
    copy_n(ROM.begin(), 0x4000, ROMBase.begin());

    //< Init switchable ROM banks. The first bank is in 0x4000
    ROMBanks.resize((unsigned long) (numROMBanks - 1));
    for(uint8_t i = 1; i < numROMBanks; i++)
        copy_n((ROM.begin() + 0x4000 * i), 0x4000, ROMBanks[i - 1].begin());

    //< Init video ram.
    VideoRAM.fill(0x00);

    //< Init RAM banks
    RAMBanks.resize(numRAMBanks);
    for(uint8_t i = 0; i < numRAMBanks; i++)
        generate(RAMBanks[i].begin(), RAMBanks[i].end(), fillRandom());

    //< Init WorkRAM and copy into EchoRam
    generate(WorkRAM.begin(), WorkRAM.end(), fillRandom());
    copy_n(WorkRAM.begin(), 0x1E00, EchoRam.begin());

    //< Init OAM Ram
    generate(OAMRam.begin(), OAMRam.end(), fillRandom());

    //< Init IOPOrts and HRAM
    IOPorts.fill(0x00);
    HRAM.fill(0x00);

    //< Set specific values in IOPorts.
    setIOReg(TIMA, 0x00);
    setIOReg(TMA, 0x00);
    setIOReg(TAC, 0x00);

    setIOReg(NR_10, 0x80);
    setIOReg(NR_11, 0xBF);
    setIOReg(NR_12, 0xF3);
    setIOReg(NR_14, 0xBF);
    setIOReg(NR_21, 0x3F);
    setIOReg(NR_22, 0x00);
    setIOReg(NR_24, 0xBF);
    setIOReg(NR_30, 0x7F);
    setIOReg(NR_31, 0xFF);
    setIOReg(NR_32, 0x9F);
    setIOReg(NR_33, 0xBF);
    setIOReg(NR_41, 0xFF);
    setIOReg(NR_42, 0x00);
    setIOReg(NR_43, 0x00);
    setIOReg(NR_44, 0xBF);
    setIOReg(NR_50, 0x77);
    setIOReg(NR_51, 0xF3);
    setIOReg(NR_52, 0xF1);

    setIOReg(LCDC, 0x91);
    setIOReg(SCY, 0X00);
    setIOReg(SCX, 0x00);
    setIOReg(LYC, 0x00);
    setIOReg(BGP, 0xFC);
    setIOReg(OBP0, 0xFF);
    setIOReg(OBP1, 0xFF);
    setIOReg(WY, 0x00);
    setIOReg(WX, 0x00);
    IERegister = 0x00;

    selectedRAMBank = 0;
    selectedROMBank = 0;
    ramEnabled = false;
    //TODO: Select MBC here.
}

void MemoryDMG::setByte(uint16_t address, uint8_t value) {};
void MemoryDMG::setWord(uint16_t address, uint16_t value) {};

uint8_t MemoryDMG::getByte(uint16_t address) { return 0;};
uint16_t MemoryDMG::getWord(uint16_t address) { return 0; }