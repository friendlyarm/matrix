#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

#define BUF_SIZE            (64)
#define DRIVER_MODULE       "dht11"

int main(int argc, char ** argv)
{
    int ret = -1;
    int dhtTemp=0, dhtHdty=0, board;
    char modStr[BUF_SIZE];
    int pin = GPIO_PIN(7);

    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }    
    if (board == BOARD_NANOPC_T2 || board == BOARD_NANOPC_T3)
        pin = GPIO_PIN(15);
    
    sprintf(modStr, "modprobe %s gpio=%d", DRIVER_MODULE, pintoGPIO(pin));
    system(modStr);
    if ((ret = dht11Read(DHT_HUMIDITY, &dhtHdty)) != -1) {
        printf("The humidity is %d\n", dhtHdty);
    } else {
        printf("Faided to get humidity\n");
    }
    if ((ret = dht11Read(DHT_TEMP, &dhtTemp)) != -1) {
        printf("The temperature is %d\n", dhtTemp);
    } else {
        printf("Faided to get temperature\n");
    }
    system("rmmod "DRIVER_MODULE);
    return ret;
}
