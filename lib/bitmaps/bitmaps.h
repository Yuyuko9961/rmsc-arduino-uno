#ifndef bitmaps_h
#define bitmaps_h

#include <Arduino.h>

#define PAGE1TITLE_LEN 78
extern const uint8_t page1Title[PAGE1TITLE_LEN] PROGMEM;

#define STORAGESTAT_LEN 10
#define SWITCHSTAT_LEN 20
extern const uint8_t storageAvailable[STORAGESTAT_LEN] PROGMEM;
extern const uint8_t switchOff[SWITCHSTAT_LEN] PROGMEM;
extern const uint8_t switchOn[SWITCHSTAT_LEN] PROGMEM;

#define LOGO_LEN 256
extern const uint8_t programLogo[LOGO_LEN] PROGMEM;

#define ANIMATION_FRAME_NUM 32
#define ANIMATION_FRAME_LEN 128
extern const uint8_t openingAnimation[ANIMATION_FRAME_NUM][ANIMATION_FRAME_LEN] PROGMEM;

#endif