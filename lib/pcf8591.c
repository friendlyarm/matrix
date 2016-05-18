#include <sys/types.h>
#include <dirent.h>
#include "common.h"
#include "libfahw-filectl.h"
#include "libfahw-pcf8591.h"

EXPORT int pcf8591Read(int channel, int *value)
{
    clearLastError();
    int existFlag = 0;
    DIR *d;
    struct dirent *de;
    char pcfFile[FILE_PATH_LENGTH];

    if (!(d = opendir(PCF8591_SYS_PATH))) {
        setLastError("Fail to opendir %s", PCF8591_SYS_PATH);
        return -1;
    }
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.')
            continue;
        sprintf(pcfFile, "%s%s/in%d_input", PCF8591_SYS_PATH, de->d_name, channel);
        if (access(pcfFile, F_OK) != -1) {
            existFlag = 1;
            break;
        }
    }
    closedir(d);

    if (existFlag == 0) {
        setLastError("Fail to access pcf8591 sys file");
        return -1;
    }
    if ((*value = readIntValueFromFile(pcfFile)) != -1) {
        return 0;
    } else {
        setLastError("Invalid dht11 data");
        return -1;
    }
}

