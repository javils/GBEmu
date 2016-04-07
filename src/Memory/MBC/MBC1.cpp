//
// Created by Javier Luque Sanabria on 7/4/16.
//
#include <MBC1.h>

void MBC1::writeByte(uint16_t address, uint8_t value) {
    switch (address & 0xF000) {
        case 0x0000:
        case 0x1000:
            memory->enableRAM((value & 0xF) == 0xA);
            if (memory->getNumRAMBanks() == 0)
                memory->enableRAM(false);
            break;
        case 0x2000:
        case 0x3000: {
            uint8_t actual = memory->getSelectedROMBank();
            uint8_t lower = (uint8_t) (value & 0x1F);
            actual &= ~0x1F;   //< Keep upper bites and clear lower bits.
            actual |= lower;
            //TODO: ¿Check if the ROM bank is greater than totalROMBank - 1?
            if (actual == 0 || actual == 0x20 || actual == 0x40 || actual == 0x60)
                actual++;
            memory->setSelectedROMBank(actual);
            break;
        }
        case 0x4000:
        case 0x5000:
        {
            value &= 0x03;
            if (memory->getMBCMode()) {   // RAM
                memory->setSelectedRAMBank(value);
                //TODO: ¿Check if the RAM bank is greater than totalRAMBank - 1?
            }
            else {  //ROM
                uint8_t actual = memory->getSelectedROMBank();
                actual &= 0x1F;   //< Keep lower bites and clear up bits.
                actual |= value << 5;
                //TODO: ¿Check if the ROM bank is greater than totalROMBank - 1?
                if (actual == 0 || actual == 0x20 || actual == 0x40 || actual == 0x60)
                    actual++;
                memory->setSelectedROMBank(actual);
            }
            break;
        }
        case 0x6000:
        case 0x7000:
            memory->setMBCMode((uint8_t) (value & 1));
            break;
        case 0xA000:
        case 0xB000:
            if (memory->isRAMEnabled())
                memory->RAMBanks[memory->getSelectedRAMBank()][address - 0xA000] = value;
            break;
        default:
            //TODO: Throw exception or something.
            break;
    }
}

uint8_t MBC1::readByte(uint16_t address) {
    if (memory->isRAMEnabled() && memory->getNumRAMBanks() > 0)
        return memory->RAMBanks[memory->getSelectedRAMBank()][address - 0xA000];
    return 0xFF;
}