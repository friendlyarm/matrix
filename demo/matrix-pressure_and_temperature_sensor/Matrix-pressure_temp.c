#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libfahw.h"

int main(int argc, char ** argv) 
{ 
    int ret = -1;
    int bmpTemp = 0;
    int bmpPressure = 0;
    float altitude = 0;
    
    boardInit();
    if ((ret = bmp180Read(BMP180_TEMP, &bmpTemp)) != -1) {
        printf("Get temperature : %.1f C\n", (float)bmpTemp / 10);
    } else {
        printf("Faided to get humidity\n");
    }

    if ((ret = bmp180Read(BMP180_PRESSURE, &bmpPressure)) != -1) {
        printf("Get pressure : %.2f hPa\n", (float)bmpPressure / 100);
    } else {
        printf("Faided to get pressure\n");
    }

    altitude = 44330 * ( 1 - pow( ((float)bmpPressure / 100 / 1013.25), (1/5.255) ) );
    printf("Get altitude : %.2f m\n", altitude);
    return 0;
}
