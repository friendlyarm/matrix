#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "libfahw-pcf8591.h"
#include "libfahw-filectl.h"
#include "libfahw-i2c.h"
#include "common.h"
#include "includes/i2c-dev.h"

static int currentCtrl = 0;

EXPORT void pcf8591DeInit(int devFD)
{
    clearLastError();
    closeHW(devFD);
}
/*
 * send the control byte to pcf8591,four single ended A inputs,read the channel 0
 */
EXPORT int pcf8591Init() 
{
    clearLastError();
    int devFD = openHW(I2C0_PATH, O_RDWR);
    if (devFD == -1) {
        setLastError("Fail to open I2C device pcf8591");
        return -1;
    } else {
        if (setI2CSlave(devFD, PCF8591_ADDRESS) == -1) {
            setLastError("Fail to set pcf8591 I2C slave address");
            closeHW(devFD);
            return -1;
        }
    }     
    return devFD;
}

EXPORT int pcf8591SetCtrl(int devFD, int control)
{
    if ((control != PCF8591_INIT_AD_CONTROL) && (control != PCF8591_INIT_DA_CONTROL)) {
        setLastError("Unsupported pcf8591 control type");
        return -1;
    }

    currentCtrl = control;
    if (I2CWriteByte(devFD, control, 0) == -1) {
        setLastError("Fail to write pcf8591 control byte"); 
        return -1;
    }
    I2CReadByte(devFD, 0);
    return 0;
}

/*
 * read the channel you selected
 */
EXPORT int pcf8591Read(int devFD, int mode, int channel) 
{
    clearLastError();
    if( currentCtrl != PCF8591_INIT_AD_CONTROL) {
        setLastError("Pcf8591 is not AD control"); 
        return -1;
    }

    unsigned char val;
    int mode_tmp = ((mode << 4) | PCF8591_INIT_AD_CONTROL);
    if ((mode_tmp & PCF8591_CONTROL_AICH_MASK) != channel) {
        unsigned int control = ( mode_tmp | channel );
        I2CWriteByte(devFD, control,0);
        /*
         * The first byte transmitted contains the conversion code of
         * the previous read cycle. FLUSH IT!
         */
        I2CReadByte(devFD, 0);
    } else {
        I2CWriteByte(devFD, mode_tmp, 0);
        I2CReadByte(devFD, 0);
    }
    usleep(1000*10);
    val = I2CReadByte(devFD, 0);
    if ( val == -1) {
        setLastError("Fail to read pcf8591 AD"); 
        return -1;
    }
    if((channel == 2 && mode == 2) || ((channel != 3) && (mode == 1 || mode == 3))) {
        return REG_TO_SIGNED(val);
    } 
    return val;
}

EXPORT int pcf8591ReadChannels(int devFD,int mode,int channels[], int values[], int count)
{
    int ret = 0;
    int i;
    for (i=0; i < count; i++) {
        values[i] = pcf8591Read(devFD, mode, channels[i]);
        if (values[i] < 0) {
            ret = values[i];
        }
    }
    return ret;
}


EXPORT int pcf8591Write(int devFD, int analog) 
{
    clearLastError();
    int ret = i2c_smbus_write_byte(devFD, analog);
    if(ret == -1){
        setLastError("Fail to write pcf8591 DA"); 
        return ret;
    } 
    i2c_smbus_read_byte(devFD);
    return 0;
}
