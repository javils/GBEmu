//
// Created by Javier Luque Sanabria on 23/3/16.
//

#ifndef GBEMU_MEMORY_H
#define GBEMU_MEMORY_H

/**
 * Abstract class used to implement memories.
 * This class have the basic methods that all memories need to have to work.
 */
class BasicMemory {
public:
    virtual void setByte(uint16_t address, uint8_t byte) = 0;
    virtual uint8_t getByte(uint16_t address) = 0;
    virtual void setWord(uint16_t address, uint16_t word) = 0;
    virtual uint16_t getWord(uint16_t address) = 0;
};
#endif //GBEMU_MEMORY_H
