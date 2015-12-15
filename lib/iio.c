#include <sys/types.h>
#include <dirent.h>
#include "common.h"
#include "libfahw-iio.h"
#include "libfahw-gpio.h"
#include "libfahw-filectl.h"

EXPORT int dht11Read(int type, int *data) 
{
    clearLastError();
    int ret = -1;
    DIR *d;
    struct dirent *de;
    
    if (!(d = opendir(DHT11_SYS_PATH))) {
        setLastError("Fail to opendir %s", DHT11_SYS_PATH);
        return 0;
    }
    char dht11File[FILE_PATH_LENGTH];
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.')
            continue;
        
        if (type == DHT_HUMIDITY)
            sprintf(dht11File, "%s%s/in_humidityrelative_input", DHT11_SYS_PATH, de->d_name);
        else if(type == DHT_TEMP)
            sprintf(dht11File, "%s%s/in_temp_input", DHT11_SYS_PATH, de->d_name);
        
        if (access(dht11File, F_OK) != -1) {
            break;
        }
    }
    closedir(d);

    switch(type) {
    case DHT_TEMP:
        ret = readIntValueFromFile(dht11File);
        break;
    case DHT_HUMIDITY:
        ret = readIntValueFromFile(dht11File);
        break;
    default:
        setLastError("Unsupport dht11 data type %d", type);
        break;
    }
    if (ret != -1) {
        *data = ret;
        ret = 0;
    } else {
        setLastError("Invalid dht11 data");
    }

    return ret;
}

static int Hcsr04Write(char* fileName, char* buff) 
{
    FILE *fp = fopen(fileName,"w");
    if (fp == NULL) {
        setLastError("Unable to open file %s",fileName);
        return -1;
    } else {
        if (fwrite(buff, sizeof(struct HCSR04_resource), 1, fp) == -1) {
            setLastError("Unable to write file %s",fileName);
        }
    }
    fclose(fp);
    return 0;
}

EXPORT int Hcsr04Init(int Pin) 
{
    clearLastError();
    struct HCSR04_resource res;
    int ret = HCSR04_MAX_DISTANCE+1;
    char *hcsr04Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(hcsr04Path, 0, FILE_PATH_LENGTH);
    strcpy(hcsr04Path, HCSR04_PATH);

    res.gpio = pintoGPIO(Pin);
    char *resStr = (char *)&res;    
    if (Hcsr04Write(hcsr04Path, resStr) == -1) {
        setLastError("Fail to write resource to hcsr04");
        ret = -1;
    }
    free(hcsr04Path);
    return ret;
}

EXPORT void Hcsr04DeInit() 
{
    clearLastError();
    struct HCSR04_resource res;
    res.gpio = -1;

    char *hcsr04Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(hcsr04Path, 0, FILE_PATH_LENGTH);
    strcpy(hcsr04Path, HCSR04_PATH);

    char *resStr = (char *)&res;    
    if (Hcsr04Write(hcsr04Path, resStr) == -1) {
        setLastError("Fail to write resource to hcsr04");
    }    
    free(hcsr04Path);
}

EXPORT int Hcsr04Read(int *distance) 
{
    clearLastError();
    int ret = HCSR04_MAX_DISTANCE + 1;
    char *hcsr04Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(hcsr04Path, 0, FILE_PATH_LENGTH);
    strcpy(hcsr04Path, HCSR04_PATH);
    ret = readIntValueFromFile(hcsr04Path);
    *distance = (int)(ret / HCSR04_PER_METRE);
    free(hcsr04Path);
    return ret;
}
