#ifndef __FRIENDLYARM_HARDWARE_PCF8591_H__
#define __FRIENDLYARM_HARDWARE_PCF8591_H__

#define PCF8591_SYS_PATH    "/sys/bus/i2c/drivers/pcf8591/"

int pcf8591Read(int channel, int *value);

#endif
