//
// Created by Javier Luque Sanabria on 8/5/16.
//

#ifndef GBEMU_MAINWINDOW_H
#define GBEMU_MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "RenderThread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    const int WINDOW_WIDTH_X1 = 160;
    const int WINDOW_HEIGHT_X1 = 144;

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void keyPressEvent(QKeyEvent *e);

    void keyReleaseEvent(QKeyEvent *e);

private slots:

    void on_actionLeer_ROM_triggered();

    void on_actionX1_triggered();

    void on_actionX2_triggered();

    void on_actionX4_triggered();

    void on_actionGuardar_triggered();

    void closeEvent(QCloseEvent *);


private:
    Ui::MainWindow *ui;
    uint8_t expand;

    RenderThread *renderThread;

    void centerMainWindow(uint8_t i);

    void resizeMainWindow(uint8_t i);

    bool eventFilter(QObject *obj,
                     QEvent *event);
};

#endif //GBEMU_MAINWINDOW_H
