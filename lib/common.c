#define _GNU_SOURCE
#include "common.h"

int writeValueToFile(char* fileName, char* buff) {
    FILE *fp = fopen(fileName,"w");
    if (fp == NULL) {
        setLastError("Unable to open file %s",fileName);
        return -1;
    } else {
        fwrite ( buff, strlen(buff), 1, fp );
    }
    fclose(fp);
    return 0;
}


int writeIntValueToFile(char* fileName, int value) {
    char buff[50];
    sprintf(buff, "%d", value);
    return writeValueToFile(fileName, buff);
}


int readValueFromFile(char* fileName, char* buff, int len) {
    int ret = -1;
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL) {
        setLastError("Unable to open file %s",fileName);
        return -1;
    } else {
        if (fread(buff, sizeof(char), len, fp)>0) {
            ret = 0;
        }
    }
    fclose(fp);
    return ret;
}


int readIntValueFromFile(char* fileName) {
    char buff[255];
    memset(buff, 0, sizeof(buff));
    int ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
    if (ret == 0) {
        return atoi(buff);
    }
    return ret;
}

static char* getBoardInfo()
{
    int n,i,j;
    char lineUntrim[1024], line[1024],*p;
    FILE *f;
    static char result[255];
    
    if (!(f = fopen("/proc/cpuinfo", "r"))) {
    	setLastError("Unable to open /proc/cpuinfo");
        return 0;
    }
    
    while (!feof(f)) {
        if(!fgets(lineUntrim, sizeof(lineUntrim), f)) {
            return 0;
        } else {
            j=0;
            for(i=0; i<strlen(lineUntrim);i++) {
                if (lineUntrim[i] != ' ') {
                    line[j++]=lineUntrim[i];
                }
            }
            line[j] = 0x00;
            n = strlen(line);
            if (n>0) {
                line[--n] = '\0';
                if ( (p = strtok(line, ":")) ) {
                    if (strncasecmp(p, "Hardware", strlen("Hardware")) == 0) {
                        if ( (p = strtok(0, ":")) ) {
                            memset(result,0,255);
                            strncpy(result, p, 254);
                            fclose(f);
                            return result;
                        }
                    }
                }
            }
        }
    }
    fclose(f);
    return 0;
}

int getBoardType() {
    
    int ret = -1;
    
    char* boardType = getBoardInfo();
    if (strncasecmp(boardType, "MINI6410", 8)==0) {
        ret = 6410;
    } else if (strncasecmp(boardType, "MINI210", 7)==0) {
        ret = 210;
    } else if (strncasecmp(boardType, "TINY4412", 8)==0) {
        ret = 4412;
    }
    
    return ret;
}

static char FAHWLastError[255];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void clearLastError() {
    pthread_mutex_lock(&mutex);
    memset(FAHWLastError, 0, sizeof(FAHWLastError));
    pthread_mutex_unlock(&mutex);
}

void setLastError(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    char* errMsg;
    if (vasprintf(&errMsg, fmt, vl) > 0) {
        DEBUG("%s\n", errMsg);
        pthread_mutex_lock(&mutex);
        strncpy(FAHWLastError, errMsg, (sizeof(FAHWLastError)-1));
        pthread_mutex_unlock(&mutex);
        free(errMsg);
    }
    va_end(vl);
}

EXPORT int getLastError(char* dest, int maxlen) {
    if (dest == NULL || maxlen<=0) {
        return -1;
    }
    pthread_mutex_lock(&mutex);
    int ret = strlen(FAHWLastError);
    strncpy(dest, FAHWLastError, maxlen);
    pthread_mutex_unlock(&mutex);

    return ret;
}

int Test() {
    setLastError("TestFunction");
    return 999;
}

