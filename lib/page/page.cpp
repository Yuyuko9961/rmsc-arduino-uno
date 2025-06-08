#include "page.h"

// 构造函数
Page::Page() {
    currentPage = 0;
}

// 析构函数
Page::~Page() {
    currentPage = 0;
}

// 重置页面数据
void Page::resetData() {
    for (uint8_t i = 0; i < DATALINE_NUM; i++) {
        dataID[i] = dataLen[i] = dataInfo[i] = 0;
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
    dataID_selected = dataLen_selected = dataInfo_selected = 0;
    memset(pageTitle, 0, DATALINE_LENGTH);
    selectedPos = 0;
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

// 绘制数据详情页 (page2)
void Page::drawPage2(U8G2_SH1106_128X64_NONAME_1_HW_I2C& u8g2) {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(2, 2, 124, 60);
    // 表头区
    drawText(4, 4, pageTitle, dataLen_selected, u8g2);
    u8g2.drawBox(104, 4, 20, 10);
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
        for (uint8_t i = 0; i < DATALINE_NUM - 1; i++) {
            dataID[i] = dataID[i + 1];
            dataLen[i] = dataLen[i + 1];
            dataInfo[i] = dataInfo[i + 1];
            pageData[i] = pageData[i + 1];
        }
        updatePageMsg(MSG_NEXT_LINE); // 发送页面信息的更新请求
        updatePage(); // 期望返回: [255, 255, 255, 新行ID]
    }
}

// 数据转移至上一行
void Page::showPrevLine() {
    if (currentPage == 1 && selectedPos > 0) {
        // 若当前为配置列表页且选中框还有上移空间
        selectedPos--;
    } else {
        // 向下轮转行数据
        for (uint8_t i = DATALINE_NUM - 1; i > 0; i--) {
            dataID[i] = dataID[i - 1];
            dataLen[i] = dataLen[i - 1];
            dataInfo[i] = dataInfo[i - 1];
            pageData[i] = pageData[i - 1];
        }
        updatePageMsg(MSG_PREV_LINE); // 发送页面信息的更新请求
        updatePage(); // 期望返回: [新行ID, 255, 255, 255]
    }
}

// 数据转移至下一页
void Page::showNextPage() {
    if (currentPage == 0) {
        currentPage = 1;
        resetExtraData();
        memcpy_P(pageTitle, page1Title, PAGE1TITLE_LEN);
        resetData();
        refreshPage();
    } else if (currentPage == 1 && (dataInfo[selectedPos] & 4)) {
        // 需要当前选中配置项有物品空间才能跳转至数据详情页
        currentPage = 2;
        resetExtraData();
        memcpy(pageTitle, pageData[selectedPos], DATALINE_LENGTH);
        dataID_selected = dataID[selectedPos];
        dataLen_selected = dataLen[selectedPos];
        dataInfo_selected = dataInfo[selectedPos];
        resetData();
        refreshPage();
    }
}

// 数据转移至上一页
void Page::showPrevPage() {
    if (currentPage == 2) {
        currentPage = 1;
        resetExtraData();
        memcpy_P(pageTitle, page1Title, PAGE1TITLE_LEN);
        resetData();
        refreshPage();
    }
}

// 刷新页面 (定时刷新或翻页时触发)
void Page::refreshPage() {
    updatePageMsg(MSG_ENTIRE_PAGE); // 发送页面信息的更新请求
    updatePage();
}

// 各种触发器事件
void Page::triggerMsg() {
    int32_t data = 0;
    if (currentPage == 1 && (dataInfo[selectedPos] & 2)) {
        // 需要当前配置项有按钮事件才能触发
        updateLineMsg(MSG_TRIGGER, dataID[selectedPos]);
        data = Serial.read();
        dataInfo[selectedPos] = (data >= 0 && data <= 255) ? (uint8_t)data : 0;
    } else if (currentPage == 2 && (dataInfo_selected & 2)) {
        // 需要当前配置项有按钮事件才能触发
        updateLineMsg(MSG_TRIGGER, dataID_selected);
        data = Serial.read();
        dataID_selected = (data >= 0 && data <= 255) ? (uint8_t)data : 0;
    }
}

// 更新一整页的信息
void Page::updatePage() {
    uint8_t dataStat[DATALINE_NUM] = {};
    int32_t data = 0;
    memset(dataStat, 0, DATALINE_NUM);
    for (uint8_t line = 0; line < DATALINE_NUM; line++) {
        data = Serial.read(); // 获取数据变更状态
        dataStat[line] = (data >= 0 && data <= 255) ? (uint8_t)data : 0;
    }
    for (uint8_t line = 0; line < DATALINE_NUM; line++) {
        if (dataStat[line] == LINE_NOCHANGE) {
            continue; // 若该行无变化则跳过更新
        }
        // 清空该行数据
        dataID[line] = dataLen[line] = dataInfo[line] = 0;
        memset(pageData[line], 0, DATALINE_LENGTH);
        if (dataStat[line] == LINE_EMPTY) {
            continue; // 若该行为空, 清空数据后跳过更新
        }
        // 若该行不为空, 更新数据
        dataID[line] = dataStat[line];
        if (currentPage == 1) {
            data = Serial.read(); // 获取该行配置信息
            dataInfo[line] = (data >= 0 && data <= 255) ? (uint8_t)data : 0;
        }
        updateLineMsg(MSG_SINGLE_LINE, dataID[line]); // 发送该行数据的更新请求
        updateLine(line); // 读取新获取到的该行内容
    }
}

// 发送页面信息的更新请求
void Page::updatePageMsg(uint8_t sign) {
    uint8_t msgBuffer[SERIAL_TX_BUFFER_SIZE]; // msg发送缓冲区
    memset(msgBuffer, 0, SERIAL_TX_BUFFER_SIZE);
    msgBuffer[0] = currentPage == 1 ? HEADER_PAGE1 : HEADER_PAGE2; // header
    msgBuffer[1] = sign; // 消息类型
    for (uint8_t i = 0; i < DATALINE_NUM; i++) {
        msgBuffer[i + 2] = dataID[i];
    }
    msgSendLoop(msgBuffer, 6);
}

// 更新一行数据
void Page::updateLine(uint8_t line) {
    uint8_t count = 0, pos = 0;
    int32_t data = 0;
    while (Serial.available()) {
        data = Serial.read(); // 读取一个8位字符
        data = (data >= 0 && data <= 255) ? (uint8_t)data : 0;
        if (count == 0) { // 当前字符是一个bitmap的首位或bitmap段结束标志
            if (data == BITMAP_END) { // 字符是bitmap段结束标志
                break; // bitmap段已结束, 跳出循环
            } else if (data & 128) { // 当前bitmap为全字符
                count += 13; // 计数器增加一个全字符
            } else { // 当前bitmap为半字符
                count += 10; // 计数器增加一个半字符
            }
            dataLen[line]++; // 若没有跳出, 字符串总长度+1
        }
        pageData[line][pos++] = data; // 读取数据, 读取指针自增
        count--; // 计数器自减
    }
}

// 发送单行数据的更新请求
void Page::updateLineMsg(uint8_t sign, uint8_t ID) {
    uint8_t msgBuffer[SERIAL_TX_BUFFER_SIZE]; // msg发送缓冲区
    memset(msgBuffer, 0, SERIAL_TX_BUFFER_SIZE);
    msgBuffer[0] = currentPage == 1 ? HEADER_PAGE1 : HEADER_PAGE2; // header
    msgBuffer[1] = sign; // 消息类型
    msgBuffer[2] = ID;
    msgSendLoop(msgBuffer, 3);
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
        if (Serial.available() && checkHeader()) {
            break; // 信息头检查通过, 跳出请求循环
        }
        while (Serial.available()) {
            Serial.read(); // 检查不通过, 清空串口输入缓存
        }
    }
    delay(50); // 小延迟, 等待全部数据传输完成
}

// 检查信息头
bool Page::checkHeader() {
    uint8_t header = Serial.read();
    if (currentPage == 1) {
        return header == HEADER_PAGE1;
    } else {
        return header == HEADER_PAGE2;
    }
}