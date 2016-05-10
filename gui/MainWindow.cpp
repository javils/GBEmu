//
// Created by Javier Luque Sanabria on 8/5/16.
//

#include <QGraphicsScene>
#include <QFileDialog>
#include <QDesktopWidget>
#include "ui_mainwindow.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);


    ui->label->installEventFilter(this);

    renderThread = new RenderThread(ui);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionLeer_ROM_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Leer ROM..."),
                                                    QDir::currentPath(), tr("Archivos ROM (*.gb)"));
    renderThread->Init(fileName.toStdString());
    renderThread->start(QThread::Priority::NormalPriority);
}

void MainWindow::on_actionX1_triggered() {
    ui->actionX1->setChecked(true);
    ui->actionX2->setChecked(false);
    ui->actionX4->setChecked(false);
    ui->actionX8->setChecked(false);

    resize(WINDOW_WIDTH_X1, WINDOW_HEIGHT_X1);
    ui->label->resize(WINDOW_WIDTH_X1, WINDOW_HEIGHT_X1);
    ui->centralWidget->resize(WINDOW_WIDTH_X1, WINDOW_HEIGHT_X1);

    resizeMainWindow(1);
    centerMainWindow(1);
}

void MainWindow::on_actionX2_triggered() {
    ui->actionX1->setChecked(false);
    ui->actionX2->setChecked(true);
    ui->actionX4->setChecked(false);
    ui->actionX8->setChecked(false);

    resize(WINDOW_WIDTH_X1 * 2, WINDOW_HEIGHT_X1 * 2);
    ui->label->resize(WINDOW_WIDTH_X1 * 2, WINDOW_HEIGHT_X1 * 2);
    ui->centralWidget->resize(WINDOW_WIDTH_X1 * 2, WINDOW_HEIGHT_X1 * 2);

    resizeMainWindow(2);
    centerMainWindow(2);
}

void MainWindow::on_actionX4_triggered() {
    ui->actionX1->setChecked(false);
    ui->actionX2->setChecked(false);
    ui->actionX4->setChecked(true);
    ui->actionX8->setChecked(false);

    resizeMainWindow(4);
    centerMainWindow(4);
}

void MainWindow::on_actionX8_triggered() {
    ui->actionX1->setChecked(false);
    ui->actionX2->setChecked(false);
    ui->actionX4->setChecked(false);
    ui->actionX8->setChecked(true);

    resizeMainWindow(8);
    centerMainWindow(8);
}

void MainWindow::centerMainWindow(uint8_t i) {
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - (WINDOW_WIDTH_X1 * i)) >> 1;
    int y = (screenGeometry.height() - (WINDOW_HEIGHT_X1 * i)) >> 1;
    move(x, y);
}

void MainWindow::resizeMainWindow(uint8_t i) {
    resize(WINDOW_WIDTH_X1 * i, WINDOW_HEIGHT_X1 * i);
    ui->label->resize(WINDOW_WIDTH_X1 * i, WINDOW_HEIGHT_X1 * i);
    ui->centralWidget->resize(WINDOW_WIDTH_X1 * i, WINDOW_HEIGHT_X1 * i);
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Up:
            renderThread->KeyPressed(Input::UP_KEY);
            break;
        case Qt::Key_Down:
            renderThread->KeyPressed(Input::DOWN_KEY);
            break;
        case Qt::Key_Left:
            renderThread->KeyPressed(Input::LEFT_KEY);
            break;
        case Qt::Key_Right:
            renderThread->KeyPressed(Input::RIGHT_KEY);
            break;
        case Qt::Key_A:
            renderThread->KeyPressed(Input::A_KEY);
            break;
        case Qt::Key_B:
            renderThread->KeyPressed(Input::B_KEY);
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            renderThread->KeyPressed(Input::START_KEY);
            break;
        case Qt::Key_Select:
            renderThread->KeyPressed(Input::SELECT_KEY);
            break;
        default:
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Up:
            renderThread->KeyReleased(Input::UP_KEY);
            break;
        case Qt::Key_Down:
            renderThread->KeyReleased(Input::DOWN_KEY);
            break;
        case Qt::Key_Left:
            renderThread->KeyReleased(Input::LEFT_KEY);
            break;
        case Qt::Key_Right:
            renderThread->KeyReleased(Input::RIGHT_KEY);
            break;
        case Qt::Key_A:
            renderThread->KeyReleased(Input::A_KEY);
            break;
        case Qt::Key_B:
            renderThread->KeyReleased(Input::B_KEY);
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            renderThread->KeyReleased(Input::START_KEY);
            break;
        case Qt::Key_Space:
            renderThread->KeyReleased(Input::SELECT_KEY);
            break;
        default:
            break;
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    QKeyEvent *keyEvent = NULL;//event data, if this is a keystroke event
    bool result;//return true to consume the keystroke

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        this->keyPressEvent(keyEvent);
        result = true;
    }
    else if (event->type() == QEvent::KeyRelease) {
        keyEvent = static_cast<QKeyEvent *>(event);
        this->keyReleaseEvent(keyEvent);
        result = true;
    }
    else
        result = QObject::eventFilter(obj, event);

    return result;
}
