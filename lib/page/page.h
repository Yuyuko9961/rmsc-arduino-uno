#ifndef page_h
#define page_h

#include <U8g2lib.h>
#include <constant.h>
#include <drawBitmaps.h>

class Page {

private:
    // 关键信息
    uint8_t currentPage;             // 当前页码
    uint8_t dataLen[DATALINE_NUM];   // 当前页每行字符串长度
    uint8_t dataInfo[DATALINE_NUM];  // 当前页每行数据额外信息
    uint8_t* pageData[DATALINE_NUM]; // 当前页全部信息的指针数组
    uint8_t dataLine1[DATALINE_LENGTH]; // 实际存储第1行信息的数组
    uint8_t dataLine2[DATALINE_LENGTH]; // 实际存储第2行信息的数组
    uint8_t dataLine3[DATALINE_LENGTH]; // 实际存储第3行信息的数组
    uint8_t dataLine4[DATALINE_LENGTH]; // 实际存储第4行信息的数组
    // 其他信息
    uint8_t pageTitle[DATALINE_LENGTH]; // 当前页标题
    uint8_t selectedPos;       // 选中位置
    uint8_t dataLen_selected;  // 选中配置的字符串长度
    uint8_t dataInfo_selected; // 选中配置的额外信息

public:
    Page();
    ~Page();
    void resetData();
    void resetExtraData();
    void drawPage(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);
    void drawPage1(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);
    void drawPage2(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2);
    void showNextLine();
    void showPrevLine();
    void showNextPage();
    void showPrevPage();
    void refreshPage();
    void triggerMsg();
    void updatePage();
    void updateLine(uint8_t line);
    void updateMsg(uint8_t sign);
    void updateMsg(uint8_t sign, uint8_t Line);
    void msgSendLoop(uint8_t msgBuffer[SERIAL_TX_BUFFER_SIZE], uint8_t len);
    bool checkHeader();
};

#endif