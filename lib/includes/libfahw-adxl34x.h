#ifndef __FRIENDLYARM_HARDWARE_ADXL34X_H__
#define __FRIENDLYARM_HARDWARE_ADXL34X_H__

#define ADXL34X_SYS_PATH    "/sys/bus/i2c/drivers/adxl34x/"

int adxl34xRead(char *position);

#endif
