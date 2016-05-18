#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int devFD;
    int i2cDev = 0;
    
    if (argc == 2) {
        i2cDev = atoi(argv[1]);
    }
    
    boardInit();
    if ((devFD = LCD1602Init(i2cDev)) == -1) {
        printf("Fail to init LCD1602\n");
        return -1;
    }

    if (LCD1602Clear(devFD) == -1) {
        printf("Fail to Clear\n");
        return -1;
    }
    printf("clearing LCD1602\n");
    sleep(1);
    
    if (LCD1602DispLines(devFD, "  B&G Char LCD", "--by FriendlyARM") == -1) {
        printf("Fail to Display String\n");
        return -1;
    }
    printf("displaying LCD1602\n");
    LCD1602DeInit(devFD);
    return 0;
}
