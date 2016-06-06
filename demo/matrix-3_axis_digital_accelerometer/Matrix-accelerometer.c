#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfahw.h"

#define BUF_SIZE 32

int main(int argc, char ** argv) 
{
    char position[BUF_SIZE];
    int board;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    system("modprobe adxl34x");
    system("modprobe adxl34x-i2c");
    memset(position, 0, BUF_SIZE);
    if (adxl34xRead(position) > 0) {
        printf("The position is %s", position);
    } else {
        printf("Fail to get position\n");        
    }
    system("rmmod adxl34x-i2c");
    system("rmmod adxl34x");
    
    return 0;
}
