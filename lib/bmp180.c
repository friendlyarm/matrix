#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "libfahw-bmp180.h"

EXPORT int bmp180Read(int type, int *data) 
{
    clearLastError();
    int ret = -1;
    char *bmp180Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(bmp180Path, 0, FILE_PATH_LENGTH);
    strcpy(bmp180Path, BMP180_PATH);

    switch(type) {
    case BMP180_TEMP:
        strcat(bmp180Path, "temp0_input");
        ret = readIntValueFromFile(bmp180Path);
        break;
    case BMP180_PRESSURE:
        strcat(bmp180Path, "pressure0_input");
        ret = readIntValueFromFile(bmp180Path);
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

    free(bmp180Path);
    return ret;
}
