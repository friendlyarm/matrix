#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfahw.h"

#define BUF_SIZE    (32)
#define ADC_READ_TIMES (1000)

int main(int argc, char ** argv)
{
    int i = 0;
    int value = 0;
    int channel = 0;

    if (argc == 2) {
        channel = atoi(argv[1]);
    }
    
    for (i=0; i<ADC_READ_TIMES; i++) {
        if (pcf8591Read(channel, &value) != -1) {
            printf("channel%d value=%d\n", channel, value);
        } else {
            printf("Fail to get channel%d value\n", channel);        
        }
        usleep(10000);
    }
    return 0;
}
