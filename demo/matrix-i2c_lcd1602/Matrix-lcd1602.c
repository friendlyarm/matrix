#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int devFD, board;
    int i2cDev = 0;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    if (argc == 2)
        i2cDev = atoi(argv[1]);
    if ((devFD = LCD1602Init(i2cDev)) == -1) {
        printf("Fail to init LCD1602\n");
        return -1;
    }
    if (LCD1602Clear(devFD) == -1) {
        printf("Fail to Clear\n");
    }
    printf("clearing LCD1602\n");
    sleep(1);
    if (LCD1602DispLines(devFD, "  B&G Char LCD", "--by FriendlyARM") == -1) {
        printf("Fail to Display String\n");
    }
    printf("displaying LCD1602\n");
    LCD1602DeInit(devFD);
    return 0;
}
