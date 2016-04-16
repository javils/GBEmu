//
// Created by Javier Luque Sanabria on 14/4/16.
//

#ifndef GBEMU_UTILS_H
#define GBEMU_UTILS_H

#include <stdint.h>

inline bool IsSetBit(const uint8_t value, const uint8_t bit)
{
    return (value & (0x01 << bit)) != 0;
}

inline uint8_t getBitVal(uint8_t value, uint8_t bit)
{
    return (uint8_t) ((value & bit) ? 1 : 0);
}

#endif //GBEMU_UTILS_H
