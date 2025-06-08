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

#endif