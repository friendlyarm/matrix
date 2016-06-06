#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libfahw.h"

#define DRIVER_MODULE "bmp085"

int main(int argc, char ** argv) 
{ 
    int ret = -1;
    int bmpTemp=0, bmpPressure=0;
    int board;
    float altitude = 0;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    system("modprobe "DRIVER_MODULE);
    if ((ret = bmp180Read(BMP180_TEMP, &bmpTemp)) != -1) {
        printf("The temperature is %.1f C\n", (float)bmpTemp / 10);
    } else {
        printf("Faided to get humidity\n");
    }
    if ((ret = bmp180Read(BMP180_PRESSURE, &bmpPressure)) != -1) {
        printf("The pressure is %.2f hPa\n", (float)bmpPressure / 100);
    } else {
        printf("Faided to get pressure\n");
    }

    altitude = 44330 * ( 1 - pow( ((float)bmpPressure / 100 / 1013.25), (1/5.255) ) );
    printf("The altitude is %.2f m\n", altitude);
    system("rmmod "DRIVER_MODULE);
    return 0;
}
