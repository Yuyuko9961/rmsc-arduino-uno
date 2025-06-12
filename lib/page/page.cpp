#include "page.h"

// 构造函数
Page::Page() {
    resetData();
}

// 析构函数
Page::~Page() {
    resetData();
}

// 重置页面数据
void Page::resetData() {
    currentPage = 0;
    for (uint8_t i = 0; i < DATALINE_NUM; i++) {
        dataLen[i] = dataInfo[i] = 0;
    }
    memset(dataLine1, 0, DATALINE_LENGTH);
    memset(dataLine2, 0, DATALINE_LENGTH);
    memset(dataLine3, 0, DATALINE_LENGTH);
    memset(dataLine4, 0, DATALINE_LENGTH);
    pageData[0] = dataLine1;
    pageData[1] = dataLine2;
    pageData[2] = dataLine3;
    pageData[3] = dataLine4;
}

// 重置页面中的其他信息 (title等)
void Page::resetExtraData() {
    dataLen_selected = dataInfo_selected = 0;
    memset(pageTitle, 0, DATALINE_LENGTH);
}

// 绘制页面
void Page::drawPage(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    if (currentPage == 1) {
        drawPage1(u8g2);
    } else {
        drawPage2(u8g2);
    }
}

// 绘制配置列表页 (page1)
void Page::drawPage1(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    u8g2.drawFrame(0, 0, 128, 64);
    // 表头区
    drawText(34, 2, pageTitle, 4, u8g2);
    drawText(104, 2, pageTitle + 52, 2, u8g2);
    u8g2.drawHLine(0, 13, 128);
    // 选中标记框
    uint8_t posY[DATALINE_NUM] = {15, 26, 37, 48};
    drawSelectBox(posY[selectedPos], u8g2);
    for (uint8_t i = 0; i < DATALINE_NUM; i++) {
        if (selectedPos <= i) {
            posY[i] += selectedPos == i ? 2 : 4;
        }
    }
    // 主要文字区
    drawTextArea(4, posY, pageData, dataLen, u8g2);
    drawStatArea(104, posY, dataInfo, u8g2);

}

// 绘制产物列表页 (page2)
void Page::drawPage2(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(2, 2, 124, 60);
    // 表头区
    drawText(4, 4, pageTitle, dataLen_selected, u8g2);
    drawStat(104, 4, dataInfo_selected, u8g2);
    u8g2.drawHLine(2, 15, 124);
    // 主要文字区
    uint8_t posY[DATALINE_NUM] = {17, 28, 39, 50};
    drawTextArea(4, posY, pageData, dataLen, u8g2);
}

// 数据转移至下一行
void Page::showNextLine() {
    if (currentPage == 1 && selectedPos < DATALINE_NUM - 1) {
        // 若当前为配置列表页且选中框还有下移空间
        selectedPos++;
    } else {
        // 向上轮转行数据
        uint8_t* temp = pageData[0];
        for (uint8_t i = 0; i < DATALINE_NUM - 1; i++) {
            dataLen[i] = dataLen[i + 1];
            dataInfo[i] = dataInfo[i + 1];
            pageData[i] = pageData[i + 1];
        }
        pageData[DATALINE_NUM - 1] = temp;
        updateMsg(MSG_NEXT_LINE); // 发送页面信息的更新请求
        updatePage();
    }
}

// 数据转移至上一行
void Page::showPrevLine() {
    if (currentPage == 1 && selectedPos > 0) {
        // 若当前为配置列表页且选中框还有上移空间
        selectedPos--;
    } else {
        // 向下轮转行数据
        uint8_t* temp = pageData[DATALINE_NUM - 1];
        for (uint8_t i = DATALINE_NUM - 1; i > 0; i--) {
            dataLen[i] = dataLen[i - 1];
            dataInfo[i] = dataInfo[i - 1];
            pageData[i] = pageData[i - 1];
        }
        pageData[0] = temp;
        updateMsg(MSG_PREV_LINE); // 发送页面信息的更新请求
        updatePage();
    }
}

// 数据转移至下一页
void Page::showNextPage() {
    if (currentPage == 0) {
        currentPage = 1;
        selectedPos = 0;
        resetExtraData();
        memcpy_P(pageTitle, page1Title, PAGE1TITLE_LEN);
        updateMsg(MSG_JUMPTO_PAGE1);
        updatePage();
    } else if (currentPage == 1 && (dataInfo[selectedPos] & 4)) {
        // 当前选中配置项有存储空间, 跳转至产物列表页
        currentPage = 2;
        resetExtraData();
        memcpy(pageTitle, pageData[selectedPos], DATALINE_LENGTH);
        dataLen_selected = dataLen[selectedPos];
        dataInfo_selected = dataInfo[selectedPos];
        updateMsg(MSG_JUMPTO_PAGE2, selectedPos);
        updatePage();
    }
}

// 数据转移至上一页
void Page::showPrevPage() {
    if (currentPage == 2) {
        currentPage = 1;
        resetExtraData();
        memcpy_P(pageTitle, page1Title, PAGE1TITLE_LEN);
        updateMsg(MSG_JUMPTO_PAGE1);
        updatePage();
    }
}

// 刷新页面 (定时刷新)
void Page::refreshPage() {
    updateMsg(MSG_REFRESH_PAGE); // 发送页面信息的更新请求
    updatePage();
}

// 各种触发器事件
void Page::triggerMsg() {
    int32_t data = 0;
    if (currentPage == 1 && (dataInfo[selectedPos] & 2)) {
        // 当前配置项有按钮事件
        updateMsg(MSG_TRIGGER, selectedPos);
        data = Serial.read();
        if (data != -1) {
            dataInfo[selectedPos] = (uint8_t)data;
        }
    } else if (currentPage == 2 && (dataInfo_selected & 2)) {
        // 当前配置项有按钮事件
        updateMsg(MSG_TRIGGER, selectedPos);
        data = Serial.read();
        if (data != -1) {
            dataInfo_selected = (uint8_t)data;
        }
    }
}

// 更新一整页的信息
void Page::updatePage() {
    int32_t data = 0; // 每次调用 read() 从串口缓冲区获得的数据
    for (uint8_t line = 0; line < DATALINE_NUM; line++) {
        data = Serial.read(); // 读取一个字节
        data = (data != -1) ? (uint8_t)data : BITMAP_LINE_UNCHANGED;
        if (data == BITMAP_LINE_START) {
            // 当前行收到新内容, 清空该行的旧数据并更新
            dataLen[line] = dataInfo[line] = 0;
            memset(pageData[line], 0, DATALINE_LENGTH);
            updateLine(line);
            if (line + 1 < DATALINE_NUM) {
                // 还有未收到的更新信息, 发送请求
                updateMsg(MSG_MORE_LINE);
            }
        } else if (data == BITMAP_LINE_EMPTY) {
            // 清空该行的旧数据, 但不做更新
            dataLen[line] = dataInfo[line] = 0;
            memset(pageData[line], 0, DATALINE_LENGTH);
        }
        // 当前行未改变, 跳过后续
    }
    // 若存在, 清空输入缓存中无法处理的剩余信息
    while (Serial.available()) {
        Serial.read();
    }
}

// 更新一行数据
void Page::updateLine(uint8_t line) {
    uint8_t count = 0; // 控制何时进行字符宽度判断的计数器
    uint8_t pos = 0; // bitmap 读取指针
    int32_t data = 0; // 每次调用 read() 从串口缓冲区获得的数据
    while (pos < DATALINE_LENGTH && Serial.available()) {
        data = Serial.read(); // 读取一个字节
        // 若从串口获取的数据不完整或过长, 将当前字节改为结束符, 确保当前行的读取过程正常结束
        data = (data != -1 || pos >= DATALINE_LENGTH - 1) ? (uint8_t)data : BITMAP_LINE_END;
        if (count == 0) { // 当前字节用于判断字符的宽度
            if (data & BITMAP_LINE_END) {
                // 若当前为配置列表页, 从结束标记中提取该行的配置信息
                dataInfo[line] = currentPage == 1 ? data & 0b00000111 : 0;
                break; // bitmap 段已结束, 跳出循环
            } else if (data & BITMAP_FULL_WIDTH) {
                count += 13; // 计数器增加一个全字符
            } else {
                count += 10; // 计数器增加一个半字符
            }
            dataLen[line]++; // 若没有跳出, 字符串总长度+1
        }
        pageData[line][pos++] = data; // 读取数据, 读取指针自增
        count--; // 计数器自减
    }
    // 若存在, 清空输入缓存中无法处理的剩余信息
    while (Serial.available()) {
        Serial.read();
    }
}

// 发送页面信息的更新请求
void Page::updateMsg(uint8_t sign) {
    uint8_t msgBuffer[SERIAL_TX_BUFFER_SIZE]; // msg发送缓冲区
    memset(msgBuffer, 0, SERIAL_TX_BUFFER_SIZE);
    msgBuffer[0] = HEADER_SEND; // 请求消息的 header
    msgBuffer[1] = sign; // 消息类型
    msgBuffer[2] = '\n'; // 结束符
    msgSendLoop(msgBuffer, 3);
}

// 发送页面信息的更新请求 (带有指定行)
void Page::updateMsg(uint8_t sign, uint8_t Line) {
    uint8_t msgBuffer[SERIAL_TX_BUFFER_SIZE]; // msg发送缓冲区
    memset(msgBuffer, 0, SERIAL_TX_BUFFER_SIZE);
    msgBuffer[0] = HEADER_SEND; // 请求消息的 header
    msgBuffer[1] = sign; // 消息类型
    msgBuffer[2] = Line; // 要请求的行
    msgBuffer[3] = '\n'; // 结束符
    msgSendLoop(msgBuffer, 4);
}

// 消息发送等待循环
void Page::msgSendLoop(uint8_t msgBuffer[SERIAL_TX_BUFFER_SIZE], uint8_t len) {
    unsigned long currentTime = 0, lastSendTime = 0;
    while (true) {
        currentTime = millis();
        if (currentTime - lastSendTime >= MSG_LOOP_INTERVAL) {
            Serial.write(msgBuffer, len);
            lastSendTime = currentTime;
        }
        if (Serial.available()) {
            if (checkHeader()) {
                break; // 信息头检查通过, 跳出请求循环
            }
            // 检查不通过, 清空输入缓存中无法处理的剩余信息
            while (Serial.available()) {
                Serial.read();
            }
        }
    }
    delay(16); // 小延迟, 等待全部数据传输完成
}

// 检查接收消息的信息头
bool Page::checkHeader() {
    uint8_t header = Serial.read();
    if (currentPage == 1) {
        return header == HEADER_RECEIVE_PAGE1;
    } else {
        return header == HEADER_RECEIVE_PAGE2;
    }
}
