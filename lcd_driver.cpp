#include <SPI.h>
#include <Adafruit_GFX.h>
#include "lcd_driver.h"

void Contrast_Adjust();

// code uint typFNT_GBK16 hz16[];

void Output_Pixel(uint x, uint y);

void delay_ms(uint time)
{
    delay(time);
}

void SPI_WriteData(uchar Data)
{
    SPI.transfer(Data);
}

void Lcd_WriteIndex(uchar Data)
{
    digitalWrite(CS, LOW);
    digitalWrite(DC, LOW);
    SPI_WriteData(Data);
    digitalWrite(CS, HIGH);
}

void Lcd_WriteData(uchar Data)
{
    digitalWrite(CS, LOW);
    digitalWrite(DC, HIGH);
    SPI_WriteData(Data);
    digitalWrite(CS, HIGH);
}

void LCD_WriteData_16Bit(unsigned int Data)
{
    digitalWrite(CS, LOW);
    digitalWrite(DC, HIGH);
    SPI_WriteData(Data >> 8);
    SPI_WriteData(Data);
    digitalWrite(CS, HIGH);
}

void LCD_Reset()
{
    digitalWrite(RST, LOW);
    delay_ms(100);
    digitalWrite(RST, HIGH);
    delay_ms(100);
}

void initial(void)
{
    Lcd_WriteIndex(0xfe);
    Lcd_WriteIndex(0xfe);
    Lcd_WriteIndex(0xfe);
    Lcd_WriteIndex(0xef);

    Lcd_WriteIndex(0xb3);
    Lcd_WriteData(0x03);

    Lcd_WriteIndex(0x36);
    Lcd_WriteData(0x60);

    Lcd_WriteIndex(0x3a);
    Lcd_WriteData(0x05);

    Lcd_WriteIndex(0xb6);
    Lcd_WriteData(0x11);
    Lcd_WriteIndex(0xac);
    Lcd_WriteData(0x0b);

    Lcd_WriteIndex(0xb4);
    Lcd_WriteData(0x21);

    Lcd_WriteIndex(0xb1);
    Lcd_WriteData(0xc0);

    Lcd_WriteIndex(0xe6);
    Lcd_WriteData(0x50);
    Lcd_WriteData(0x43);
    Lcd_WriteIndex(0xe7);
    Lcd_WriteData(0x56);
    Lcd_WriteData(0x43);

    Lcd_WriteIndex(0xF0);
    Lcd_WriteData(0x1f);
    Lcd_WriteData(0x41);
    Lcd_WriteData(0x1B);
    Lcd_WriteData(0x55);
    Lcd_WriteData(0x36);
    Lcd_WriteData(0x3d);
    Lcd_WriteData(0x3e);
    Lcd_WriteData(0x0);
    Lcd_WriteData(0x16);
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x14);
    Lcd_WriteData(0xf);

    Lcd_WriteIndex(0xF1);

    Lcd_WriteData(0x1f);
    Lcd_WriteData(0x41);
    Lcd_WriteData(0x1B);
    Lcd_WriteData(0x55);
    Lcd_WriteData(0x36);
    Lcd_WriteData(0x3d);
    Lcd_WriteData(0x3e);
    Lcd_WriteData(0x0);
    Lcd_WriteData(0x16);
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x14);
    Lcd_WriteData(0xf);

    Lcd_WriteIndex(0xfe);
    Lcd_WriteIndex(0xff);

    Lcd_WriteIndex(0x35);
    Lcd_WriteData(0x00);
    Lcd_WriteIndex(0x44);
    Lcd_WriteData(0x00);
    Lcd_WriteIndex(0x11);
    delay_ms(120);
    Lcd_WriteIndex(0x29);
    // Lcd_WriteIndex(0x2c);

    Lcd_WriteIndex(0x2A); // Set Column Address
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x67);
    Lcd_WriteIndex(0x2B); // Set Page Address
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x9f);
    Lcd_WriteIndex(0x2c);
}

void LCD_SetRegion(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end)
{

    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_start + Xoffset);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_end + Xoffset);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_start + Yoffset);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_end + Yoffset);
    Lcd_WriteIndex(0x2c);
}

void LCD_PutPixel(uint x_start, uint y_start, uint color)
{
    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_start + Xoffset);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_start + Xoffset); // Explicitly use start coordinates

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_start + Yoffset);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_start + Yoffset); // Explicitly use start coordinates
    Lcd_WriteIndex(0x2c);

    LCD_WriteData_16Bit(color);
}

void LCD_FillRect(uint x, uint y, uint w, uint h, uint color)
{
    LCD_SetRegion(x, y, x + w - 1, y + h - 1);
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            LCD_WriteData_16Bit(color);
        }
    }
}

void LCD_DrawRect(uint x, uint y, uint w, uint h, uint color)
{
    LCD_FillRect(x, y, w, 1, color);
    LCD_FillRect(x, y + h - 1, w, 1, color);
    LCD_FillRect(x, y, 1, h, color);
    LCD_FillRect(x + w - 1, y, 1, h, color);
}

void LCD_Init()
{
    SPI.begin();
    pinMode(CS, OUTPUT);
    pinMode(DC, OUTPUT);
    pinMode(RST, OUTPUT);
    pinMode(BL, OUTPUT);
    digitalWrite(BL, HIGH);
    LCD_Reset();
    initial();
    LCD_FillRect(0, 0, 160, 80, BLACK);
}
