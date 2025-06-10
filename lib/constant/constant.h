#ifndef constant_h
#define constant_h

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Pin
#define PIN_CON 2
#define PIN_TRB 3
#define PIN_TRA 4
#define PIN_PSH 5
#define PIN_BAK 6

// Data
#define DATALINE_NUM 4
#define DATALINE_LENGTH 156
#define SERIAL_TX_BUFFER_SIZE 8
#define SERIAL_RX_BUFFER_SIZE 192
#define PAGE_REFRESH_INTERVAL 20000

// Protocol
/*
    特别说明, 对于每一个字符的 bitmap (LSB):
    若为全字符: 其首字节一定为 0000????
    若为半字符: 其首字节一定为 000?????

    另外, 若当前字节事实上表示一个配置项的信息:
    该字节一定为 00000???, ? 表示不确定

    所以, 对于需判断的字节我们总是拥有 3 个空闲位
    通过在空闲位添加 1, 总共可表达 8 个不同状态
    通过这 8 个不同状态, 设计通信协议进行信息标记与处理
*/

// Message Receive
#define HEADER_RECEIVE_PAGE1 '@'
#define HEADER_RECEIVE_PAGE2 '#'
#define BITMAP_HALF_WIDTH 0       /* 在读取 bitmap 时使用 (000?????) */
#define BITMAP_LINE_START 32      /* 在读取 bitmap 前使用 (00100000) */
#define BITMAP_LINE_END 64        /* 在读取 bitmap 时使用 (010?????) */
#define BITMAP_LINE_EMPTY 96      /* 在读取 bitmap 前使用 (01100000) */
#define BITMAP_FULL_WIDTH 128     /* 在读取 bitmap 时使用 (100?????) */
#define BITMAP_LINE_UNCHANGED 160 /* 在读取 bitmap 前使用 (10100000) */

// Message Send
#define HEADER_SEND '&'
#define MSG_LOOP_INTERVAL 2000
#define MSG_SYS_INIT 0     /* &0\n */
#define MSG_REFRESH_PAGE 1 /* &1\n */
#define MSG_NEXT_PAGE 2    /* &2\n */
#define MSG_PREV_PAGE 3    /* &3\n */
#define MSG_NEXT_LINE 4    /* &4\n */
#define MSG_PREV_LINE 5    /* &5\n */
#define MSG_NEW_LINE 6     /* &6[line]\n */
#define MSG_TRIGGER 7      /* &7[line]\n */

#endif