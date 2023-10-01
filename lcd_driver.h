#pragma once

#include "lcd_config.h"
#include <Adafruit_GFX.h>

void LCD_Init();
void LCD_Reset();
void LCD_SetRegion(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end);
void LCD_PutPixel(uint x_start, uint y_start, uint color);
void LCD_FillRect(uint x, uint y, uint w, uint h, uint color);
void LCD_DrawRect(uint x, uint y, uint w, uint h, uint color);
void LCD_DisplaySingleColour(int color);
class GC9106_LCD : public Adafruit_GFX
{
public:
    GC9106_LCD() : Adafruit_GFX(160, 80) {}

    void drawPixel(int16_t x, int16_t y, uint16_t color) override
    {
        LCD_PutPixel(x, y, color);
    }

    void fillScreen(uint16_t color)
    {
        LCD_FillRect(0, 0, 160, 80, color);
    }

    void begin()
    {
        LCD_Init();
    }
};