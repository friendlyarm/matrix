#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "common.h"
#include "libfahw-bmp180.h"

EXPORT int bmp180Read(int type, int *data) 
{
    clearLastError();
    int ret = -1;
    DIR *d;
    struct dirent *de;
    
    if (!(d = opendir(BMP180_SYS_PATH))) {
        setLastError("Fail to opendir %s", BMP180_SYS_PATH);
        return 0;
    }
    char bmpFile[FILE_PATH_LENGTH];
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.')
            continue;
        
        if (type == BMP180_TEMP)
            sprintf(bmpFile, "%s%s/temp0_input", BMP180_SYS_PATH, de->d_name);
        else if(type == BMP180_PRESSURE)
            sprintf(bmpFile, "%s%s/pressure0_input", BMP180_SYS_PATH, de->d_name);
        
        if (access(bmpFile, F_OK) != -1) {
            break;
        }
    }
    closedir(d);
    
    switch(type) {
    case BMP180_TEMP:
        ret = readIntValueFromFile(bmpFile);
        break;
    case BMP180_PRESSURE:
        ret = readIntValueFromFile(bmpFile);
        break;
    default:
        setLastError("Unsupport bmp180 data type %d", type);
        break;
    }
    if (ret != -1) {
        *data = ret;
        ret = 0;
    } else {
        setLastError("Invalid bmp180 data");
    }
    
    return ret;
}
