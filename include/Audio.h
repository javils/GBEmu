//
// Created by Javier Luque Sanabria on 2/5/16.
//

#ifndef GBEMU_AUDIO_H
#define GBEMU_AUDIO_H

#include <IOHandler.h>
#include "../libs/blargg/Blip_Buffer.h"
#include "../libs/blargg/Gb_Apu.h"
#include "../libs/blargg/Multi_Buffer.h"
#include "../tests/SoundQueue/Sound_Queue.h"

class Audio {
public:
    static const uint16_t SAMPLE_BUFFER_SIZE = 2048;

    Audio() : audioClock(0), time(0), count(0), gbApu(new Gb_Apu()), stereoBuffer(new Stereo_Buffer()) {
        stereoBuffer->clock_rate(4194304);
        stereoBuffer->set_sample_rate(44100);
        gbApu->set_output(stereoBuffer->center(), stereoBuffer->left(), stereoBuffer->right());
    }

    inline void setIOHandler(IOHandler *ioHandler) { this->ioHandler = ioHandler; }

    void update(uint32_t clock, std::function<void(blip_sample_t *, int)> f);

    void endFrame(std::function<void(blip_sample_t *, int)> f);

    inline void writeAudio(uint16_t address, uint8_t value) { gbApu->write_register(audioClock, address, value); }

    inline uint8_t readAudio(uint16_t address) { return (uint8_t) gbApu->read_register(audioClock, address); }


    inline blip_sample_t *getSampleBuffer() { return sampleBuffer; }

    inline long getCountSampleBuffer() { return count; }

private:
    IOHandler *ioHandler;


    Gb_Apu *gbApu;
    Stereo_Buffer *stereoBuffer;
    blip_sample_t sampleBuffer[SAMPLE_BUFFER_SIZE];
    uint32_t audioClock;
    uint32_t time;

    long count;
};

#endif //GBEMU_AUDIO_H
