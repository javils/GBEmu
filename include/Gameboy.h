//
// Created by Javier Luque Sanabria on 19/4/16.
//

#ifndef GBEMU_GAMEBOY_H
#define GBEMU_GAMEBOY_H

#include <string>
#include "LCD.h"
#include "IOHandlerDMG.h"
#include "Audio.h"

#include "../libs/blargg/Blip_Buffer.h"

class Timer;

class IOHandlerDMG;

class Audio;
class GameBoy {
public:
    static const int TICKS_PER_FRAME = 70221;

    void init(std::string filename);

    void writeSave();

    void loadSave();

    screen_t step(std::function<void(blip_sample_t *, int)> f);

    void KeyPressed(Input::Gameboy_Keys key);

    void KeyReleased(Input::Gameboy_Keys key);

    inline Z80 *getCPU() { return cpu.get(); }

    inline blip_sample_t *getSampleBuffer() { return audio->getSampleBuffer(); }

    inline long getCountSampleBuffer() { return audio->getCountSampleBuffer(); }

private:
    std::string filePath;
    unique_ptr<Cartridge> cart;
    unique_ptr<BasicMemory> mem;
    unique_ptr<Z80> cpu;
    unique_ptr<Timer> timer;
    unique_ptr<GPUDMG> gpuDMG;
    unique_ptr<Input> input;
    unique_ptr<Audio> audio;
    unique_ptr<IOHandlerDMG> ioHandler;
};

#endif //GBEMU_GAMEBOY_H
