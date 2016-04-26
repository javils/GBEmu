//
// Created by Javier Luque Sanabria on 19/4/16.
//

#ifndef GBEMU_GAMEBOY_H
#define GBEMU_GAMEBOY_H

#include <string>
#include "LCD.h"
#include "IOHandlerDMG.h"

class GameBoy {
public:
    static const int TICKS_PER_FRAME = 70221;

    void init(std::string filename);

    screen_t step();

    void KeyPressed(Input::Gameboy_Keys key);

    void KeyReleased(Input::Gameboy_Keys key);

    inline Z80 *getCPU() { return cpu.get(); }

private:
    unique_ptr<Cartridge> cart;
    unique_ptr<BasicMemory> mem;
    unique_ptr<Z80> cpu;
    unique_ptr<Timer> timer;
    unique_ptr<GPUDMG> gpuDMG;
    unique_ptr<Input> input;
    unique_ptr<IOHandlerDMG> ioHandler;
};

#endif //GBEMU_GAMEBOY_H
