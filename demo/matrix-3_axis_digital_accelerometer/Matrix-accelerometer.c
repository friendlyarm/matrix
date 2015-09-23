#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfahw.h"

#define BUF_SIZE 32

int main(int argc, char ** argv) 
{
    char position[BUF_SIZE];
    memset(position, 0, BUF_SIZE);
    
    if (adxl34xRead(position) > 0) {
        printf("Get position: %s", position);
    } else {
        printf("Fail to get position\n");        
    }
    return 0;
}
