#define _GNU_SOURCE
#include "common.h"
#include "libfahw.h"

int writeValueToFile(char* fileName, char* buff) 
{
    clearLastError();
    int ret;
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) {
        setLastError("Unable to open file %s", fileName);
        ret = -1;
    } else {
        ret = fwrite(buff, strlen(buff), 1, fp);
        fclose(fp);
    }
    return ret;
}


int writeIntValueToFile(char* fileName, int value) 
{
    clearLastError();
    char buff[50];
    sprintf(buff, "%d", value);
    return writeValueToFile(fileName, buff);
}


int readValueFromFile(char* fileName, char* buff, int len) 
{
    clearLastError();
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


int readIntValueFromFile(char* fileName) 
{
    clearLastError();
    char buff[255];
    memset(buff, 0, sizeof(buff));
    int ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
    if (ret == 0) {
        return atoi(buff);
    }
    return ret;
}

static int getBoardInfo(int length, char info[][32])
{
    clearLastError();
    int n,i,j;
    char lineUntrim[1024], line[1024],*p;
    FILE *f;
    int ret = 0;
    
    f = fopen("/sys/devices/platform/board/info", "r");
    if (!f)
        f = fopen("/proc/cpuinfo", "r");

    if (!f) {
        setLastError("Unable to get board info");
        return ret;
    }
    
    while (!feof(f)) {
        if(!fgets(lineUntrim, sizeof(lineUntrim), f)) {
            return ret;
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
                            printf("Hardware:%s\n", p);
                            memset(info[0], 0, 32);
                            strncpy(info[0], p, 32-1);
                            ret++;
                        }
                    } else if (strncasecmp(p, "Revision", strlen("Revision")) == 0) {
                        if ( (p = strtok(0, ":")) ) {
                            printf("Revision:%s\n", p);
                            memset(info[1], 0, 32);
                            strncpy(info[1], p, 32-1);
                            ret++;
                        }
                    }
                }
            }
        }
    }
    fclose(f);
    return ret;
}

static int getBoardType() 
{    
    clearLastError();
    int ret = -1;
    char info[2][32];
    int boardType = getBoardInfo(2, info);
    if (boardType < 1) {
        return ret;
    }
    if (strncasecmp(info[0], "MINI6410", 8)==0) {
        ret = BOARD_MINI6410;
    } else if (strncasecmp(info[0], "MINI210", 7)==0) {
        ret = BOARD_MINI210;
    } else if (strncasecmp(info[0], "TINY4412", 8)==0) {
        ret = BOARD_TINY4412;
    } else if (strncasecmp(info[0], "sun8i", 5)==0) {
        ret = BOARD_NANOPI_M1;
    } else if (strncasecmp(info[0], "NANOPI2", 7)==0) {
        if (strncasecmp(info[1], "0000", 4)==0) {
            ret = BOARD_NANOPI_2;
        } else if (strncasecmp(info[1], "0004", 4)==0) {
            ret = BOARD_NANOPI_2_FIRE;
        } else if (strncasecmp(info[1], "0005", 4)==0) {
            ret = BOARD_NANOPI_M2;   
        } else if (strncasecmp(info[1], "0001", 4)==0) {
            ret = BOARD_NANOPC_T2;
        }
    } else if(strncasecmp(info[0], "NANOPI3", 7)==0) {
        if (strncasecmp(info[1], "0007", 4)==0) {
            ret = BOARD_NANOPI_M3;
		} else if (strncasecmp(info[1], "0001", 4)==0) {
			ret = BOARD_NANOPC_T3;
		}
   }
    
    return ret;
}

EXPORT int boardInit()
{
    clearLastError();
    int board = getBoardType();
    printf("BoardType:%d\n", board);
    if (initPinGPIO(board))
        setLastError("Fail to initPinGPIO");
    if (initPwmGPIO(board))
        setLastError("Fail to initPwmGPIO");
    return board;
}

static char FAHWLastError[255];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
EXPORT void clearLastError() 
{
    pthread_mutex_lock(&mutex);
    memset(FAHWLastError, 0, sizeof(FAHWLastError));
    pthread_mutex_unlock(&mutex);
}

EXPORT void setLastError(const char *fmt, ...) 
{
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

EXPORT int getLastError(char* dest, int maxlen) 
{
    if (dest == NULL || maxlen<=0) {
        return -1;
    }
    pthread_mutex_lock(&mutex);
    int ret = strlen(FAHWLastError);
    strncpy(dest, FAHWLastError, maxlen);
    pthread_mutex_unlock(&mutex);

    return ret;
}

int Test() 
{
    setLastError("TestFunction");
    return 999;
}
