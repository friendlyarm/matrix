#include "common.h"
#include "libfahw-GPIOSensor.h"

static int sensor_num = -1;

EXPORT int sensorInit(struct sensor *dev, int num) {
    clearLastError();
    int devFD;
    sensor_num = num;
    
    devFD = open(SENSOR_DEVICE, 0);
    if (devFD == -1) {
        setLastError("Fail to open %s\n", SENSOR_DEVICE);
    }

    int i;

    for(i=0; i<num; i++)
    {
        if(ioctl(devFD, ADD_SENSOR, &dev[i]) == -1) {
            setLastError("Fail to add sensor\n");
        }
    }

    if(ioctl(devFD, START_ALL_SENSOR, 0) == -1) {
        setLastError("Fail to start sensor\n");
    }
    return devFD;
}

EXPORT int sensorRead(int devFD, char *buf, int len) {
    clearLastError();
    int retSize = 0;
    if ((retSize = read(devFD, buf, len)) < 0) {
        setLastError("Fail to read sensor\n");
        return -1;
    }
    return retSize;
}

EXPORT void sensorDeinit(int devFD) {
    clearLastError();
    int i = 0;
    if(ioctl(devFD, STOP_ALL_SENSOR, 0) == -1) {
        setLastError("Fail to stop sensor\n");
    }

    for(i=0; i<sensor_num; i++)
    {
        if(ioctl(devFD, DEL_SENSOR, 0) == -1) {
            setLastError("Fail to delete sensor\n");
        }
    }
    close(devFD);
    sensor_num = -1;
}
