//
// Created by Javier Luque Sanabria on 2/4/16.
//

#ifndef GBEMU_CARTRIDGE_H
#define GBEMU_CARTRIDGE_H

#include <string>
#include <fstream>
#include <vector>
#include <array>

using namespace std;

/**
 * This class represent a Cartridge of GameBoy
 */
class Cartridge {
public:
    enum CartrigdeType {
        CARTRIDGETYPE_NO_MBC,
        CARTRIDGETYPE_MBC1,
        CARTRIDGETYPE_MMM01,
        CARTRIDGETYPE_MBC2,
        CARTRIDGETYPE_MBC3,
        CARTRIDGETYPE_MBC5,
        CARTRIDGETYPE_NOT_SUPPORTED,
    };

    Cartridge();
    /**
     * Load a ROM from a path.
     * @param fileName path of the rom
     * @return true if the ROM is loaded, false in other case.
     */
    bool loadRom(std::string fileName);

    // Getters
    inline string getTitle() { return header->getTitle(); }
    inline uint8_t getGBMode() { return header->getGBMode(); }
    inline uint8_t getSGBMode() { return header->getSGBMode(); }
    inline CartrigdeType getCartridgeType() { return cartridgeType; }
    inline uint8_t getROMBanks() { return romBanks; }
    inline uint8_t getRAMBanks() { return ramBanks; }
    inline uint8_t getOldLicenseCode() { return header->getOldLicenseCode(); }
private:
    /**
     * Cartridge header
     */
    class CartridgeHeader {
    public:
        CartridgeHeader() {};
        CartridgeHeader(vector<uint8_t> ROM);

        /**
         * Fill the class with the metadata of the rom.
         */
        bool fillData();

        // Getters
        inline array<uint8_t, 0x04> getInitPoint() { return initPoint; }
        inline array<uint8_t, 0x30> getNintendoLogo() { return nintendoLogo; }
        inline string getTitle() { return title; }
        inline uint8_t getGBMode() { return gbmode; }
        inline array<uint8_t, 0x02> getNewLicenseCode() { return newLicenseCode; }
        inline uint8_t getSGBMode() { return sgbmode; }
        inline uint8_t getCartridgeType() { return cartrigdeType; }
        inline uint8_t getROMSize() { return romSize; }
        inline uint8_t getRAMSize() { return ramSize; }
        inline uint8_t getDestinationCode() { return destinationCode; }
        inline uint8_t getOldLicenseCode() { return oldLicenseCode; }
        inline uint8_t getMaskROMVersion() { return maskROMVersion; }
        inline uint8_t getChecksum() { return checksum; }
        inline uint16_t getGlobalChecksum() { return globalChecksum; }

    private:
        vector<uint8_t> ROM;
        array<uint8_t, 0x04> initPoint;         //< 0x0100 - 0x0103
        array<uint8_t, 0x30> nintendoLogo;      //< 0x0104 - 0x0133
        string title;                           //< 0x0134 - 0x0142
        uint8_t gbmode;                         //< 0x0143
        array<uint8_t, 0x02> newLicenseCode;    //< 0x0144 - 0x0145
        uint8_t sgbmode;                        //< 0x0146
        uint8_t cartrigdeType;                  //< 0x0147
        uint8_t romSize;                        //< 0x0148
        uint8_t ramSize;                        //< 0x0149
        uint8_t destinationCode;                //< 0x014A
        uint8_t oldLicenseCode;                 //< 0x014B
        uint8_t maskROMVersion;                 //< 0x014C
        uint8_t checksum;                       //< 0x014D
        uint16_t globalChecksum;                //< 0x014E - 0x014F

        /**
         * Get byte of the rom.
         * @param address address of the rom
         * @return the byte that correspond to the rom.
         */
        uint8_t getByte(uint16_t address);

        /**
         * Get a vector with all the bytes in the specifics range address
         * @param addressInit initial address to be loaded
         * @param addressEnd latest address to be loaded
         * @return vector with all the bytes read.
         */
        vector<uint8_t> getBytes(uint16_t addressInit, uint16_t addressEnd);

        /**
         * Check the checksum of the rom.
         * @return true if the checksum is correct, false in other case.
         */
        bool checkChecksum();
    };  // End CartridgeHeader

    vector<uint8_t> ROM;
    streampos size;
    unique_ptr<CartridgeHeader> header;
    CartrigdeType cartridgeType;
    uint8_t romBanks;
    uint8_t ramBanks;

    /**
     * Read the cartridge type of the header.
     * @return The cartridge type.
     * @see CartridgeType
     */
    CartrigdeType readCartridgeType();

    /**
     * Read the count ROM banks from the header.
     * @return true if the ROM size is correct, false in other case.
     */
    bool readROMBanks();

    /**
     * Read the count RAM banks from the header.
     * @return false if there are wrong value in the header ram size, true in other case.
     */
    bool readRAMBanks();

};

#endif //GBEMU_CARTRIDGE_H
