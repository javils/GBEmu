//
// Created by Javier Luque Sanabria on 14/4/16.
//

#ifndef GBEMU_LCD_H
#define GBEMU_LCD_H

#include <array>
#include <vector>
class LCDColor {
private:
    uint8_t r, g, b, a;
public:
    LCDColor () { r = 0x00; g = 0x00; b = 0x00; a = 0xFF; }
    inline void setColor(uint8_t r, uint8_t g, uint8_t b) { setR(r); setG(g); setB(b); }
    inline void setR(uint8_t r) { this->r = r; }
    inline void setG(uint8_t g) { this->g = g; }
    inline void setB(uint8_t b) { this->b = b; }

    inline void setA(uint8_t a) { this->a = a; }

    inline uint8_t getR() { return r; }
    inline uint8_t getG() { return g; }
    inline uint8_t getB() { return b; }
    inline uint32_t getColor() { return (uint32_t) ((r << 24) | (g << 16) | (b << 8) | a); }
};

using namespace std;
class LCD {
public:

    LCD() {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
            for(int x = 0; x < SCREEN_WIDTH; x++)
                screenBuffer[y][x] = 0;

    }

    static const uint8_t SCREEN_WIDTH = 160;
    static const uint8_t SCREEN_HEIGHT = 144;
    static const uint8_t SCREEN_HEIGHT_VBLANK = 153;

    inline void setPixelColor(uint8_t x, uint8_t y, uint8_t color) { screenBuffer[y][x] = color; }

    inline array<array<uint8_t, SCREEN_WIDTH>, SCREEN_HEIGHT> getScreenBuffer() { return screenBuffer; };
private:
    array<array<uint8_t, LCD::SCREEN_WIDTH>, LCD::SCREEN_HEIGHT> screenBuffer;
};

typedef array<array<uint8_t, LCD::SCREEN_WIDTH>, LCD::SCREEN_HEIGHT> screen_t;
#endif //GBEMU_LCD_H
