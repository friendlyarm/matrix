#include "common.h"
#include "libfahw-filectl.h"
#include "libfahw-pwm.h"

EXPORT int PWMPlay(int pin, int freq, int duty) 
{
    clearLastError();
    int arg[3];
    int devFD = -1;
    arg[0] = pin;
    arg[1] = freq;
    arg[2] = duty;

    if (duty < 0 || duty > 1000) {
        setLastError("Invalid duty\n");
        return -1;
    }

    if ((devFD = openHW("/dev/pwm", O_RDONLY)) == -1) {
        setLastError("Fail to open pwm device");
        return -1;
    }

    if (ioctl(devFD, PWM_IOCTL_SET_FREQ, arg) == -1) {
        setLastError("Fail to set pwm");
        closeHW(devFD);
        return -1;
    }
    closeHW(devFD);
    return 0;
}

EXPORT int PWMStop(int pin) 
{
    clearLastError();
    int devFD = -1;

    if ((devFD = openHW("/dev/pwm", O_RDONLY)) == -1) {
        setLastError("Fail to open pwm device");
        return -1;
    }

    if (ioctl(devFD, PWM_IOCTL_STOP, &pin) == -1) {
        setLastError("Fail to stop pwm");
        closeHW(devFD);
        return -1;
    }
    close(devFD);
    return 0;
}
