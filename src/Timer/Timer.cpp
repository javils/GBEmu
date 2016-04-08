//
// Created by Javier Luque Sanabria on 8/4/16.
//
#include <IOHandler.h>
#include "Timer.h"

void Timer::update(uint16_t clock) {

    divCycles += clock;

    //< 4194304/16384 = 256. Each 256 cycles IO Register DIV(0xFF04) is incremented.
    while (divCycles >= 0xFF) {
        divCycles -= 0xFF;
        uint8_t div = ioHandler->getIOReg(IOHandler::DIV);
        div++;
        ioHandler->setIOReg(IOHandler::DIV, div);
    }

    uint8_t tac = ioHandler->getIOReg(IOHandler::TAC);

    //< Timer enable
    if ((tac & 0x04) != 0) {
        TIMACycles += clock;

        selectFrequency();

        while (TIMACycles >= frequency) {
            TIMACycles -= frequency;

            uint8_t TIMA = ioHandler->getIOReg(IOHandler::TIMA);

            //< If tima Overflow, TIMA = TMA and request interrupt.
            if (TIMA == 0xFF) {
                ioHandler->setIOReg(IOHandler::TIMA, ioHandler->getIOReg(IOHandler::TMA));
                cpu->requestInterrupt(TIMER_INT);
            }
            else
                ioHandler->setIOReg(IOHandler::TIMA, ++TIMA);

        }
    }

}

void Timer::selectFrequency() {
    uint8_t tac = ioHandler->getIOReg(IOHandler::TAC);
    switch (tac & 0x03) {
        case 0x0:   //< 4194304/4096 = 1024.
            frequency = 0x400;
            break;
        case 0x1:   //< 4194304/262144 = 16.
            frequency = 0x10;
            break;
        case 0x2:   //< 4194304/65536 = 64.
            frequency = 0x40;
            break;
        case 0x3:   //< 4194304/16384 = 256
            frequency = 0x100;
            break;
        default:
            break;
    }
}