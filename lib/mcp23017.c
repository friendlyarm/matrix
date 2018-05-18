#include <stdio.h>
#include "libfahw-mcp23017.h"
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
    usleep(1000);
    if (I2CWriteByteTo(devFD, REG_GPIOA, (data | En), 0) == -1) {
        return -1;
    }
    usleep(1000);
    if (I2CWriteByteTo(devFD, REG_GPIOA, (data & ~En), 0) == -1) {
        return -1;
    }
    return 0;
}

// 写四位指令
EXPORT int mcpWriteCmd4(int devFD, unsigned char command)
{
    clearLastError();

    if (I2CWriteByteTo(devFD, REG_GPIOA, command | BL, 0) == -1) {
        return -1;
    }
    if (pulseEnable(devFD, command | BL) == -1) {
        setLastError("Fail to pulseEnable");
        return -1;
    }
    return 0;
}

// 写八位指令
EXPORT int mcpWriteCmd8(int devFD, unsigned char command)
{
    clearLastError();
    unsigned char command_H = command & 0xf0;
    unsigned char command_L = (command << 4) & 0xf0;
    if (mcpWriteCmd4(devFD, command_H) == -1) {
        setLastError("Fail to write cmd high 4bit");
        return -1;
    }

    if (mcpWriteCmd4(devFD, command_L) == -1) {
        setLastError("Fail to write cmd low 4bit");
        return -1;
    }
    return 0;
}

// 写四位数据
EXPORT int mcpWriteData4(int devFD, unsigned char data)
{
    clearLastError();
    if (I2CWriteByteTo(devFD, REG_GPIOA, (data | RS | BL), 0) == -1) {
        return -1;
    }
    if (pulseEnable(devFD, (data | RS | BL)) == -1) {
        return -1;
    }
    return 0;
}

// 写八位数据
EXPORT int mcpWriteData8(int devFD, unsigned char data)
{
    clearLastError();
    unsigned char data_H = data & 0xf0;
    unsigned char data_L = (data << 4) & 0xf0;
    if (mcpWriteData4(devFD, data_H) == -1) {
        setLastError("Fail to write dat low 4bit");
        return -1;
    }
    if (mcpWriteData4(devFD, data_L) == -1) {
        setLastError("Fail to write dat low 4bit");
        return -1;
    }
    return 0;
}

EXPORT int mcpInit(int i2cDev) 
{
    clearLastError();
    int devFD;
    char buf[16];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "/dev/i2c-%d", i2cDev);
    if ((devFD = openHW(buf, O_RDWR)) < 0) {
        setLastError("Fail to open I2C device mcp");
        return -1;
    } else {
        if (setI2CSlave(devFD, MCP23017_ADDRESS) < 0) {
            setLastError("Fail to set mcp I2C slave address");
            closeHW(devFD);
            return -1;
        }
    } 
    I2CWriteByteTo(devFD, REG_IOCON, 0x20, 0);  // set not auto 
    I2CWriteByteTo(devFD, REG_IODIRA, 0x0, 0);  // set GPA output
    I2CWriteByteTo(devFD, REG_IODIRB, 0xFF, 0); // set GPB input
    
    return devFD;
}

EXPORT void mcpDeInit(int devFD) 
{
    clearLastError();
    closeHW(devFD);
}

// 在第y行第x个显示字符
EXPORT int LCD1602KeyDispChar(int devFD, unsigned char x, unsigned char y, unsigned char data)
{
    clearLastError();
    unsigned char addr;
    if (y == 0)
        addr = 0x80 + x;
    else
        addr = 0xc0 + x;

    if (mcpWriteCmd8(devFD, addr) == -1) {
        setLastError("Fail to write cmd 8bit");
        return -1;
    }
    if (mcpWriteData8(devFD, data) == -1) {
        setLastError("Fail to write data 8bit");
        return -1;
    }

    return 0;
}

// 在第y行第x个开始写字符串
EXPORT int LCD1602KeyDispStr(int devFD, unsigned char x, unsigned char y, char *str)
{
    clearLastError();
    unsigned char addr;
    addr = ((y + 2) * 0x40) + x;

    if (mcpWriteCmd8(devFD, addr) == -1) {
        setLastError("Fail to write cmd 8bit");
        return -1;
    }
    while (*str != '\0') {
        if (mcpWriteData8(devFD, *str++) == -1) {
            setLastError("Fail to write data 8bit");
            return -1;
        }
    }

    return 0;
}

EXPORT int LCD1602KeyDispLines(int devFD, char* line1, char* line2) {
    int ret = LCD1602KeyDispStr(devFD, 0, 0, line1);
    if (ret != -1) {
        ret = LCD1602KeyDispStr(devFD, 0, 1, line2);
    }
    return ret;
}

EXPORT int LCD1602KeyInit(int i2cDev)
{
    clearLastError();
    int devFD;
    if ((devFD = mcpInit(i2cDev)) == -1) {
        setLastError("Fail to init mcp23017");
        return -1;
    }

    usleep(1000*15);
    if (mcpWriteCmd4(devFD, 0x03 << 4) == -1) {
        return -1;
    }
    usleep(100*41);
    if (mcpWriteCmd4(devFD, 0x03 << 4) == -1) {
        return -1;
    }
    usleep(100);
    if (mcpWriteCmd4(devFD, 0x03 << 4) == -1) {    // 8位数据总线
        return -1;
    }
    if (mcpWriteCmd4(devFD, 0x02 << 4) == -1) {    // 4位数据总线
        return -1;
    }    
    if (mcpWriteCmd8(devFD, 0x28) == -1) {         // 4位数据总线，显示2行，5x7
        return -1;
    }
    if (mcpWriteCmd8(devFD, 0x0c) == -1) {         // 显示开，无光标
        return -1;
    }
    usleep(2000);
    if (mcpWriteCmd8(devFD, 0x06) == -1) {         // 光标右移，显示屏不移动
        return -1;
    }
    if (mcpWriteCmd8(devFD, 0x02) == -1) {         // 光标复位
        return -1;
    }
    usleep(2000);

    return devFD;
}

EXPORT int LCD1602KeyClear(int devFD) {
    clearLastError();
    if (mcpWriteCmd8(devFD, 0x01) == -1) { 
        setLastError("Fail to mcp write cmd 0x01");
        return -1;
    }
    return 0;
}

EXPORT void LCD1602KeyDeInit(int devFD)
{
    clearLastError();
    closeHW(devFD);    
}

EXPORT int LCD1602GetKey(int devFD)
{
    clearLastError();
    return I2CReadByteFrom(devFD, REG_GPIOB, 0);
}
