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
    int keyValue = 0;
    char keyStr[8];
    if ((devFD = LCD1602KeyInit()) == -1) {
        printf("Fail to init LCD1602\n");
        return -1;
    }

    if (LCD1602KeyClear(devFD) == -1) {
        printf("Fail to Clear\n");
        return -1;
    }
    printf("clearing LCD1602\n");
    sleep(1);
    
    if (LCD1602KeyDispLines(devFD, "FriendlyARM", "NanoPi & Matrix") == -1) {
        printf("Fail to Display String\n");
        return -1;
    }
    printf("displaying LCD1602\n");
    sleep(1);
    
    printf("waiting key press\n");
    LCD1602KeyClear(devFD);
    LCD1602KeyDispStr(devFD, 0, 0, "key value:");
    while (1) {
        keyValue = LCD1602GetKey(devFD);
        if (keyValue == 0xf) {
            keyStr[0] = '0';
            sprintf(&keyStr[1], "%x", keyValue);
        } else {
            sprintf(keyStr, "%x", keyValue);
        }
        LCD1602KeyDispStr(devFD, 0, 1, keyStr);
        usleep(1000);
    }
    printf("quit reading key press\n");
    LCD1602KeyDeInit(devFD);
    return 0;
}









