#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfahw.h"

int main(int argc, char ** argv) 
{
    char *position = (char *) malloc(32);
    memset(position, 0, 32);
    
    if (adxl34xRead(position) > 0) {
        printf("Get position: %s", position);
    } else {
        printf("Fail to get position\n");        
    }
    free(position);
    return 0;
}
