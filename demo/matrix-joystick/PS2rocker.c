#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "libfahw.h"

#define SW_TRIGGER          (5)
#define PS2_READ_TIMES      (10)

static int devFD;
void PS2Handler(int signNum)
{
    if (signNum == SIGINT) {
        printf("Quit reading PS2 rocker\n");
        pcf8591DeInit(devFD);
    }
    exit(0);
}

int main(int argc, char ** argv)
{
	int mode = 0x0;

	if ((devFD = pcf8591Init()) == -1) {
        printf("Fail to init pcf8591 AD\n");
        return -1;
    }
    if (pcf8591SetCtrl(devFD, PCF8591_INIT_AD_CONTROL) == -1) {
        printf("Fail to Set pcf8591 control AD\n");
        pcf8591DeInit(devFD);
        return -1;
    }

    int i = 0;
	int x, y, z;
	signal(SIGINT, PS2Handler);
	for (i=0; i<PS2_READ_TIMES; i++) {
		x = pcf8591Read(devFD, mode, PCF8591_AIN_CHANNEL0);
		y = pcf8591Read(devFD, mode, PCF8591_AIN_CHANNEL1);
		z = pcf8591Read(devFD, mode, PCF8591_AIN_CHANNEL2);
		if (z > SW_TRIGGER) {
			z = 0;
		} else {
			z = 1;
		}
		printf("X=%3d Y=%3d Z=%d\n", x, y, z);
		sleep(1);
	}
	pcf8591DeInit(devFD);
	return 0;
}
