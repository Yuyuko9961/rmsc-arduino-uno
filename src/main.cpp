#include <Arduino.h>
#include <page.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, OLED_RESET);
Page page = Page();

void setup() {
    // 使用内部上拉状态初始化引脚
    pinMode(PIN_CON, INPUT_PULLUP);
    pinMode(PIN_PSH, INPUT_PULLUP);
    pinMode(PIN_TRA, INPUT_PULLUP);
    pinMode(PIN_TRB, INPUT_PULLUP);
    pinMode(PIN_BAK, INPUT_PULLUP);
    // 开启端口
    Serial.begin(115200);
    // 创建屏幕绘制类
    u8g2.begin();
    delay(100);
    // 绘制启动动画与 Logo
    drawOpening(u8g2);
    delay(500);
    // 进入配置列表页
    page.showNextPage();
}

void loop() {
    static unsigned long currentTime = 0;
    static unsigned long lastRefreshTime = 0;
    // 触发器的状态列表 [CON, BAK, PSH, TRA]
    static bool triggerCurrStats[4] = {HIGH, HIGH, HIGH, HIGH};
    static bool triggerLastStats[4] = {HIGH, HIGH, HIGH, HIGH};
    static bool pageRefresh = true;
    currentTime = millis(); // 获取当前时间
    // 更新触发器当前状态
    triggerCurrStats[0] = digitalRead(PIN_CON);
    triggerCurrStats[1] = digitalRead(PIN_BAK);
    triggerCurrStats[2] = digitalRead(PIN_PSH);
    triggerCurrStats[3] = digitalRead(PIN_TRA);
    if (triggerCurrStats[0] == LOW && triggerLastStats[0] == HIGH) {
        // 确认按钮信号
        page.showNextPage();
        pageRefresh = true;
        // Serial.println(F(" | PIN_CON 按下!"));
    } else if (triggerCurrStats[1] == LOW && triggerLastStats[1] == HIGH) {
        // 返回按钮信号
        page.showPrevPage();
        pageRefresh = true;
        // Serial.println(F(" | PIN_BAK 按下!"));
    } else if (triggerCurrStats[2] == LOW && triggerLastStats[2] == HIGH) {
        // 旋转编码器按钮信号
        page.triggerMsg();
        pageRefresh = true;
        // Serial.println(F(" | PIN_PSH 按下!"));
    } else if (triggerCurrStats[3] == LOW && triggerLastStats[3] == HIGH) {
        // 旋转编码器转动信号 (下降沿触发)
        if (digitalRead(PIN_TRB) == HIGH) {
            // 顺时针转动
            page.showNextLine();
            // Serial.println(F(" | 顺时针转动!"));
        } else {
            // 逆时针转动
            page.showPrevLine();
            // Serial.println(F(" | 逆时针转动!"));
        }
        pageRefresh = true;
    } else if (currentTime - lastRefreshTime >= PAGE_REFRESH_INTERVAL) {
        // 页面定时刷新信号, 若检测到交互事件则本次循环不刷新, 优先处理交互事件
        page.refreshPage();
        pageRefresh = true;
        lastRefreshTime = currentTime;
        // Serial.println(F(" | 页面刷新!"));
    }
    // 更新触发器的前一状态
    triggerLastStats[0] = triggerCurrStats[0];
    triggerLastStats[1] = triggerCurrStats[1];
    triggerLastStats[2] = triggerCurrStats[2];
    triggerLastStats[3] = triggerCurrStats[3];
    // 若页面有刷新则重新绘制页面
    if (pageRefresh) {
        pageRefresh = false;
        u8g2.firstPage();
        do {
            page.drawPage(u8g2);
        } while (u8g2.nextPage());
    }
}
