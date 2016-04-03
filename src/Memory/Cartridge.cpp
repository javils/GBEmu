//
// Created by Javier Luque Sanabria on 2/4/16.
//

#include <iostream>
#include "Cartridge.h"

Cartridge::Cartridge(){
    header.reset(new CartridgeHeader);
}

bool Cartridge::loadRom(std::string fileName) {
    bool result = false;

    //< Open the file and put the cursor in the end of the file to get the size later.
    ifstream file (fileName, ifstream::in | ifstream::binary | ifstream::ate);

    if (file.is_open()) {
        size = file.tellg();
        file.seekg(0, ifstream::beg);
        ROM.resize((unsigned long) size);
        file.read(reinterpret_cast<char*> (ROM.data()), size);
        file.close();

        header.reset(new CartridgeHeader(ROM));
        if (header->fillData() && readCartridgeType() != CARTRIDGETYPE_NOT_SUPPORTED
                && readRAMBanks() && readROMBanks())
            result = true;
    }
    return result;
}

Cartridge::CartrigdeType Cartridge::readCartridgeType() {
    switch (header->getCartridgeType()) {
        case 0x00:  //< NO MBC
        case 0x08:  //< NO MBC + RAM
        case 0x09:  //< NO MBC + RAM + BATT
            cartridgeType = CARTRIDGETYPE_NO_MBC;
            break;
        case 0x01:  //< MBC1
        case 0x02:  //< MBC1 + RAM
        case 0x03:  //< MBC1 + RAM + BATT
        case 0xFF:  //< HuC-1
            cartridgeType = CARTRIDGETYPE_MBC1;
            break;
        case 0x05:  //< MBC2
        case 0x06:  //< MBC2 + BATT
            cartridgeType = CARTRIDGETYPE_MBC2;
            break;
        case 0x0B:  //< MMM01
        case 0x0C:  //< MMM01 + SRAM
        case 0x0D:  //< MMM01 + SRAM + BATT
            cartridgeType = CARTRIDGETYPE_MMM01;
            break;
        case 0x0F:  //< ROM + MBC3 + TIMER + BATT
        case 0x10:  //< ROM + MBC3 + TIMER + RAM + BATT
        case 0x11:  //< ROM + MBC3
        case 0x12:  //< ROM + MBC3 + RAM
        case 0x13:  //< ROM + MBC3 + RAM + BATT
            cartridgeType = CARTRIDGETYPE_MBC3;
            break;
        case 0x19:  //< ROM + MBC5
        case 0x1A:  //< ROM + MBC5 + RAM
        case 0x1B:  //< ROM + MBC5 + RAM + BATT
        case 0x1C:  //< ROM + MBC5 + RUMBLE
        case 0x1D:  //< ROM + MBC5 + RUMBLE + SRAM
        case 0x1E:  //< ROM + MBC5 + RUMBLE + SRAM + BATT
            cartridgeType = CARTRIDGETYPE_MBC5;
            break;
        default:
            cartridgeType = CARTRIDGETYPE_NOT_SUPPORTED;
            break;
    }

    return cartridgeType;
}

bool Cartridge::readRAMBanks() {
    bool result = true;
    switch (header->getRAMSize()) {
        case 0x0:
            ramBanks = 0;
            break;
        case 0x1:   //< 2KB
        case 0x2:   //< 8KB
            ramBanks = 1;
            break;
        case 0x3:   //< 4 * 8KB
            ramBanks = 4;
            break;
        case 0x4:   //< 16 * 8KB
            ramBanks = 16;
            break;
        default:
            result = false;
            break;
    }

    if (cartridgeType == CARTRIDGETYPE_MBC2)
        ramBanks = 1;

    return result;
}

bool Cartridge::readROMBanks() {
    bool result = true;
    romBanks = (uint8_t) (1 << (header->getROMSize() + 1));

    //< Rom size in bytes = banks * 16KB * 1024
    if ((romBanks * 16 * 1024) != size)
        result = false;

    return result;
}

/**
 * Cartridge header code
 */
Cartridge::CartridgeHeader::CartridgeHeader(vector<uint8_t> ROM) {
    this->ROM = ROM;
}

bool Cartridge::CartridgeHeader::fillData() {

    copy_n(getBytes(0x0100, 0x0103).begin(), 0x04, initPoint.begin());
    copy_n(getBytes(0x0104, 0x0133).begin(), 0x30, nintendoLogo.begin());
    copy_n(getBytes(0x0134, 0x0142).begin(), 0x14, title.begin());
    gbmode = getByte(0x0143);
    copy_n(getBytes(0x0144, 0x0145).begin(), 0x02, newLicenseCode.begin());
    sgbmode = getByte(0x0146);
    cartrigdeType = getByte(0x0147);
    romSize = getByte(0x0148);
    ramSize = getByte(0x0149);
    destinationCode = getByte(0x014A);
    oldLicenseCode = getByte(0x014B);
    maskROMVersion = getByte(0x014C);
    checksum = getByte(0x014D);
    globalChecksum = (getByte(0x014E) << 8) | getByte(0x014F);

    printf("%s", title.c_str());
    return checkChecksum();
}

uint8_t Cartridge::CartridgeHeader::getByte(uint16_t address) {
    return ROM.data()[address];
}

vector<uint8_t> Cartridge::CartridgeHeader::getBytes(uint16_t addressInit, uint16_t addressEnd) {
    uint16_t size = addressEnd - addressInit;
    vector<uint8_t> result(size);
    uint16_t index = 0;
    for (uint16_t i = addressInit; i < addressEnd; i++) {
        result[index] = getByte(i);
        index++;
    }

    return result;
}

bool Cartridge::CartridgeHeader::checkChecksum(){
    uint8_t sum = 0;
    for(uint16_t i = 0x0134; i <= 0x014C; i++) {
        sum = (uint8_t) (sum - getByte(i) - 1);
    }

    return sum == checksum;
}

