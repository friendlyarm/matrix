#include "common.h"
#include "libfahw-gpio.h"

static int pinGPIO[41];

int initPinGPIO(int board)
{
    clearLastError();
    int ret = -1;
    
    switch(board) {
    case BOARD_NANOPI_M1: {
        int tempPinGPIO[41] = {-1, -1, -1, -1, -1, -1,  -1, 203, 198, -1, 199,
                                    0,  6,  2, -1,  3, 200,  -1, 201, -1, -1,
                                   -1,  1, -1, -1, -1,  -1,  -1,  -1, 20, -1,
                                   21,  7,  8, -1, 16,  13,   9,  15, -1, 14,
                                  };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        ret = 0;
        break;
    }
    case BOARD_NANOPI_2:{        
        int tempPinGPIO[41] = {-1, -1, -1, 99, -1, 98, -1,  60, 117, -1, 113,
                                   61, 58, 62, -1, 63, 78,  -1,  59, 95, -1,
                                   96, 97, 93, 94, -1, 77, 103, 102, 72, -1,
                                   73, 92, 74, -1, 76, 71,  75, 162, -1, 163,
                                  };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        ret = 0;
        break;
    }
    case BOARD_NANOPI_2_FIRE:
    case BOARD_NANOPI_M2: {
        int tempPinGPIO[41] = {-1, -1, -1, -1, -1, -1, -1, 104, 117, -1, 113,
                                   61, 97, 62, -1, 63, 78,  -1,  59, -1, -1,
                                   -1, 60, -1, -1, -1, 58,  -1,  -1, 72, -1,
                                   71, 92, 77, -1, 75, 74,  163, 76, -1, 73,
                                  };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        ret = 0;
        break;
    }
    case BOARD_NANOPC_T2: {
        int tempPinGPIO[41] = {-1, -1,   -1, 116, 112, -1,  -1,  -1,  -1, -1, -1,
                                  117,  113,  61,  60, 63,  62,  68,  71, 72, 88,
                                   92,   58,  97, 104, 77, 163,  78, 165, -1, -1,
                                   };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        ret = 0;
        break;
    }
    case BOARD_NANOPI_M3: {
        int tempPinGPIO[41] = {-1, -1, -1, -1, -1, -1, -1, 104, 117, -1, 113,
                                   61, 97, 62, -1, 63, 78,  -1,  59, -1, -1,
                                   -1, 60, -1, -1, -1, 58,  -1,  -1, 72, -1,
                                   71, 92, 77, -1, 75, 74,  163, 76, -1, 73,
                                  };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        ret = 0;
        break;
    }
    case BOARD_NANOPC_T3: {
        int tempPinGPIO[41] = {-1, -1,   -1, 116, 112, -1,  -1,  -1,  -1, -1, -1,
                                  117,  113,  61,  60, 63,  62,  68,  71, 72, 88,
                                   92,   58,  97, 104, 77, 163,  78, 165, -1, -1,
                                   };
        memcpy(pinGPIO, tempPinGPIO, sizeof(pinGPIO));
        ret = 0;
        break;
    }
    default:
        ret = -1;
        break;
    }
    
    return ret;
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
    int ret = writeIntValueToFile("/sys/class/gpio/export", gpio);
    if (ret > 0)
        return 0;
    else 
        return -1;
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
