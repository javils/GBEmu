//
// Created by Javier Luque Sanabria on 10/5/16.
//

#include <QTime>
#include "RenderThread.h"

Sound_Queue *sound = new Sound_Queue();

void updateSound(blip_sample_t *sample, int count) {
    sound->write(sample, count);
}

void RenderThread::Init(std::string fileName) {

    DMGPalette[0].setR(0xFF);
    DMGPalette[0].setG(0xFF);
    DMGPalette[0].setB(0xFF);
    DMGPalette[0].setA(0xFF);

    DMGPalette[1].setR(0xCC);
    DMGPalette[1].setG(0xCC);
    DMGPalette[1].setB(0xCC);
    DMGPalette[1].setA(0xFF);

    DMGPalette[2].setR(0x77);
    DMGPalette[2].setG(0x77);
    DMGPalette[2].setB(0x77);
    DMGPalette[2].setA(0xFF);

    DMGPalette[3].setR(0x00);
    DMGPalette[3].setG(0x00);
    DMGPalette[3].setB(0x00);
    DMGPalette[3].setA(0xFF);

    gameBoy = new GameBoy();
    gameBoy->init(fileName);
    finished = false;
    init = true;
    sound = new Sound_Queue();

    sound->start(44100, 2);

    resize = 1;
}

void RenderThread::run() {

    QTime timer;
    timer.start();
    double before = 0;
    double after = 0;
    double rest = 0;

    while (!finished) {
        before = timer.elapsed();
        RenderFrame(gameBoy->step(updateSound));
        after = timer.elapsed();

        double diff = after - before + rest;

        if (diff < double(1000.0f / 60.0f)) {
            QThread::msleep((Uint32) (17 - diff));
            rest = 0;
        }
        else {
            rest = diff - 17;
        }
    }

}

void RenderThread::KeyPressed(Input::Gameboy_Keys key) {
    mutex.lock();
    gameBoy->KeyPressed(key);
    mutex.unlock();
}

void RenderThread::KeyReleased(Input::Gameboy_Keys key) {
    mutex.lock();
    gameBoy->KeyReleased(key);
    mutex.unlock();
}

void RenderThread::saveGame() {
    mutex.lock();
    gameBoy->writeSave();
    mutex.unlock();
}

void RenderThread::RenderFrame(array<array<uint8_t, LCD::SCREEN_WIDTH>, LCD::SCREEN_HEIGHT> screen) {
    mutex.lock();

    QImage image(LCD::SCREEN_WIDTH, LCD::SCREEN_HEIGHT, QImage::Format_RGB888);

    for (uint8_t y = 0; y < LCD::SCREEN_HEIGHT; y++)
        for (uint8_t x = 0; x < LCD::SCREEN_WIDTH; x++) {
            LCDColor color = DMGPalette[screen[y][x]];
            image.setPixel(x, y, qRgb(color.getR(), color.getG(), color.getB()));
        }

    image = image.scaled(LCD::SCREEN_WIDTH * resize, LCD::SCREEN_HEIGHT * resize, Qt::KeepAspectRatio);

    QPixmap pixmap = QPixmap::fromImage(image);

    ui->label->setPixmap(pixmap);

    mutex.unlock();
}