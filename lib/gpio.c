#include "common.h"
#include "libfahw-gpio.h"

static int pinGPIO[41] = {-1, -1, -1, 99, -1, 98, -1,  60, 117, -1, 113,
                              61, 58, 62, -1, 63, 78,  -1,  59, 95, -1,
                              96, 97, 93, 94, -1, 77, 103, 102, 72, -1,
                              73, 92, 74, -1, 76, 71,  75, 162, -1, 163,
                          };

void initPinGPIO(int board)
{
    clearLastError();
    switch(board) {
    case BOARD_NANOPI_M1: {
        // i2c & spi do not use as gpio
        int tempPinGPIO[41] = {-1, -1, -1, -1, -1, -1, -1, 203, 198, -1, 199,
                                   0, 6, 2, -1, 3, 200,  -1, 201, -1, -1,
                                   -1, 1, -1, -1, -1, -1, -1, -1, 20, -1,
                                   21, 7, 8, -1, -1, -1, 9, -1, -1, -1,
                                  };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        break;
    }
    default:
        break;
    }
}

EXPORT int pintoGPIO(int pin)
{
    clearLastError();

    if (pin<1 || pin>40 || pinGPIO[pin]==-1) {
        setLastError("invalid pin %d, it may be 5V/3.3V/GND or occupied by kernel?", pin);
        return -1;
    }
    return pinGPIO[pin];
}

EXPORT int exportGPIOPin(int pin) 
{
    clearLastError();
    int gpio = pintoGPIO(pin);
    
    return writeIntValueToFile("/sys/class/gpio/export", gpio);
}

EXPORT int unexportGPIOPin(int pin) 
{
    clearLastError();
    int gpio = pintoGPIO(pin);
    
    return writeIntValueToFile("/sys/class/gpio/unexport", gpio);
}

EXPORT int getGPIOValue(int pin) 
{
    clearLastError();
    int gpio = pintoGPIO(pin);
    GPIO_FILENAME_DEFINE(gpio, "value")
    
    return readIntValueFromFile(fileName);
}

EXPORT int setGPIOValue(int pin, int value) 
{
    clearLastError();
    int gpio = pintoGPIO(pin);
    GPIO_FILENAME_DEFINE(gpio, "value")
    
    return writeIntValueToFile(fileName, value);
}

EXPORT int setGPIODirection(int pin, int direction) 
{
    clearLastError();
    int gpio = pintoGPIO(pin);
    char directionStr[10];
    GPIO_FILENAME_DEFINE(gpio, "direction")
    
    if (direction == GPIO_IN) {
        strcpy(directionStr, "in");
    } else if (direction == GPIO_OUT) {
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
    char buff[255] = {0};
    int direction;
    int ret;
    int gpio = pintoGPIO(pin);
    GPIO_FILENAME_DEFINE(gpio, "direction")
    
    ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
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
