#ifndef constant_h
#define constant_h

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// pin
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
#define PAGE_REFRESH_INTERVAL 5000

// Message
#define HEADER_PAGE1 '@'
#define HEADER_PAGE2 '#'
#define BITMAP_END 192
#define LINE_EMPTY 0
#define LINE_NOCHANGE 255

#define MSG_LOOP_INTERVAL 2000
#define MSG_SYS_INIT 0
#define MSG_ENTIRE_PAGE 1
#define MSG_NEXT_LINE 2
#define MSG_PREV_LINE 3
#define MSG_SINGLE_LINE 4
#define MSG_TRIGGER 5

#endif