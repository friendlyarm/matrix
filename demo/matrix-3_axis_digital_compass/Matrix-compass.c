#include <stdio.h>
#include <stdlib.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int devFD;
    double angle;
    int i2cDev = 0;
    
    if (boardInit() < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    if (argc == 2)
        i2cDev = atoi(argv[1]);
    if ((devFD = hmc5883Init(i2cDev)) == -1) {
        printf("Fail to init hmc5883\n");
        return -1;
    }
    if ((angle = hmc5883Read(devFD)) != -1) {
        printf("The angle is %.1f\n", angle);
    } else {
        printf("Fail to read hmc5883\n");
    }
    hmc5883DeInit(devFD);
    
    return 0;
}
