#include "common.h"
#include "libfahw-gpio.h"
#include "libfahw-GPIOSensor.h"

static int sensor_num = 0;

EXPORT int sensorInit(struct sensor *dev, int num) 
{
    clearLastError();
    int devFD;
    int i;
    
    devFD = open(SENSOR_DEVICE, 0);
    if (devFD == -1) {
        setLastError("Fail to open %s", SENSOR_DEVICE);
    }

    for(i=0; i<num; i++) {
        dev[i].pin = pintoGPIO(dev[i].pin);
        if (dev[i].pin == -1) {
            return -1;
        }
        if(ioctl(devFD, ADD_SENSOR, &dev[i]) == -1) {
            setLastError("Fail to add sensor");
            return -1;
        }
        sensor_num++;
    }

    if(ioctl(devFD, START_ALL_SENSOR, 0) == -1) {
        setLastError("Fail to start sensor");
        return -1;
    }
    return devFD;
}

EXPORT int sensorRead(int devFD, char *buf, int len) 
{
    clearLastError();
    int retSize = 0;
    if ((retSize = read(devFD, buf, len)) < 0) {
        setLastError("Fail to read sensor");
        return -1;
    }
    return retSize;
}

EXPORT void sensorDeinit(int devFD) 
{
    clearLastError();
    if(ioctl(devFD, STOP_ALL_SENSOR, 0) == -1) {
        setLastError("Fail to stop sensor");
    }

    for(; sensor_num>0; sensor_num--) {
        if(ioctl(devFD, DEL_SENSOR, sensor_num) == -1) {
            setLastError("Fail to delete sensor");
        }
    }
    close(devFD);
}
