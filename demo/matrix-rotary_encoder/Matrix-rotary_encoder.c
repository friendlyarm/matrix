#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include "libfahw.h"

#define ENCODER_READ_TIMES      (20)
#define DRIVER_MODULE           "matrix_rotary_encoder"

void encoderHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        rotaryEncoderDeInit();
        system("rmmod "DRIVER_MODULE);
    }
    exit(0);
}

int main(int argc, char ** argv) 
{ 
    int i = 0;
    int encoderSw = 0;
    int encoderValue = 0;
    int board;
    int swPin = GPIO_PIN(7);
    int siaPin = GPIO_PIN(8);
    int sibPin = GPIO_PIN(10);

    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    if (board == BOARD_NANOPC_T2 || board == BOARD_NANOPC_T3) {
        swPin = GPIO_PIN(15);
        siaPin = GPIO_PIN(16);
        sibPin = GPIO_PIN(17);
    }
    system("modprobe "DRIVER_MODULE);
    if (rotaryEncoderInit(swPin, siaPin, sibPin)) {
        printf("Fail to init rotary encoder\n");
        goto err;
    }
    signal(SIGINT, encoderHandler);
    for (i=0; i<ENCODER_READ_TIMES; i++) {
        rotaryEncoderRead(ENCODER_SW, &encoderSw);
        rotaryEncoderRead(ENCODER_VALUE, &encoderValue);
        printf("Get sw=%d value=%d\n", encoderSw, encoderValue);
        sleep(1);
    }
    rotaryEncoderDeInit();
err:
    system("rmmod "DRIVER_MODULE);
    return 0;
}
