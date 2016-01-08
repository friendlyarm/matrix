#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include "libfahw.h"

#define ENCODER_READ_TIMES      (100)

void encoderHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Quit reading rotary encoder\n");
        rotaryEncoderDeInit();
    }
    exit(0);
}

int main(int argc, char ** argv) 
{ 
    int i = 0;
    int encoderSw = 0;
    int encoderValue = 0;
    int swPin = GPIO_PIN(7);
    int siaPin = GPIO_PIN(11);
    int sibPin = GPIO_PIN(12);

    if (rotaryEncoderInit(swPin, siaPin, sibPin)) {
        printf("Fail to init rotary encoder\n");
        return -1;
    }

    signal(SIGINT, encoderHandler);
    for (i=0; i<ENCODER_READ_TIMES; i++) {
        rotaryEncoderRead(ENCODER_SW, &encoderSw);
        rotaryEncoderRead(ENCODER_VALUE, &encoderValue);
        printf("Get sw=%d value=%d\n", encoderSw, encoderValue);
        usleep(100000);
    }
    rotaryEncoderDeInit();
    return 0;
}
