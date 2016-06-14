//
// Created by Javier Luque Sanabria on 19/4/16.
//

#include <Gameboy.h>

void GameBoy::init(std::string filename) {
    cart.reset(new Cartridge());
    filePath = filename;
    bool loaded = cart->loadRom(filename);
    printf("\nCART TYPE: %02x\n", cart->getCartridgeType());

    ioHandler.reset(new IOHandlerDMG());
    timer.reset(new Timer());
    gpuDMG.reset(new GPUDMG());
    input.reset(new Input());
    audio.reset(new Audio());
    mem.reset(new MemoryDMG(cart.get(), ioHandler.get()));
    cpu.reset(new Z80(move(mem)));

    ioHandler->setGPU(gpuDMG.get());
    ioHandler->setTimer(timer.get());
    ioHandler->setCPU(cpu.get());
    ioHandler->setInput(input.get());
    ioHandler->setAudio(audio.get());

    timer->setIOHandler(ioHandler.get());
    gpuDMG->setIOHandler(ioHandler.get());
    input->setIOHandler(ioHandler.get());
    input->Init();
    audio->setIOHandler(ioHandler.get());

    cpu->setCP(0x100);
    cpu->setAF(0x01B0);
    cpu->setBC(0x0013);
    cpu->setDE(0x00D8);
    cpu->setHL(0x014D);
    cpu->setSP(0xFFFE);

    loadSave();

}

screen_t GameBoy::step(std::function<void(blip_sample_t *, int)> f) {
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
        audio->update((uint16_t) instCycles, f);
    }

    return gpuDMG->getLCD()->getScreenBuffer();
}

void GameBoy::KeyPressed(Input::Gameboy_Keys key) {
    input->KeyPressed(key);
}

void GameBoy::KeyReleased(Input::Gameboy_Keys key) {
    input->KeyReleased(key);
}

void GameBoy::writeSave() {
    if (cart->getRAMBanks() == 0 || !cart->hasBattery())
        return;

    std::string fileName = filePath;
    fileName = fileName.replace(fileName.end() - 3, fileName.end(), ".sav");
    ofstream file(fileName, ofstream::out | ofstream::binary);

    if (!file.is_open()) {
        printf("Couldn't save RAM.");   //< Throw exception.
        return;
    }

    if (cart->getCartridgeType() == Cartridge::CARTRIDGETYPE_MBC2)
        file.write(reinterpret_cast<char *>(cpu->getMem()->RAMBanks[0].data()), 0x200);
    else
        for (uint16_t i = 0; i < cart->getRAMBanks(); i++)
            file.write(reinterpret_cast<char *>(cpu->getMem()->RAMBanks[i].data()), 0x2000);

    file.close();
}

void GameBoy::loadSave() {

    if (cart->getRAMBanks() == 0 || !cart->hasBattery())
        return;

    std::string fileName = filePath;
    fileName = fileName.replace(fileName.end() - 3, fileName.end(), ".sav");
    ifstream file(fileName, ifstream::out | ifstream::binary);

    if (!file.is_open()) {
        printf("Couldn't save RAM.");   //< Throw exception.
        return;
    }

    if (cart->getCartridgeType() == Cartridge::CARTRIDGETYPE_MBC2)
        file.read(reinterpret_cast<char *>(cpu->getMem()->RAMBanks[0].data()), 0x200);
    else
        for (uint16_t i = 0; i < cart->getRAMBanks(); i++)
            file.read(reinterpret_cast<char *>(cpu->getMem()->RAMBanks[i].data()), 0x2000);

    file.close();
}