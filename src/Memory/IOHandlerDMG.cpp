//
// Created by Javier Luque Sanabria on 8/4/16.
//
#include <IOHandlerDMG.h>

IOHandlerDMG::IOHandlerDMG() {

    IOPorts.fill(0x0);

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
            break;
        }
        case LCDC:
        {
            IOPorts[regIO - 0xFF00] = value;

            //< Enable or disable LCD
            gpuDMG->enableLCD((value & 0x80) != 0);
            break;
        }
        case STAT:
        {
            //< Three lower bits are read only
            uint8_t stat = (uint8_t) (IOPorts[regIO - 0xFF00] & 0x07);
            uint8_t interrupt = (uint8_t) (IOPorts[regIO - 0xFF00] & 0x70);
            interrupt >>= 3;
            //< & 0x78 because only use the lower 7 bits of the byte.
            IOPorts[regIO - 0xFF00] = uint8_t((value & 0x78) | stat);

            if (!gpuDMG->isLCDEnable())
                return;


            uint8_t gpuMode = gpuDMG->getGPUMode();

            //< Request interrupt if the flag is enabled and we are in the correct mode.
            if ((interrupt  & 0x04) && gpuMode == GPU::SCAN_OAM)
                cpu->requestInterrupt(LCD_STAT_INT);
            if ((interrupt  & 0x02) && gpuMode == GPU::VBLANK)
                    cpu->requestInterrupt(LCD_STAT_INT);
            if ((interrupt  & 0x01) && gpuMode == GPU::HBLANK)
                    cpu->requestInterrupt(LCD_STAT_INT);

            break;
        }
        case LY:
        {
            //< Only read, write here reset the counter.
            IOPorts[regIO - 0xFF00] = 0x0;
            break;
        }
        case LYC:
        {
            IOPorts[regIO - 0xFF00] = value;

            if (gpuDMG->isLCDEnable())
                gpuDMG->checkLYWithLYC();
            break;
        }
        case DMA:
        {
            uint16_t address = value << 8 ; // source address is data * 100
            for (uint16_t i = 0 ; i < 0xA0; i++)
            {
                cpu->writeByteMem((uint16_t) (0xFE00 + i), cpu->readByteMem(address + i)) ;
            }
            break;
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