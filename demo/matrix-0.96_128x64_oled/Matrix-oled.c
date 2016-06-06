#include <stdio.h>
#include "libfahw.h"

int main(int argc, char *argv[]) 
{
    int devFD;
    
    if (boardInit() < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    if ((devFD = OLEDInit(GPIO_PIN(7), GPIO_PIN(11))) == -1) {
        printf("Fail to init OLED\n");
        return -1;
    }
    OLEDCleanScreen(devFD);
    // Char bitmap: 8x16
    OLEDDisp8x16Str(devFD, 0, 0, "ABCDEFGHIJKLMN");
    OLEDDisp8x16Str(devFD, 0, 16, "123456789");
    OLEDDeInit(devFD);
    
    return 0;
}
