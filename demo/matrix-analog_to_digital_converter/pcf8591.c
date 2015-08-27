#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "libfahw.h"

int main(int argc, char ** argv)
{
    int devFD;
    int data, channel, mode;

    if ((devFD = pcf8591Init()) == -1) {
        printf("Fail to init pcf8591\n");
        return -1;
    }

    if (pcf8591SetCtrl(devFD, PCF8591_INIT_AD_CONTROL) == -1) {
        printf("Fail to Set pcf8591 control AD\n");
        pcf8591DeInit(devFD);
        return -1;
    }
    
    mode = 0;
    printf("pcf8591 working as AD in mode%d\n",mode);
    for (channel = PCF8591_AIN_CHANNEL0; channel <= PCF8591_AIN_CHANNEL3; channel++) {
        data = pcf8591Read(devFD, mode, channel);
        printf("Channel%d's value: %d\n",channel,data);
    }
    
    pcf8591DeInit(devFD);
    return 0;
}
