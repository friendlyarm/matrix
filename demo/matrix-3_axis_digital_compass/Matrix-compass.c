#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int devFD;
    double angle;
    int i2cDev = 0;
    
    if (boardInit() < 0)
        printf("Fail to init board\n");
    
    if (argc == 2)
        i2cDev = atoi(argv[1]);
    
    if ((devFD = hmc5883Init(i2cDev)) == -1) {
        printf("Fail to init hmc5883\n");
        return -1;
    }

    if ((angle = hmc5883Read(devFD)) == -1) {
        printf("Fail to read hmc5883\n");
        hmc5883DeInit(devFD);
        return -1;
    }
    printf("The angle is %.1f\n", angle);
    printf("You are heading ");
    if((angle < 22.5) || (angle > 337.5 )) {           
        printf("South\n");
    }
    else if((angle > 22.5) && (angle < 67.5 )) {
        printf("South-West\n");
    }
    else if((angle > 67.5) && (angle < 112.5 )) {  
        printf("West\n");
    }
    else if((angle > 112.5) && (angle < 157.5 )) {  
        printf("North-West\n");
    }
    else if((angle > 157.5) && (angle < 202.5 )) {   
        printf("North\n");
    }
    else if((angle > 202.5) && (angle < 247.5 )) {  
        printf("NorthEast\n");
    }
    else if((angle > 247.5) && (angle < 292.5 )) {  
        printf("East\n");
    }
    else if((angle > 292.5) && (angle < 337.5 )) {   
        printf("SouthEast\n");
    }
    hmc5883DeInit(devFD);
    return 0;
}
