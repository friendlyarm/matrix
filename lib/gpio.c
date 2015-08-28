#include "common.h"
#include "libfahw-gpio.h"

#define GPIO_FILENAME_DEFINE(pin,field) char fileName[255] = {0}; \
        sprintf(fileName, "/sys/devices/virtual/gpio/gpio%d/%s", pin, field);

/*
pin:
cd /sys/class/gpio
/sys/class/gpio # for i in gpiochip* ; do echo `cat $i/label`: `cat $i/base` ; done

nanopi display:
GPIOA0: 0
GPIOB0: 32
GPIOC0: 64
GPIOD0: 96
GPIOE0: 128
GPIOF0: 160
GPIOG0: 192
GPIOH0: 224
GPIOJ0: 256
GPIOK0: 288
GPIOL0: 320
GPIOM0: 352

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
    if (pin == GPIO_PIN11) {
        return 1;
    }
    if (pin == GPIO_PIN12) {
        return 1;
    }
    if (pin == GPIO_PIN13) {
        return 1;
    }
    if (pin == GPIO_PIN14) {
        return 1;
    }
    if (pin == GPIO_PIN15) {
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
