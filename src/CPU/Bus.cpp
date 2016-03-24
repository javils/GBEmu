//
// Created by Javier Luque Sanabria on 24/3/16.
//

#include "Bus.h"

void Bus::connectToMemory(unique_ptr<BasicMemory> mem) {
    this->mem = move(mem);
}

uint8_t Bus::receiveByte(uint16_t address) {
    return mem->getByte(address);
}

uint16_t Bus::receiveWord(uint16_t address) {
    return mem->getWord(address);
}

void Bus::sendByte(uint16_t address, uint8_t value) {
    mem->setByte(address, value);
}

void Bus::sendWord(uint16_t address, uint16_t value) {
    mem->setWord(address, value);
}