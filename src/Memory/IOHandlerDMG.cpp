//
// Created by Javier Luque Sanabria on 8/4/16.
//
#include <IOHandlerDMG.h>

IOHandlerDMG::IOHandlerDMG() {

    //< Set specific values in IOPorts.
    setIOReg(IOHandler::TIMA, 0x00);
    setIOReg(IOHandler::TMA, 0x00);
    setIOReg(IOHandler::TAC, 0x00);

    setIOReg(IOHandler::NR_10, 0x80);
    setIOReg(IOHandler::NR_11, 0xBF);
    setIOReg(IOHandler::NR_12, 0xF3);
    setIOReg(IOHandler::NR_14, 0xBF);
    setIOReg(IOHandler::NR_21, 0x3F);
    setIOReg(IOHandler::NR_22, 0x00);
    setIOReg(IOHandler::NR_24, 0xBF);
    setIOReg(IOHandler::NR_30, 0x7F);
    setIOReg(IOHandler::NR_31, 0xFF);
    setIOReg(IOHandler::NR_32, 0x9F);
    setIOReg(IOHandler::NR_33, 0xBF);
    setIOReg(IOHandler::NR_41, 0xFF);
    setIOReg(IOHandler::NR_42, 0x00);
    setIOReg(IOHandler::NR_43, 0x00);
    setIOReg(IOHandler::NR_44, 0xBF);
    setIOReg(IOHandler::NR_50, 0x77);
    setIOReg(IOHandler::NR_51, 0xF3);
    setIOReg(IOHandler::NR_52, 0xF1);

    setIOReg(IOHandler::LCDC, 0x91);
    setIOReg(IOHandler::SCY, 0X00);
    setIOReg(IOHandler::SCX, 0x00);
    setIOReg(IOHandler::LYC, 0x00);
    setIOReg(IOHandler::BGP, 0xFC);
    setIOReg(IOHandler::OBP0, 0xFF);
    setIOReg(IOHandler::OBP1, 0xFF);
    setIOReg(IOHandler::WY, 0x00);
    setIOReg(IOHandler::WX, 0x00);
}

void IOHandlerDMG::writeIOReg(IOREGS regIO, uint8_t value) {
    switch(regIO) {
        case DIV:
            timer->resetDIVCycles();
            IOPorts[regIO - 0xFF00] = 0x0;
            break;
        case TAC:
        {
            //< Reset TIMA if change the frequency in TAC.
            uint8_t tac = getIOReg(TAC);
            if ((tac & 0x03) != (value & 0x03))
                timer->resetTIMACycles();
            IOPorts[regIO - 0xFF00] = value;
        }
        default:
            IOPorts[regIO - 0xFF00] = value;
            break;
    }

}

uint8_t IOHandlerDMG::readIOReg(IOREGS regIO) {

    switch(regIO) {
        default:
            return IOPorts[regIO - 0xFF00];
    }

}