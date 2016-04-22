#ifndef GBEMU_INPUT_H
#define GBEMU_INPUT_H

#include <stdint.h>
#include "IOHandler.h"

class Input {
public:
    inline void setIOHandler(IOHandler * ioHandler) { this->ioHandler = ioHandler; }
    void Init();
    void Reset();
    void update(unsigned int clockCycles);
    void Write(uint8_t value);
    uint8_t Read();
private:
    void step();

    IOHandler * ioHandler;
    uint8_t joypadState;
    uint8_t p1;
    uint8_t inputCycles;
};

inline void Input::update(unsigned int clockCycles)
{
    inputCycles += clockCycles;

    // Joypad Poll Speed (64 Hz)
    if (inputCycles >= 65536)
    {
        inputCycles -= 65536;
        step();
    }
}

inline void Input::Write(uint8_t value)
{
    p1 = (uint8_t) ((p1 & 0xCF) | (value & 0x30));
    step();
}

inline uint8_t Input::Read()
{
    return p1;
}

#endif