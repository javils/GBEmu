//
// Created by Javier Luque Sanabria on 31/3/16.
//

#include <MBCs.h>
#include <IOHandler.h>
#include "MemoryDMG.h"

MemoryDMG::MemoryDMG(Cartridge *cartridge, IOHandlerDMG *ioHandler) {
    ROM = cartridge->getROM();
    setNumRAMBanks(cartridge->getRAMBanks());
    setNumROMBanks(cartridge->getROMBanks());
    cartrigdeType = cartridge->getCartridgeType();

    this->ioHandler = move(ioHandler);

    init(cartrigdeType);
}

void MemoryDMG::init(Cartridge::CartrigdeType cartrigdeType) {

    //< Init bank 0 ROM.
    copy_n(ROM.begin(), 0x4000, ROMBase.begin());

    //< Init switchable ROM banks. The first bank is in 0x4000
    if ((getNumROMBanks() - 1) > 0) {
        ROMBanks.resize((uint8_t) (getNumROMBanks() - 1));
        for (uint8_t i = 1; i < getNumROMBanks(); i++)
            copy_n((ROM.begin() + 0x4000 * i), 0x4000, ROMBanks[i - 1].begin());
    }

    //< Init video ram.
    VideoRAM.fill(0x00);

    //< Init RAM banks
    RAMBanks.resize(getNumRAMBanks());
    for(uint8_t i = 0; i < getNumRAMBanks(); i++)
        generate(RAMBanks[i].begin(), RAMBanks[i].end(), fillRandom());

    //< Init WorkRAM and copy into EchoRam
    generate(WorkRAM.begin(), WorkRAM.end(), fillRandom());
    copy_n(WorkRAM.begin(), 0x1E00, EchoRam.begin());

    //< Init OAM Ram
    generate(OAMRam.begin(), OAMRam.end(), fillRandom());

    //< Init HRAM
    HRAM.fill(0x00);

    IERegister = 0x00;

    setSelectedROMBank(0);
    setSelectedRAMBank(0);
    enableRAM(false);

    selectMBC(cartrigdeType);
}

void MemoryDMG::selectMBC(Cartridge::CartrigdeType cartridgeType) {
    switch(cartridgeType) {
        case Cartridge::CartrigdeType::CARTRIDGETYPE_NO_MBC:
        {
            mbc.reset(new NOMBC(this));
            break;
        }
        case Cartridge::CartrigdeType::CARTRIDGETYPE_MBC1:
        {
            mbc.reset(new MBC1(this));
            break;
        }
        case Cartridge::CartrigdeType::CARTRIDGETYPE_MBC2:
            break;
        case Cartridge::CartrigdeType::CARTRIDGETYPE_MBC3:
            break;
        case Cartridge::CartrigdeType::CARTRIDGETYPE_MBC5:
            break;
        case Cartridge::CartrigdeType::CARTRIDGETYPE_MMM01:
            break;
        default:
            //TODO: Throw exception?
            break;
    }
};

void MemoryDMG::setByte(uint16_t address, uint8_t value) {
    switch(address & 0xF000) {
        case 0x0000:    // ROM
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            mbc->writeByte(address, value);
            break;
        case 0x8000:    // VIDEO
        case 0x9000:
            VideoRAM[address - 0x8000] = value;
            break;
        case 0xA000:    // External RAM
        case 0xB000:
            mbc->writeByte(address, value);
            break;
        case 0xC000:    // Work RAM
        case 0xD000:
            WorkRAM[address - 0xC000] = value;
            if (address < 0xDE00)   //TODO: Do that of call setByte function recursively?
                EchoRam[address - 0xC000] = value;
            break;
        case 0xE000:    // Echo RAM
            EchoRam[address - 0xE000] = value;
            WorkRAM[address - 0xE000] = value;
            break;
        case 0xF000:
            if (address < 0xFE00) {
                EchoRam[address - 0xE000] = value;
                WorkRAM[address - 0xE000] = value;
            } else if (address < 0xFEA0) {
                OAMRam[address - 0xFE00] = value;   // OAM RAM
            } else if (address > 0xFEFF && address < 0xFF4C) {
                ioHandler->writeIOReg((IOHandler::IOREGS) address, value);  // IO REGs
            } else if (address > 0xFF7F && address < 0xFFFF) {
                HRAM[address - 0xFF80] = value;
            } else if (address == 0xFFFF) {
                IERegister = value; //IE Register
            }
            else {
                /**
                 * TODO: Add Exception here
                 * If we are here the game use the Not Usable memory so
                 * printf or throw something
                 */
            }
            break;
        default:
            // Imposible stay here, throw exception or something
            break;
    }
};

void MemoryDMG::setWord(uint16_t address, uint16_t value) {
    setByte(address++, (uint8_t) (value & 0xFF));
    setByte(address, (uint8_t) (value >> 8));
};

uint8_t MemoryDMG::getByte(uint16_t address) {

    switch (address & 0xF000) {
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
            return ROMBase[address];
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return ROMBanks[getSelectedROMBank() - 1][address - 0x4000];
        case 0x8000:
        case 0x9000:
            return VideoRAM[address - 0x8000];
        case 0xA000:
        case 0xB000:
            return mbc->readByte(address);
        case 0xC000:
        case 0xD000:
            return WorkRAM[address - 0xC000];
        case 0xE000:
           return WorkRAM[address - 0xE000];
        case 0xF000:
            if (address < 0xFE00)
                return WorkRAM[address - 0xE000];
            else if (address < 0xFEA0)
                return OAMRam[address - 0xFE00];
            else if (address > 0xFEFF && address < 0xFF4C)
                return ioHandler->readIOReg((IOHandler::IOREGS) address);
            else if (address > 0xFF7F && address < 0xFFFF)
                return HRAM[address - 0xFF80];
            else if (address == 0xFFFF)
                return IERegister;
            else {
                /**
                 * TODO: Add Exception here
                 * If we are here the game use the Not Usable memory so
                 * printf or throw something
                 */
            }
            break;
        default:
            // Imposible stay here, throw exception or something
            break;
    }

    // Imposible stay here, throw exception or something
    return 0;
};

uint16_t MemoryDMG::getWord(uint16_t address) {
    uint8_t l = getByte(address);
    uint8_t h = getByte(++address);

    return h << 8 | l;
}