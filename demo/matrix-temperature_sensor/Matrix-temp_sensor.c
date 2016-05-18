#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfahw.h"

#define BUF_SIZE 32

int main(int argc, char ** argv)
{
    char temperature[BUF_SIZE];
    
    boardInit();
    memset(temperature, 0, BUF_SIZE);
    if (ds18b20Read(temperature) > 0) {
        printf("Temperature = %.3f C\n", atoi(temperature)/1000.0);
    } else {
        printf("Fail to get temperature\n");		
    }
    return 0;
}
