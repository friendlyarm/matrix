#ifndef __FRIENDLYARM_HARDWARE_OLED_H__
#define __FRIENDLYARM_HARDWARE_OLED_H__

#define  ARRAY_SIZE(a)          (sizeof(a) / sizeof((a)[0]))
#define OLED_WIDTH              (132)
#define OLED_HEIGHT             (64)

int OLEDDisp8x16Str(int devFD, int x, int y, char ch[]);
int OLEDDisp8x16Char(int devFD, int x, int y, char ch);
int OLEDCleanScreen(int devFD);
int OLEDInit(int resetPin, int cmdDatPin);
void OLEDDeInit(int devFD);

#endif
