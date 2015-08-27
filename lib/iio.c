#include "common.h"
#include "libfahw-iio.h"
#include "libfahw-filectl.h"

EXPORT int dht11Init(int pin) 
{
    int dht11Pin = pin;
    clearLastError();
    int devFD = openHW(DHT11_DEV, O_RDONLY);
    if (devFD == -1) {
        printf("Fail to open dht11");
        return -1;
    }
    if(ioctl(devFD, SET_DHT11_PIN, &dht11Pin) == -1) {
        setLastError("Fail to set dht11 pin");
        closeHW(devFD);
        return -1;
    }
    
    dht11Pin = -1;
    if(ioctl(devFD, GET_DHT11_PIN, &dht11Pin) == -1) {
        setLastError("Fail to get dht11 pin");
        closeHW(devFD);
        return -1;
    }
    if (dht11Pin != pin) {
        setLastError("Get error dht11 pin %d", dht11Pin);
        closeHW(devFD);
        return -1;
    }
    
    return devFD;
}

EXPORT void dht11DeInit(int devFD) 
{
    clearLastError();
    if(ioctl(devFD, UNSET_DHT11_PIN, 0) == -1) {
        setLastError("Fail to unset dht11 pin\n");
    }    
    closeHW(devFD);
}

EXPORT int dht11Read(int type, int *data) 
{
    clearLastError();
    int ret = -1;
    char *dht11Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(dht11Path, 0, FILE_PATH_LENGTH);
    strcpy(dht11Path, DHT11_PATH);

    switch(type) {
    case DHT_TEMP:
        strcat(dht11Path, "in_temp_input");
        ret = readIntValueFromFile(dht11Path);
        break;
    case DHT_HUMIDITY:
        strcat(dht11Path, "in_in_input");
        ret = readIntValueFromFile(dht11Path);
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

    free(dht11Path);
    return ret;
}

static int Hcsr04Write(char* fileName, char* buff) {
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

EXPORT int Hcsr04Init(int echoPin, int triggerPin) 
{
    clearLastError();
    struct HCSR04_resource res;
    int ret = HCSR04_MAX_DISTANCE+1;
    char *hcsr04Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(hcsr04Path, 0, FILE_PATH_LENGTH);
    strcpy(hcsr04Path, HCSR04_PATH);

    res.echoPin = echoPin;
    res.triggerPin = triggerPin;
    
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
    res.echoPin = -1;
    res.triggerPin = -1;

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
    int errCount = -1;

    char *hcsr04Path = (char *) malloc(FILE_PATH_LENGTH);
    memset(hcsr04Path, 0, FILE_PATH_LENGTH);
    strcpy(hcsr04Path, HCSR04_PATH);

    while(ret > HCSR04_MAX_DISTANCE && errCount < 5) {
        ret = readIntValueFromFile(hcsr04Path);
        errCount++;
    }

    if (errCount == 5) {
        ret = -1;
        setLastError("Invalid hcsr04 data");
    } else {            
        *distance = (int)(ret / HCSR04_PER_METRE);
        ret = 0;
    }

    free(hcsr04Path);
    return ret;
}
