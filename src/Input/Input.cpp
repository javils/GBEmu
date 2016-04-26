//
// Created by Javier Luque Sanabria on 22/4/16.
//

#include <Utils.h>
#include "Input.h"

void Input::Init()
{
    Reset();
}

void Input::Reset()
{
    joypadState = 0xFF;
    p1 = 0xCF;
    inputCycles = 0;
}

void Input::step()
{
    uint8_t current = (uint8_t) (p1 & 0xF0);

    switch (current & 0x30)
    {
        case 0x10:
        {
            uint8_t topJoypad = (uint8_t) ((joypadState >> 4) & 0x0F);
            current |= topJoypad;
            break;
        }
        case 0x20:
        {
            uint8_t bottomJoypad = (uint8_t) (joypadState & 0x0F);
            current |= bottomJoypad;
            break;
        }
        case 0x30:
            current |= 0x0F;
            break;
    }

    if ((p1 & ~current & 0x0F) != 0)
        ioHandler->getCPU()->requestInterrupt(JOYPAD_INT);

    p1 = current;
}

void Input::KeyPressed(Gameboy_Keys key) {
    joypadState = UnsetBit(joypadState, key);
}

void Input::KeyReleased(Gameboy_Keys key) {
    joypadState = SetBit(joypadState, key);
}