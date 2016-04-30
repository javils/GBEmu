//
// Created by Javier Luque Sanabria on 30/4/16.
//

#include <MBCs.h>

void MBC2::writeByte(uint16_t address, uint8_t value) {
    switch (address & 0xF000) {
        case 0x0000:
        case 0x1000:
            if (address & 0x0100) {
                memory->enableRAM((value & 0xF) == 0xA);
                if (memory->getNumRAMBanks() == 0)
                    memory->enableRAM(false);
            }
            break;
        case 0x2000:
        case 0x3000: {
            if (address & 0x0100) {
                uint16_t romBank = (uint16_t) (value & 0x0F);
                if (romBank == 0)
                    romBank++;
                memory->setSelectedROMBank(romBank);
            }
            break;
        }
        case 0xA000:
            if (address < 0xA200) {
                if (memory->isRAMEnabled())
                    memory->RAMBanks[memory->getSelectedRAMBank()][address - 0xA000] = (uint8_t) (value & 0x0F);
            }
            break;
        default:
            //TODO: Throw exception or something.
            break;
    }
}

uint8_t MBC2::readByte(uint16_t address) {
    if (memory->isRAMEnabled() && memory->getNumRAMBanks() > 0)
        return (uint8_t) (memory->RAMBanks[memory->getSelectedRAMBank()][address - 0xA000] | 0x0F);
    return 0xFF;
}