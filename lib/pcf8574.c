#include <stdio.h>
#include "libfahw-pcf8574.h"
#include "libfahw-filectl.h"
#include "libfahw-i2c.h"
#include "includes/i2c-dev.h"
#include "common.h"

static unsigned char  RS = 0x01;
//static unsigned char  RW = 0x02;
static unsigned char  En = 0x04;
static unsigned char  BL = 0x08;

static int pulseEnable(int devFD, unsigned char data)
{
    clearLastError();
    if (I2CWriteByte(devFD, (data | En), 0) == -1) {
        return -1;
    }
    usleep(1);
    if (I2CWriteByte(devFD, (data & ~En), 0) == -1) {
        return -1;
    }
    return 0;
}

// 写四位指令
EXPORT int pcf8574WriteCmd4(int devFD, unsigned char command)
{
    clearLastError();
    if (I2CWriteByte(devFD, command | BL, 0) == -1) {
        return -1;
    }
    if (pulseEnable(devFD, command | BL) == -1) {
        setLastError("Fail to pulseEnable");
        return -1;
    }
    return 0;
}

// 写八位指令
EXPORT int pcf8574WriteCmd8(int devFD, unsigned char command)
{
    clearLastError();
    unsigned char command_H = command & 0xf0;
    unsigned char command_L = (command << 4) & 0xf0;
    if (pcf8574WriteCmd4(devFD, command_H) == -1) {
        setLastError("Fail to write cmd high 4bit");
        return -1;
    }

    if (pcf8574WriteCmd4(devFD, command_L) == -1) {
        setLastError("Fail to write cmd low 4bit");
        return -1;
    }
    return 0;
}

// 写四位数据
EXPORT int pcf8574WriteData4(int devFD, unsigned char data)
{
    clearLastError();
    if (I2CWriteByte(devFD, (data | RS | BL), 0) == -1) {
        return -1;
    }
    if (pulseEnable(devFD, (data | RS | BL)) == -1) {
        return -1;
    }
    return 0;
}

// 写八位数据
EXPORT int pcf8574WriteData8(int devFD, unsigned char data)
{
    clearLastError();
    unsigned char data_H = data & 0xf0;
    unsigned char data_L = (data << 4) & 0xf0;
    if (pcf8574WriteData4(devFD, data_H) == -1) {
        setLastError("Fail to write dat low 4bit");
        return -1;
    }
    if (pcf8574WriteData4(devFD, data_L) == -1) {
        setLastError("Fail to write dat low 4bit");
        return -1;
    }
    return 0;
}

EXPORT int pcf8574Init(int i2cDev) 
{
    clearLastError();
    int devFD;
    char buf[16];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "/dev/i2c-%d", i2cDev);
    if ((devFD = openHW(buf, O_RDWR)) < 0) {
        setLastError("Fail to open I2C device pcf8574");
        return -1;
    } else {
        if (setI2CSlave(devFD, PCF8574_ADDRESS) < 0) {
            setLastError("Fail to set pcf8574 I2C slave address");
            closeHW(devFD);
            return -1;
        }
    } 
    return devFD;
}

EXPORT void pcf8574DeInit(int devFD) 
{
    clearLastError();
    closeHW(devFD);
}

// 在第y行第x个显示字符
EXPORT int LCD1602DispChar(int devFD, unsigned char x, unsigned char y, unsigned char data)
{
    clearLastError();
    unsigned char addr;
    if (y == 0)
        addr = 0x80 + x;
    else
        addr = 0xc0 + x;

    if (pcf8574WriteCmd8(devFD, addr) == -1) {
        setLastError("Fail to write cmd 8bit");
        return -1;
    }
    if (pcf8574WriteData8(devFD, data) == -1) {
        setLastError("Fail to write data 8bit");
        return -1;
    }

    return 0;
}

// 在第y行第x个开始写字符串
EXPORT int LCD1602DispStr(int devFD, unsigned char x, unsigned char y, char *str)
{
    clearLastError();
    unsigned char addr;
    addr = ((y + 2) * 0x40) + x;

    if (pcf8574WriteCmd8(devFD, addr) == -1) {
        setLastError("Fail to write cmd 8bit");
        return -1;
    }
    while (*str != '\0') {
        if (pcf8574WriteData8(devFD, *str++) == -1) {
            setLastError("Fail to write data 8bit");
            return -1;
        }
    }

    return 0;
}

EXPORT int LCD1602DispLines(int devFD, char* line1, char* line2) {
    int ret = LCD1602DispStr(devFD, 0, 0, line1);
    if (ret != -1) {
        ret = LCD1602DispStr(devFD, 0, 1, line2);
    }
    return ret;
}

EXPORT int LCD1602Init(int i2cDev)
{
    clearLastError();
    int devFD;
    if ((devFD = pcf8574Init(i2cDev)) == -1) {
        setLastError("Fail to init pcf8574");
        return -1;
    }
    
    usleep(1000*15);
    if (pcf8574WriteCmd4(devFD, 0x03 << 4) == -1) {
        return -1;
    }
    usleep(100*41);
    if (pcf8574WriteCmd4(devFD, 0x03 << 4) == -1) {
        return -1;
    }
    usleep(100);
    if (pcf8574WriteCmd4(devFD, 0x03 << 4) == -1) {    // 8位数据总线
        return -1;
    }
    if (pcf8574WriteCmd4(devFD, 0x02 << 4) == -1) {    // 4位数据总线
        return -1;
    }    
    if (pcf8574WriteCmd8(devFD, 0x28) == -1) {         // 4位数据总线，显示2行，5x7
        return -1;
    }
    if (pcf8574WriteCmd8(devFD, 0x0c) == -1) {         // 显示开，无光标
        return -1;
    }
    usleep(2000);
    if (pcf8574WriteCmd8(devFD, 0x06) == -1) {         // 光标右移，显示屏不移动
        return -1;
    }
    if (pcf8574WriteCmd8(devFD, 0x02) == -1) {         // 光标复位
        return -1;
    }
    usleep(2000);

    return devFD;
}

EXPORT int LCD1602Clear(int devFD) {
    clearLastError();
    if (pcf8574WriteCmd8(devFD, 0x01) == -1) { 
         setLastError("Fail to pcf8574write cmd 0x01");
         return -1;
    }
    return 0;
}

EXPORT void LCD1602DeInit(int devFD)
{
    clearLastError();
    closeHW(devFD);    
}

