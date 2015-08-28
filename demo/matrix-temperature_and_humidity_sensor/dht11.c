#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

void parseCmd(int argc, char **argv, int *pin)
{
    int num = atoi(argv[1]);
    switch(num) {
    case 1:
        *pin = GPIO_PIN1; 
        break;
    case 2:
        *pin = GPIO_PIN2;
        break;
    case 3:
        *pin = GPIO_PIN3;
        break;
    case 4:
        *pin = GPIO_PIN4;
        break;
    case 5:
        *pin = GPIO_PIN5;
        break;
    default:
        printf("Unsupported pin GPIO_PIN%d\n", num);
        num = 1;
        *pin = GPIO_PIN1;
    }
    printf("Using pin GPIO_PIN%d\n", num);
}

int main(int argc, char ** argv)
{
    int ret = -1;
    int dhtTemp = 0;
    int dhtHdty = 0;
    int devFD = -1;
    int pin = GPIO_PIN1;

    if (argc == 2) {
        parseCmd(argc, argv, &pin);
    } else {
        printf("Using default pin GPIO_PIN1\n");
    }
    if ((devFD = dht11Init(pin)) == -1) {
        printf("Fail to init dht11\n");
        return -1;
    }

    if ((ret = dht11Read(DHT_HUMIDITY, &dhtHdty)) != -1) {
        printf("Get humidity : %d\n", dhtHdty);
    } else {
        printf("Faided to get humidity\n");
    }

    if ((ret = dht11Read(DHT_TEMP, &dhtTemp)) != -1) {
        printf("Get temperature : %d\n", dhtTemp);
    } else {
        printf("Faided to get temperature\n");
    }

    dht11DeInit(devFD);
    return ret;
}
