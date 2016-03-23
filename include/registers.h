//
// Created by Javier Luque Sanabria on 23/3/16.
//

#ifndef GBEMU_REGISTERS_H_H
#define GBEMU_REGISTERS_H_H

#include <stdint.h>

typedef union{
    uint16_t val;
    // Z80 is little-endian
    struct {
        uint8_t l, h;
    } bytes;
} reg_t;

#endif //GBEMU_REGISTERS_H_H
