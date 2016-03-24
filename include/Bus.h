//
// Created by Javier Luque Sanabria on 24/3/16.
//

#ifndef GBEMU_BUS_H
#define GBEMU_BUS_H

#include <stdint.h>
#include "Memory.h"

/**
 * Class that simulates a microprocessor bus.
 */
class Bus {
public:
    /**
     * Connects the memory bus
     * @param mem pointer to the memory you want to access.
     */
    void connectToMemory(BasicMemory *mem);

    /**
     * Send byte to the memory.
     * @param address where want to write.
     * @param value value to write.
     */
    void sendByte(uint16_t address, uint8_t value);

    /**
     * Send word to the memory.
     * @param address where want to write.
     * @param value value to write.
     */
    void sendWord(uint16_t address, uint16_t value);

    /**
     * Receive byte from the memory.
     * @param address where want to read.
     * @return value read from memory.
     */
    uint8_t receiveByte(uint16_t address);

    /**
     * Receive word from the memory.
     * @param address where want to read.
     * @return value read from memory.
     */
    uint16_t receiveWord(uint16_t address);

private:
    BasicMemory *mem;
};

#endif //GBEMU_BUS_H
