#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "libfahw-gpio.h"
#include "libfahw-encoder.h"

EXPORT int rotaryEncoderInit(int swPin, int siaPin, int sibPin) 
{
    clearLastError();
    char buf[32];
    
    sprintf(buf,"%d,%d,%d\n", pintoGPIO(swPin), pintoGPIO(siaPin), pintoGPIO(sibPin));
    if (writeValueToFile(ENCODER_PATH"gpio", buf) <= 0) {
        setLastError("Fail to set pin for rotary encoder");
        return -1;
    }
    return 0;
}

EXPORT int rotaryEncoderRead(int type, int *data) 
{
    clearLastError();
    int ret = -1;
    char *encoderPath = (char *) malloc(FILE_PATH_LENGTH);
    memset(encoderPath, 0, FILE_PATH_LENGTH);
    strcpy(encoderPath, ENCODER_PATH);

    switch(type) {
    case ENCODER_SW:
        strcat(encoderPath, "sw");
        ret = readIntValueFromFile(encoderPath);
        break;
    case ENCODER_VALUE:
        strcat(encoderPath, "value");
        ret = readIntValueFromFile(encoderPath);
        break;
    default:
        setLastError("Unsupport encoder data type %d", type);
        break;
    }
    *data = ret;
    free(encoderPath);
    return ret;
}

EXPORT int rotaryEncoderDeInit() 
{
    clearLastError();
    char buf[32];
    
    sprintf(buf,"%d,%d,%d\n", -1, -1, -1);
    if (writeValueToFile(ENCODER_PATH"gpio", buf) <= 0) {
        setLastError("Fail to unset pin for rotary encoder");
        return -1;
    }
    return 0;
}
