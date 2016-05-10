//
// Created by Javier Luque Sanabria on 10/5/16.
//

#ifndef GBEMU_RENDERTHREAD_H
#define GBEMU_RENDERTHREAD_H

#include <QThread>
#include <QGraphicsView>
#include <Gameboy.h>
#include <ui_mainwindow.h>

using namespace std;

class RenderThread : public QThread {
public:
    RenderThread(Ui::MainWindow *ui) : ui(ui) { };

    void Init(std::string fileName);

    void run();

    void KeyPressed(Input::Gameboy_Keys key);

    void KeyReleased(Input::Gameboy_Keys key);

    void RenderFrame(array<array<uint8_t, LCD::SCREEN_WIDTH>, LCD::SCREEN_HEIGHT> screen);

    void finish() { finished = true; }

private:
    bool finished;
    GameBoy *gameBoy;
    QMutex mutex;

    Ui::MainWindow *ui;

    LCDColor DMGPalette[4];
};

#endif //GBEMU_RENDERTHREAD_H
