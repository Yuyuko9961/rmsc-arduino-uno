#include "drawBitmaps.h"

// 解析函数
void decodeWord(uint8_t word[], uint8_t decoded[20], bool isWide) {
    memset(decoded, 0, 20);
    if (isWide) {
        for (uint8_t i = 3; i < 13; i++) {
            decoded[(i - 3) << 1] = word[i];
        }
        uint8_t t = 0b00000011;
        decoded[1] = word[0] & t;
        decoded[3] = (word[0] & (t << 2)) >> 2;
        for (uint8_t i = 0; i < 8; i += 2) {
            decoded[i + 5] = (word[1] & (t << i)) >> i;
        }
        for (uint8_t i = 0; i < 8; i += 2) {
            decoded[i + 13] = (word[2] & (t << i)) >> i;
        }
    } else {
        for (uint8_t i = 0; i < 10; i++) {
            decoded[i] = word[i];
        }
    }
}

// 解析并绘制一行文字
void drawText(uint8_t x, uint8_t y, uint8_t line[], uint8_t wordsNum, U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    uint8_t pos = 0, alrDisplayed = 0;
    uint8_t decoded[20] = {};
    while (alrDisplayed < wordsNum) {
        if (line[pos] & 128) {
            decodeWord(line + pos, decoded, true);
            u8g2.drawXBM(x, y, 10, 10, decoded);
            x += 10;
            pos += 13;
        } else {
            decodeWord(line + pos, decoded, false);
            u8g2.drawXBM(x, y, 5, 10, decoded);
            x += 5;
            pos += 10;
        }
        alrDisplayed++;
    }
}

// 绘制文字区域的全部文字
void drawTextArea(uint8_t x, uint8_t y[], uint8_t* lines[], uint8_t wordsNums[], U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    for (uint8_t i = 0; i < DATALINE_NUM; i++) {
        drawText(x, y[i], lines[i], wordsNums[i], u8g2);
    }
}

// 绘制一行的数据状态
void drawStat(uint8_t x, uint8_t y, uint8_t dataStat, U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    if (dataStat & 4) { // 该配置有存储空间
        u8g2.drawXBMP(x + 12, y, 8, 10, storageAvailable);
    }
    if (dataStat & 2) { // 该配置有开关
        if (dataStat & 1) { // 开关开启
            u8g2.drawXBMP(x, y, 11, 10, switchOn);
        } else {
            u8g2.drawXBMP(x, y, 11, 10, switchOff);
        }
    }
}

// 绘制文字区域的全部数据状态
void drawStatArea(uint8_t x, uint8_t y[], uint8_t dataStats[], U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    for (uint8_t i = 0; i < DATALINE_NUM; i++) {
        drawStat(x, y[i], dataStats[i], u8g2);
    }
}

// 绘制选中标记框
void drawSelectBox(uint8_t y, U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    u8g2.drawFrame(2, y, 124, 14);
    u8g2.setDrawColor(0);
    u8g2.drawHLine(12, y, 104);
    u8g2.drawHLine(12, y + 13, 104);
    u8g2.drawVLine(2, y + 5, 4);
    u8g2.drawVLine(125, y + 5, 4);
    u8g2.setDrawColor(1);
}
