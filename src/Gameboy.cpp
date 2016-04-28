//
// Created by Javier Luque Sanabria on 19/4/16.
//

#include <Gameboy.h>

void GameBoy::init(std::string filename) {
    cart.reset(new Cartridge());
    bool loaded = cart->loadRom(filename);
    printf("\nCART TYPE: %02x\n", cart->getCartridgeType());

    ioHandler.reset(new IOHandlerDMG());
    timer.reset(new Timer());
    gpuDMG.reset(new GPUDMG());
    input.reset(new Input());
    mem.reset(new MemoryDMG(cart.get(), ioHandler.get()));
    cpu.reset(new Z80(move(mem)));

    ioHandler->setGPU(gpuDMG.get());
    ioHandler->setTimer(timer.get());
    ioHandler->setCPU(cpu.get());
    ioHandler->setInput(input.get());

    timer->setIOHandler(ioHandler.get());
    gpuDMG->setIOHandler(ioHandler.get());
    input->setIOHandler(ioHandler.get());
    input->Init();

    cpu->setCP(0x100);
    cpu->setAF(0x01B0);
    cpu->setBC(0x0013);
    cpu->setDE(0x00D8);
    cpu->setHL(0x014D);
    cpu->setSP(0xFFFE);

}

screen_t GameBoy::step() {
    uint32_t currentCycle = 0;
    uint32_t instCycles = 0;
    cpu->setClockCounter(0);
    bool vblank = false;
    while (!vblank) {
        currentCycle = cpu->getClockCounter();
        cpu->step();
        instCycles = cpu->getClockCounter() - currentCycle;
        timer->update((uint16_t) instCycles);
        vblank = gpuDMG->update((uint8_t) instCycles);
        input->update((uint8_t) instCycles);
    }

    return gpuDMG->getLCD()->getScreenBuffer();
}

void GameBoy::KeyPressed(Input::Gameboy_Keys key) {
    input->KeyPressed(key);
}

void GameBoy::KeyReleased(Input::Gameboy_Keys key) {
    input->KeyReleased(key);
}