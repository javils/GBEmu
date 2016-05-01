//
// Created by Javier Luque Sanabria on 24/3/16.
//

#ifndef GBEMU_BUS_H
#define GBEMU_BUS_H

#include <stdint.h>
#include <memory>
#include "Memory.h"

using namespace std;
/**
 * Class that simulates a microprocessor bus.
 */
class Bus {
public:
    /**
     * Connects the memory with the bus.
     * @param mem pointer to the memory you want to access.
     */
    void connectToMemory(unique_ptr<BasicMemory> mem);

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

    BasicMemory *getMem() { return mem.get(); }

private:
    unique_ptr<BasicMemory> mem;
};

#endif //GBEMU_BUS_H
