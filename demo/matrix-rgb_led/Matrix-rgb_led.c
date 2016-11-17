#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "libfahw.h"

#define STATUS_CHANGE_TIMES     (5)
static int ledPin1 = GPIO_PIN(7);
static int ledPin2 = GPIO_PIN(8);
static int ledPin3 = GPIO_PIN(10);

void intHandler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Clean up\n");
        unexportGPIOPin(ledPin1);
        unexportGPIOPin(ledPin2);
        unexportGPIOPin(ledPin3);
    }
    exit(0);
}

void setRGBLED(int data)
{
    if (data & 0x01) {
        setGPIOValue(ledPin1, GPIO_HIGH);
    } else {
        setGPIOValue(ledPin1, GPIO_LOW);
    }
    if (data & 0x02) {
        setGPIOValue(ledPin2, GPIO_HIGH);
    } else {
        setGPIOValue(ledPin2, GPIO_LOW);
    }
    if (data & 0x04) {
        setGPIOValue(ledPin3, GPIO_HIGH);
    } else {
        setGPIOValue(ledPin3 ,GPIO_LOW);
    }
}

int main(int argc, char ** argv)
{
    int ret = -1;
    int val, board;

    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    if (board == BOARD_NANOPC_T2 || BOARD_NANOPC_T3) {
        ledPin1 = GPIO_PIN(15);
        ledPin2 = GPIO_PIN(16);
        ledPin3 = GPIO_PIN(17);
    }
    if ((ret = exportGPIOPin(ledPin1)) == -1) {
        printf("exportGPIOPin(%d) failed\n", ledPin1);
    }
    if ((ret = setGPIODirection(ledPin1, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", ledPin1);
    }
    if ((ret = exportGPIOPin(ledPin2)) == -1) {
        printf("exportGPIOPin(%d) failed\n", ledPin2);
    }
    if ((ret = setGPIODirection(ledPin2, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", ledPin2);
    }
    if ((ret = exportGPIOPin(ledPin3)) == -1) {
        printf("exportGPIOPin(%d) failed\n", ledPin3);
    }
    if ((ret = setGPIODirection(ledPin3, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", ledPin3);
    }
    signal(SIGINT, intHandler);
    for (val = 0; val < 8; val++) {
        printf("Set RGB LED: %x\n", val);
        setRGBLED(val);
        usleep(1000 * 1000);
    }
    unexportGPIOPin(ledPin1);
    unexportGPIOPin(ledPin2);
    unexportGPIOPin(ledPin3);
    return 0;
}
