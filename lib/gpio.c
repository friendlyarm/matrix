#include "common.h"
#include "libfahw-gpio.h"

#define GPIO_FILENAME_DEFINE(pin,field) char fileName[255] = {0}; \
        sprintf(fileName, "/sys/devices/virtual/gpio/gpio%d/%s", pin, field);

/*
pin:
cd /sys/class/gpio
/sys/class/gpio # for i in gpiochip* ; do echo `cat $i/label`: `cat $i/base` ; done

4412 display:
GPA0: 0
GPF2: 105
GPF3: 114
GPJ0: 121
GPJ1: 130
GPK0: 136
GPK1: 144
GPK2: 152
GPB: 16
GPK3: 160
GPL0: 168
GPL1: 177
GPL2: 181
GPX0: 190
GPX1: 199
GPX2: 208
GPX3: 217
GPY0: 226
GPY1: 233
GPY2: 238
GPY3: 245
GPC0: 25
GPY4: 254
GPY5: 263
GPY6: 272
GPZ: 281
GPC1: 31
GPD0: 37
GPD1: 42
GPM0: 47
GPM1: 56
GPM2: 64
GPM3: 70
GPM4: 79
GPF0: 87
GPA1: 9
GPF1: 96
 */

static int isPinValid(int pin) 
{
    if (pin == GPIO_PIN1) {
        return 1;
    }
    if (pin == GPIO_PIN2) {
        return 1;
    }
    if (pin == GPIO_PIN3) {
        return 1;
    }
    if (pin == GPIO_PIN4) {
        return 1;
    }
    if (pin == GPIO_PIN5) {
        return 1;
    }
    if (pin == GPIO_PIN6) {
        return 1;
    }
    if (pin == GPIO_PIN7) {
        return 1;
    }
    if (pin == GPIO_PIN8) {
        return 1;
    }
    if (pin == GPIO_PIN9) {
        return 1;
    }
    if (pin == GPIO_PIN10) {
        return 1;
    }
    return 0;
}

EXPORT int exportGPIOPin(int pin) 
{
    clearLastError();
    return writeIntValueToFile("/sys/class/gpio/export", pin);
}

EXPORT int unexportGPIOPin(int pin) 
{
    clearLastError();
    return writeIntValueToFile("/sys/class/gpio/unexport", pin);
}

EXPORT int getGPIOValue(int pin) 
{
    clearLastError();
    GPIO_FILENAME_DEFINE(pin, "value")
    return readIntValueFromFile(fileName);
}

EXPORT int setGPIOValue(int pin, int value) 
{
    clearLastError();
    GPIO_FILENAME_DEFINE(pin, "value")
    return writeIntValueToFile(fileName, value);
}

EXPORT int setGPIODirection(int pin, int direction) 
{
    clearLastError();
    GPIO_FILENAME_DEFINE(pin, "direction")
    char directionStr[10];
    if (direction == GPIO_IN) {
        if (!isPinValid(pin)) {
            setLastError("invalid pin %d", pin);
            return -1;
        }
        strcpy(directionStr, "in");
    } else if (direction == GPIO_OUT) {
        if (!isPinValid(pin)) {
            setLastError("invalid pin %d", pin);
            return -1;
        }
        strcpy(directionStr, "out");
    } else {
        setLastError("direction must be 1 or 2,  1->in, 2->out");
        return -1;
    }
    return writeValueToFile(fileName, directionStr);
}

EXPORT int getGPIODirection(int pin) 
{
    clearLastError();
    GPIO_FILENAME_DEFINE(pin, "direction")
    char buff[255] = {0};
    int direction;
    int ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
    if (ret >= 0) {
        if (strncasecmp(buff, "out", 3)==0) {
            direction = GPIO_OUT;
        } else if (strncasecmp(buff, "in", 2)==0) {
            direction = GPIO_IN;
        } else {
            setLastError("direction wrong, must be in or out,  but %s", buff);
            return -1;
        }
        return direction;
    }
    return ret;
}
