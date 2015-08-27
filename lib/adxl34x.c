#include <math.h>
#include <string.h>
#include "common.h"
#include "libfahw-filectl.h"
#include "libfahw-adxl34x.h"

#define ADXL34X_SYS_PATH    "/sys/bus/i2c/drivers/adxl34x/0-001d/position"
EXPORT int adxl34xRead(char *position) 
{
    clearLastError();
    int ret;
    int maxLength = sizeof("(-255, -255, -255)");
    
    memset(position, 0, maxLength);
    if ((ret = readValueFromFile(ADXL34X_SYS_PATH, position, maxLength)) == -1) {
        setLastError("Fail to read adxl34x");
        return -1;
    }
    ret = strlen(position) + 1;
    return ret;
}
