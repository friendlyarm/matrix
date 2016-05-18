#include <math.h>
#include "common.h"
#include "libfahw-i2c.h"
#include "libfahw-filectl.h"
#include "libfahw-hmc5883.h"

static int hmc5883HWInit(int devFD)
{   
    int ret = 0;
    if (I2CWriteByte(devFD, WriteAddress, 6) == -1) {
        ret = -1;
    }
    if (I2CWriteByteTo(devFD, 0x00, 0x70, 6) == -1) {
        ret = -1;
    }
    if (I2CWriteByte(devFD, WriteAddress, 6) == -1) {
        ret = -1;
    }
    // continued reading mode
    if (I2CWriteByteTo(devFD, 0x02, 0x00, 6) == -1) {
        ret = -1;
    }
    // read command to check the mode
    if (I2CWriteByte(devFD, ReadAddress, 6) == -1) {
        ret = -1;
    }
    // printf("A=%d\n",I2CReadByteFrom(devFD, 0x00, 6));
    // printf("B=%d\n",I2CReadByteFrom(devFD, 0x02, 6));
    usleep(1000*100);
    return ret;
}

EXPORT double hmc5883Read(int devFD) 
{
    int x,x1,x2,y,y1,y2,z,z1,z2;
    double angle;

    I2CWriteByte(devFD, ReadAddress, 6);            // read command
    x1 = I2CReadByteFrom(devFD, 0x03, 6);
    x2 = I2CReadByteFrom(devFD, 0x04, 6);
    x = x1 << 8 | x2;                               // Combine MSB and LSB of X Data output register

    z1 = I2CReadByteFrom(devFD, 0x05, 6);
    z2 = I2CReadByteFrom(devFD, 0x06, 6);
    z = z1 << 8 | z2;                               // Combine MSB and LSB of Z Data output register  

    y1 = I2CReadByteFrom(devFD, 0x07, 6);
    y2 = I2CReadByteFrom(devFD, 0x08, 6);
    y = y1 << 8 | y2;                               // Combine MSB and LSB of Y Data output register
    if (x > 32768)
        x = -(0xFFFF - x + 1);
    if (z > 32768)
        z = -(0xFFFF - z + 1);
    if (y>32768)
        y = -(0xFFFF - y + 1);

    // printf("x=%d,y=%d,z=%d\n",x,y,z);
    angle = atan2((double)y,(double)x) * (180 / PI) + 180; // angle in degrees
    if(angle >=0 && angle <=360) {
        return angle;
    } else {
        return -1;
    }

}

EXPORT int hmc5883Init(int i2cDev) 
{
    clearLastError();
    int devFD;
    char buf[16];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "/dev/i2c-%d", i2cDev);
    if ((devFD = openHW(buf, O_RDWR)) < 0) {
        setLastError("Fail to open I2C hmc5883 device");
        return -1;
    } else {
        if (setI2CSlave(devFD, HMC5883_ADDRESS) < 0) {
            setLastError("Fail to set hmc5883 I2C slave address");
            closeHW(devFD);
            return -1;
        }
    } 
    if (hmc5883HWInit(devFD) == -1) {
        closeHW(devFD);
        return -1;
    }
    return devFD;
}

EXPORT void hmc5883DeInit(int devFD) {
    closeHW(devFD);
}
