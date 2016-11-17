#include "common.h"
#include "libfahw-filectl.h"
#include "libfahw-pwm.h"

static int pwmGPIO[3];
static int pwmNum = -1;
int initPwmGPIO(int board)
{
    clearLastError();
    int ret = -1;
    
    memset(pwmGPIO, -1, sizeof(pwmGPIO));
    switch(board) {
    case BOARD_NANOPI_M1: {
        int buf[2] = {5, 6};
        memcpy(pwmGPIO, buf, sizeof(buf));
        pwmNum = 2;
        ret = 0;
        break;
    }
    case BOARD_NANOPI_2: {
        int buf[3] = {97, 77, 78};
        memcpy(pwmGPIO, buf, sizeof(buf));
        pwmNum = 3;
        ret = 0;
        break;
    }
    case BOARD_NANOPI_2_FIRE:
    case BOARD_NANOPI_M2: {
        int buf[3] = {97, 77, 78};
        memcpy(pwmGPIO, buf, sizeof(buf));
        pwmNum = 3;
        ret = 0;
        break;
    }
    case BOARD_NANOPC_T2: {
        int buf[3] = {97, 77, 78};
        memcpy(pwmGPIO, buf, sizeof(buf));
        pwmNum = 3;
        ret = 0;
        break;
    }
   case BOARD_NANOPI_M3: {
        int buf[3] = {97, 77, 78};
        memcpy(pwmGPIO, buf, sizeof(buf));
        pwmNum = 3;
        ret = 0;
        break;
    }
   case BOARD_NANOPC_T3: {
        int buf[3] = {97, 77, 78};
        memcpy(pwmGPIO, buf, sizeof(buf));
        pwmNum = 3;
        ret = 0;
        break;
    }
    default:
        ret = -1;
        break;
    }
    return ret;
}

EXPORT int pwmtoGPIO(int pwm)
{
    clearLastError();

    if (pwm<0 || pwm>=pwmNum || pwmGPIO[pwm]==-1) {
        setLastError("invalid pwm %d", pwm);
        return -1;
    }
    return pwmGPIO[pwm];
}

EXPORT int PWMPlay(int pwm, int freq, int duty) 
{
    clearLastError();
    int arg[3];
    int devFD = -1;
    int gpio = pwmtoGPIO(pwm);
    arg[0] = gpio;
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

EXPORT int PWMStop(int pwm) 
{
    clearLastError();
    int devFD = -1;
    int gpio = pwmtoGPIO(pwm);
    
    if ((devFD = openHW("/dev/pwm", O_RDONLY)) == -1) {
        setLastError("Fail to open pwm device");
        return -1;
    }

    if (ioctl(devFD, PWM_IOCTL_STOP, &gpio) == -1) {
        setLastError("Fail to stop pwm %d", pwm);
        closeHW(devFD);
        return -1;
    }
    close(devFD);
    return 0;
}
