#ifndef GBEMU_INPUT_H
#define GBEMU_INPUT_H

#include <stdint.h>
#include "IOHandler.h"

class Input {
public:
    enum Gameboy_Keys {
        A_KEY = 4,
        B_KEY = 5,
        START_KEY = 7,
        SELECT_KEY = 6,
        RIGHT_KEY = 0,
        LEFT_KEY = 1,
        UP_KEY = 2,
        DOWN_KEY = 3,
        NONE_KEY = -1
    };

    inline void setIOHandler(IOHandler * ioHandler) { this->ioHandler = ioHandler; }
    void Init();
    void Reset();

    void KeyPressed(Gameboy_Keys key);

    void KeyReleased(Gameboy_Keys key);
    void update(uint16_t clockCycles);
    void Write(uint8_t value);
    uint8_t Read();
private:
    void step();

    IOHandler * ioHandler;
    uint8_t joypadState;
    uint8_t p1;
    uint32_t inputCycles;
};

inline void Input::update(uint16_t clockCycles)
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