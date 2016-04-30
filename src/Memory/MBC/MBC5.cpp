//
// Created by Javier Luque Sanabria on 30/4/16.
//

#include <MBCs.h>

void MBC5::writeByte(uint16_t address, uint8_t value) {
    switch (address & 0xF000) {
        case 0x0000:
        case 0x1000:
            memory->enableRAM((value & 0xF) == 0xA);
            if (memory->getNumRAMBanks() == 0)
                memory->enableRAM(false);
            break;
        case 0x2000: {
            uint16_t actualROMBank = (uint16_t) (memory->getSelectedROMBank() & 0xFF00);
            actualROMBank |= (value & 0x00FF);
            memory->setSelectedROMBank(actualROMBank);
            break;
        }
        case 0x3000: {
            uint16_t actual = (uint16_t) (memory->getSelectedROMBank() & 0x00FF);
            actual |= (value << 8);
            memory->setSelectedROMBank(actual);
            break;
        }
        case 0x4000:
        case 0x5000: {
            memory->setSelectedRAMBank(value);
            break;
        }
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

uint8_t MBC5::readByte(uint16_t address) {
    if (memory->isRAMEnabled() && memory->getNumRAMBanks() > 0)
        return memory->RAMBanks[memory->getSelectedRAMBank()][address - 0xA000];
    return 0xFF;
}