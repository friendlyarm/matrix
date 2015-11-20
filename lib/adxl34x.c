#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "common.h"
#include "libfahw-filectl.h"
#include "libfahw-adxl34x.h"

EXPORT int adxl34xRead(char *position) 
{
    clearLastError();
    int ret;
    int maxLength = sizeof("(-255, -255, -255)");
    DIR *d;
    struct dirent *de;
    
    if (!(d = opendir(ADXL34X_SYS_PATH))) {
        setLastError("Fail to opendir %s", ADXL34X_SYS_PATH);
        return 0;
    }
    char adxFile[FILE_PATH_LENGTH];
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.')
            continue;
        
        sprintf(adxFile, "%s%s/position", ADXL34X_SYS_PATH, de->d_name);
        
        if (access(adxFile, F_OK) != -1) {
            break;
        }
    }
    closedir(d);

    memset(position, 0, maxLength);
    if ((ret = readValueFromFile(adxFile, position, maxLength)) == -1) {
        setLastError("Fail to read adxl34x");
        return -1;
    }
    ret = strlen(position) + 1;
    return ret;
}
