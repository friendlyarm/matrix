#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int ret = -1;
    int dhtTemp = 0;
    int dhtHdty = 0;

    printf("Using pin GPIO_PIN1\n");
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

    return ret;
}
