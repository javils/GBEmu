//
// Created by Javier Luque Sanabria on 2/5/16.
//

#include <Audio.h>

void Audio::update(uint32_t clock, std::function<void(blip_sample_t *, int)> f) {
    audioClock += clock;
    time += clock;

    while (time >= 10000) {
        endFrame(f);
        time -= 10000;
        audioClock = 0;
    }
}

void Audio::endFrame(std::function<void(blip_sample_t *, int)> f) {
    gbApu->end_frame(audioClock);
    stereoBuffer->end_frame(audioClock);

    if (stereoBuffer->samples_avail() >= SAMPLE_BUFFER_SIZE) {
        count = stereoBuffer->read_samples(sampleBuffer, SAMPLE_BUFFER_SIZE);
        if (f != nullptr)
            f(sampleBuffer, (int) count);
    }
}