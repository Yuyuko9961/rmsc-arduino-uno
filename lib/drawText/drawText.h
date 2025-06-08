#ifndef drawText_h
#define drawText_h

#include <U8g2lib.h>
#include <bitmaps.h>
#include <constant.h>

void decodeWord(uint8_t word[], uint8_t decoded[20], bool isWide);
void drawText(uint8_t x, uint8_t y, uint8_t line[], uint8_t wordsNum, U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);
void drawTextArea(uint8_t x, uint8_t y[], uint8_t* lines[], uint8_t wordsNums[], U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);
void drawStatArea(uint8_t x, uint8_t y[], uint8_t dataStats[], U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);
void drawSelectBox(uint8_t y, U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);

#endif