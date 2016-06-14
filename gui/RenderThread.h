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
    RenderThread(Ui::MainWindow *ui) : ui(ui), init(false) { };

    void Init(std::string fileName);

    void run();

    void KeyPressed(Input::Gameboy_Keys key);

    void KeyReleased(Input::Gameboy_Keys key);

    void saveGame();

    void RenderFrame(array<array<uint8_t, LCD::SCREEN_WIDTH>, LCD::SCREEN_HEIGHT> screen);

    void finish() { finished = true; }

    bool isInit() { return init; }

    void setResize(uint8_t resize) { this->resize = resize; }

private:
    bool finished;
    bool init;
    GameBoy *gameBoy;
    QMutex mutex;

    uint8_t resize;

    Ui::MainWindow *ui;

    LCDColor DMGPalette[4];
};

#endif //GBEMU_RENDERTHREAD_H
