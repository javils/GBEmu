//
// Created by Javier Luque Sanabria on 23/3/16.
//

#ifndef GBEMU_REGISTERS_H_H
#define GBEMU_REGISTERS_H_H

#include <stdint.h>

/**
 * Type that represent a register of 16 bits.
 *
 * Can be used like one value 16 bits or like two value of 8 bits that represent the high and the low part
 * of 16 bits register.
 */
typedef union{
    uint16_t val;   //< Word value
    // Z80 is little-endian
    struct {
        uint8_t l;  //< Byte low
        uint8_t h;  //< Byte high
    } bytes;
} reg_t;

#endif //GBEMU_REGISTERS_H_H
