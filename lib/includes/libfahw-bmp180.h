#ifndef __FRIENDLYARM_HARDWARE_BMP180_H__
#define __FRIENDLYARM_HARDWARE_BMP180_H__

#define BMP180_SYS_PATH            "/sys/bus/i2c/drivers/bmp085/" 
#define BMP180_TEMP                (1)
#define BMP180_PRESSURE            (2)

 int bmp180Read(int type, int *data); 
 
#endif
